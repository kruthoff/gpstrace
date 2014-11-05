gpstrace
========

28.07.2014 - AKR - Initial version.

This code can be used to create GPS traces on a Raspberry Pi without network connection, but with a GPS receiver using UART. It is used as a mobile device (i.e. in a car). A small TFT display was attached to see what's happening.

The scripts which are required to run gpstrace were put into the operating system, with the intention to "just boot" your Raspberry Pi and "everything works automatically".

01.11.2014 - AKR - Navit.

Navit http://www.navit-project.org/ was added to the setup, to see the current position on the TFT.
Sample configurations for 2.8" and 3.5" TFT's were added.



See the [WIKI](https://github.com/kruthoff/gpstrace/wiki) for more information.


Files
-----
<pre>
/
├── etc
│   ├── default
│   │   ├── console-setup                 defines a small console font
│   │   ├── gpsd                          flags for gpsd on GPIO
│   │   ├── gpstrace                      flags for gpstrace script
│   │   └── tftbuttons                    flags for 2.8" TFT tftbuttons script
│   ├── init.d
│   │   ├── gpstrace                      boot console start/stop script
│   │   └── tftbuttons                    start/stop script
│   ├── kbd
│   │   └── config                        disable boot console screensaver
│   ├── navit
│   │   └── navit.xml                     navit configuration with OSD for 3.5" TFT
│   ├── ntp.conf                          get time from gpsd
│   ├── profile.d
│   │   └── tft.sh                        set FRAMEBUFFER device for TFT at login
│   └── rc.local                          start rpi_gpio_ntp, startx
├── home
│   └── pi
│       ├── .config
│       │   └── autostart
│       │       ├── gpstrace.desktop      start gpstrace when desktop starts (default)
│       │       └── navit.desktop         start navit when desktop starts
│       ├── gps
│       │   ├── gpstime.py                set initial gps time to OS 
│       │   ├── gpstrace                  main trace script
│       │   ├── id                        persisted unique trace filename prefix
│       │   ├── startgpstrace.sh          start gpstrace in terminal when desktop starts
│       │   └── traces                    traces are stored here
│       └── tft
│           ├── duty                      store brightness of 3.5" TFT backlight
│           ├── tftbuttons                handle 2.8" TFT buttons, i.e. switch backlight
│           └── toggle.sh                 switch brightness of 3.5" TFT backlight
├── LICENSE
├── README.md
├── usr
│   └── local
│       ├── bin
│       │   └── rpi_gpio_ntp              program to set the initial time from PPS
│       └── src
│           └── rpi_gpio_ntp-0.3.tar.bz2  src code
└── var
    └── log
        └── gpstrace                      gpstrace log dir, if started via boot console
</pre>


References
----------

* https://learn.adafruit.com/adafruit-ultimate-gps-on-the-raspberry-pi/
* https://learn.adafruit.com/adafruit-nfc-rfid-on-raspberry-pi/freeing-uart-on-the-pi/
* https://learn.adafruit.com/adafruit-pitft-28-inch-resistive-touchscreen-display-raspberry-pi/
* https://learn.adafruit.com/adafruit-pitft-3-dot-5-touch-screen-for-raspberry-pi/
* http://code.google.com/p/gpstime/
* http://vanheusden.com/time/rpi_gpio_ntp/
* http://www.raspberrypi-spy.co.uk/2014/04/how-to-change-the-command-line-font-size/

