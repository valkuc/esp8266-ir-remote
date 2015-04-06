# esp8266-ir-remote
Infrared remote control library for ESP8266 RTOS SDK

The library itself consist of next files:

 - include/ir_remote.h
 - include/ir_remote_def.h
 - user/ir_remote.c

It use FRC1 timer interrupt to create precise PWM signal with specified frequency.

IR code defines are based on https://github.com/shirriff/Arduino-IRremote by Ken Shirriff
Makefile used from CHERTS ESP8266-Dev-Kit: https://github.com/CHERTS/esp8266-devkit

The library is distributed as an example application that can be flashed to ESP8266:

 - modify Makefile according to your ESP8266 RTOS SDK environment
 - change serial port
 - run "make"
 - run "make flash"
