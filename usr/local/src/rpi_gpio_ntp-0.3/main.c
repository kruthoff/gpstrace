#include <poll.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/time.h>

#include "error.h"
#include "gpio-int-test.h"

#define NTP_KEY	1314148400
#define DEFAULT_FUDGE_FACTOR 0.0

struct shmTime {
	int    mode; /* 0 - if valid set
		      *       use values, 
		      *       clear valid
		      * 1 - if valid set 
		      *       if count before and after read of 
		      *       values is equal,
		      *         use values 
		      *       clear valid
		      */
	int    count;
	time_t clockTimeStampSec;      /* external clock */
	int    clockTimeStampUSec;     /* external clock */
	time_t receiveTimeStampSec;    /* internal clock, when external value was received */
	int    receiveTimeStampUSec;   /* internal clock, when external value was received */
	int    leap;
	int    precision;
	int    nsamples;
	int    valid;
	int    dummy[10]; 
};

int write_pidfile(char *fname)
{
	FILE *fh = fopen(fname, "w");
	if (!fh)
		error_exit("write_pidfile: failed creating file %s", fname);

	fprintf(fh, "%d", getpid());

	fclose(fh);

	return 0;
}

struct shmTime * get_shm_pointer(int unitNr)
{
	void *addr;
	struct shmTime *pst;
	int shmid = shmget(NTP_KEY + unitNr, sizeof(struct shmTime), IPC_CREAT);
	if (shmid == -1)
		error_exit("get_shm_pointer: shmget failed");

	addr = shmat(shmid, NULL, 0);
	if (addr == (void *)-1)
		error_exit("get_shm_pointer: shmat failed");

	pst = (struct shmTime *)addr;

	return pst;
}

void notify_ntp(struct shmTime *pst, int fudge_s, int fudge_ns)
{
	struct timespec ts;

	pst -> valid = 0;

	/* see what time the local system thinks it is, ASAP */
	if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
		error_exit("clock_gettime(CLOCK_REALTIME) failed");

	/* apply fudge */
	ts.tv_sec += fudge_s;
	ts.tv_nsec += fudge_ns;

	if (ts.tv_nsec > 999999999)
	{
		ts.tv_nsec -= 1000000000;
		ts.tv_sec++;
	}

	/* if local time is more than 0.5 seconds off, assume
	 * it is the next second
	 */
	pst -> receiveTimeStampSec = ts.tv_sec;
	pst -> receiveTimeStampUSec = ts.tv_nsec / 1000;

	if (ts.tv_nsec >= 500000000)
		ts.tv_sec++;

	pst -> clockTimeStampSec = ts.tv_sec;
	pst -> clockTimeStampUSec = 0;

	pst -> leap = pst -> mode = pst -> count = /* */
	pst -> precision = 0;	/* 0 = precision of 1 sec., -1 = 0.5s */

	pst -> valid = 1;
}

void set_prio(void)
{
	struct sched_param p;

	if (nice(-20) == -1)
		error_exit("nice() failed");

	p.sched_priority = sched_get_priority_max(SCHED_RR);

	if (sched_setscheduler(0, SCHED_RR, &p) == -1)
		error_exit("sched_setscheduler() failed");
}

void help(void)
{
	fprintf(stderr, "-N x    x must be 0...3, it is the NTP shared memory unit number\n");
	fprintf(stderr, "-g x    gpio pin to listen on\n");
	fprintf(stderr, "-d      debug mode\n");
	fprintf(stderr, "-F x    fudge factor (in microseconds)\n");
	fprintf(stderr, "-p x    when enabled, toggle GPIO pin x so that you can measure delays using a scope\n");
	fprintf(stderr, "-f      do not fork\n");
}

int main(int argc, char *argv[])
{
	struct shmTime *pst = NULL;
	int fudge_s = 0, fudge_ns = 0;
	int unit = 0; /* 0...3 */
	int gpio_pps_in_fd = -1, gpio_pps_in_pin = -1, gpio_pps_out_pin = -1;
	char debug = 0, do_fork = 1, gpio_pps_out_pin_value = 1;
	int c = -1;

	printf("rpi_gpio_ntp v" VERSION ", (C) 2013 by folkert@vanheusden.com\n\n");

	while((c = getopt(argc, argv, "p:fN:g:F:dh")) != -1)
	{
		switch(c)
		{
			case 'p':
				gpio_pps_out_pin = atoi(optarg);
				break;

			case 'f':
				do_fork = 0;
				break;

			case 'N':
				unit = atoi(optarg);
				break;

			case 'g':
				gpio_pps_in_pin = atoi(optarg);
				break;

			case 'F':
				fudge_s = atol(optarg) / 1000000;
				fudge_ns = (atol(optarg) % 1000000) * 1000;
				break;

			case 'd':
				debug = 1;
				break;

			case 'h':
				help();
				return 0;

			default:
				error_exit("%c is an invalid switch", c);
		}
	}

	if (gpio_pps_in_pin == -1)
		error_exit("You need to select a GPIO pin to \"listen\" on.");

	if (gpio_pps_out_pin == gpio_pps_in_pin)
		error_exit("You can't use the same pin for both in- and output.");

	if (debug)
	{
		printf("NTP unit: %d\n", unit);
		printf("GPIO pin: %d\n", gpio_pps_in_pin);
		printf("Fudge   : %d.%09d\n", fudge_s, fudge_ns);

		if (do_fork)
		{
			do_fork = 0;

			printf("\"Fork into the background\" disabled because of debug mode.\n");
		}
	}

	if (mlockall(MCL_CURRENT) == -1)
		error_exit("mlockall(MCL_CURRENT)");

	/* connect to ntp */
	pst = get_shm_pointer(unit);

	/* setup gpio */
	gpio_export(gpio_pps_in_pin);
	gpio_set_dir(gpio_pps_in_pin, 0);
	gpio_set_edge(gpio_pps_in_pin, "rising");
	gpio_pps_in_fd = gpio_fd_open(gpio_pps_in_pin);

	if (gpio_pps_out_pin != -1)
	{
		gpio_export(gpio_pps_out_pin);
		gpio_set_dir(gpio_pps_out_pin, 1);
	}

	set_prio();

	if (do_fork)
	{
		if (daemon(0, 0) == -1)
			error_exit("daemon() failed");
	}

	for(;;)
	{
		int rc;
		struct pollfd fdset[1];

		fdset[0].fd = gpio_pps_in_fd;
		fdset[0].events = POLLPRI;
		fdset[0].revents = 0;

		rc = poll(fdset, 1, -1);

		if (rc <= 0)
		{
			if (rc == 0)	/* should not happen due to the -1 timeout */
				continue;

			error_exit("poll() failed");
		}
            
		if (fdset[0].revents & POLLPRI)
		{
			char buffer[64];

			notify_ntp(pst, fudge_s, fudge_ns);

			(void)read(fdset[0].fd, buffer, sizeof buffer);

			if (gpio_pps_out_pin != -1)
			{
				gpio_set_value(gpio_pps_out_pin, gpio_pps_out_pin_value);

				gpio_pps_out_pin_value = !gpio_pps_out_pin_value;
			}

			if (debug)
			{
				struct timespec ts;

				if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
					error_exit("clock_gettime(CLOCK_REALTIME) failed");

				printf("%ld.%09ld] poll() GPIO %d interrupt occurred\n", ts.tv_sec, ts.tv_nsec, gpio_pps_in_pin);
			}
		}
	}

	return 0;
}
