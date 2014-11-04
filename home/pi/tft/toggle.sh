#!/bin/sh

# switch the backlight of the 3.5" TFT.


echo pwm > /sys/class/rpi-pwm/pwm0/mode
echo 1000 > /sys/class/rpi-pwm/pwm0/frequency

DUTY=$(/bin/cat /home/pi/tft/duty) # brightness

if [ "$DUTY" = 1 ]; then
  DUTY=33
  echo "\033[32;1mduty is $DUTY\033[0m"   # GREEN
  # on 33%
  echo 33 > /sys/class/rpi-pwm/pwm0/duty
elif [ "$DUTY" = 33 ]; then
  DUTY=66
  echo "\033[32;1mduty is $DUTY\033[0m"   # GREEN
  # on 66%
  echo 66 > /sys/class/rpi-pwm/pwm0/duty
elif [ "$DUTY" = 66 ]; then
  DUTY=99
  echo "\033[32;1mduty is $DUTY\033[0m"   # GREEN
  # on 99%
  echo 99 > /sys/class/rpi-pwm/pwm0/duty
else
  DUTY=1
  echo "\033[31;1mduty is $DUTY\033[0m"   # RED
  # off
  echo 1 > /sys/class/rpi-pwm/pwm0/duty
fi;

echo $DUTY > /home/pi/tft/duty

