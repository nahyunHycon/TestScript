#!/bin/bash


bash_file_path="/home/hyconsoft_rnd/Desktop/TestScript/CAN_Interface/start_up.sh"

# bash 파일을 실행합니다.
bash "$bash_file_path"

cd ~/ICSimulator-and-QtDashBoard/build/Dashboard
./dashboard &

sleep 10

cd ~/Autoware-tool-subscriber
source install/setup.bash

cd ~/Autoware-tool-subscriber/build/autoware_subscriber
./autoware_subscriber &

sleep 10

source ~/autoware-1/install/setup.bash

ros2 launch autoware_launch logging_simulator.launch.xml map_path:=$HOME/autoware_map/sample-map-planning vehicle_model:=sample_vehicle sensor_model:=sample_sensor_kit &

sleep 60

ros2 bag play ~/autoware-1/rosbag2_2024_10_23-16_09_36/ -r 0.7 -s sqlite3 &

sleep 30

cansend vcan0 580#ffff000000

sleep 180

bash_file_path="/home/hyconsoft_rnd/Desktop/TestScript/CAN_Protocol/autoware_shutdown.sh"
bash "$bash_file_path"

