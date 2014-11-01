#!/bin/sh
#
# 28.07.2014 - AKR
#
# This script is executed from ~/.config/autostart/gpstrace.desktop
# to launch the gpstrace when the LXDE desktop is started.
# An alternative is to launch gpstrace via /etc/init.d/gpstrace


# 2.8" display with terminal monospace 7 font
#/usr/bin/lxterminal --geometry=60x19 --command /home/pi/gps/gpstrace
# 3.5" display with terminal monospace 8 font
/usr/bin/lxterminal --geometry=66x19 --command /home/pi/gps/gpstrace

