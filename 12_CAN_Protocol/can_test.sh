#!/bin/bash

cansend vcan0 580#ff00000000
cansend vcan0 581#ff00000000
cansend vcan0 582#ffff0000
cansend vcan0 583#ffff000000

sleep 30
cansend vcan0 580#ff00000000
cansend vcan0 581#ff00000000
cansend vcan0 582#00000000
cansend vcan0 583#0000000000

sleep 30
cansend vcan0 580#ff00000000
cansend vcan0 581#ff00000000
cansend vcan0 582#00000000
cansend vcan0 583#0000000000
