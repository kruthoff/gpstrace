gpstrace
========

28.07.2014 - AKR - Initial version.


This code can be used to create gps traces on a Raspberry Pi without network connection, but with a GPS receiver using UART. Using it as a mobile device (i.e. in a car), a small TFT display was attached to see what's happening.


Files
-----
<pre>
.
├── etc
│   ├── default
│   │   ├── console-setup              defines a small console font
│   │   ├── gpstrace                   flags for gpstrace script
│   │   └── tftbuttons                 flags for tftbuttons script
│   ├── init.d
│   │   ├── gpstrace                   start/stop script
│   │   └── tftbuttons                 start/stop script
│   ├── kbd
│   │   └── config                     disable console screensaver
│   ├── ntp.conf                       get time from gpsd
│   └── rc.local                       start rpi_gpio_ntp
├── home
│   └── pi
│       ├── .config
│       │   └── autostart
│       │       └── gpstrace.desktop   start gpstrace when desktop starts
│       ├── gps
│       │   ├── gpstime.py             set initial gps time to OS 
│       │   ├── gpstrace               main trace script
│       │   ├── id                     persisted unique trace filename prefix
│       │   ├── startgpstrace.sh       start gpstrace in terminal when desktop starts
│       │   └── traces                 traces are stored here
│       └── tft
│           └── tftbuttons             handle tft buttons, i.e. switch backlight
├── LICENSE
├── README.md
└── usr
    └── local
        ├── bin
        │   └── rpi_gpio_ntp           program to set the initial time
        └── src
            └── rpi_gpio_ntp-0.3       src code
</pre>


References
----------

* https://learn.adafruit.com/adafruit-ultimate-gps-on-the-raspberry-pi/
* https://learn.adafruit.com/adafruit-nfc-rfid-on-raspberry-pi/freeing-uart-on-the-pi/
* https://learn.adafruit.com/adafruit-pitft-28-inch-resistive-touchscreen-display-raspberry-pi/
* http://code.google.com/p/gpstime/
* http://vanheusden.com/time/rpi_gpio_ntp/
* http://www.raspberrypi-spy.co.uk/2014/04/how-to-change-the-command-line-font-size/

