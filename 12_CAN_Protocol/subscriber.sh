#!/bin/bash

cd ~/ICSimulator-and-QtDashBoard/build/Dashboard
./dashboard &

cd ~/Autoware-tool-subscriber
source install/setup.bash

cd ~/Autoware-tool-subscriber/build/autoware_subscriber
./autoware_subscriber &

sleep 20

cansend vcan0 580#ffff000000 
cansend vcan0 581#ff00000000
cansend vcan0 582#ffff0000              
cansend vcan0 583#ffff000000

sleep 3

cansend vcan0 580#0000000000
cansend vcan0 581#0000000000
cansend vcan0 582#00000000
cansend vcan0 583#0000000000


#sleep 3
#
#cansend vcan0 580#ffff000000
#cansend vcan0 581#ff00000000
#cansend vcan0 582#ffff0000
#cansend vcan0 583#ffff000000
#
#sleep 3
#
#cansend vcan0 580#0000000000
#cansend vcan0 581#0000000000
#cansend vcan0 582#00000000
#cansend vcan0 583#0000000000
#
#sleep 3
#
#cansend vcan0 580#ffff000000 
#cansend vcan0 581#ff00000000
#cansend vcan0 582#ffff0000
#cansend vcan0 583#ffff000000

