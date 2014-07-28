#!/bin/sh
#
# 28.07.2014 - AKR
#
# This script is executed from ~/.config/autostart/gpstrace.desktop
# to launch the gpstrace when the LXDE desktop is started.
# An alternative is to launch gpstrace via /etc/init.d/gpstrace


/usr/bin/lxterminal --geometry=60x19 --command /home/pi/gps/gpstrace
