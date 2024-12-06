#!/bin/bash
source ~/autoware-1/install/setup.bash

ros2 launch autoware_launch logging_simulator.launch.xml map_path:=$HOME/autoware_map/sample-map-planning vehicle_model:=sample_vehicle sensor_model:=sample_sensor_kit > /dev/null 2>&1 &

sleep 60

ros2 bag play ~/autoware-1/rosbag2_2024_10_23-16_09_36/ -r 0.7 -s sqlite3 &

echo "$(date '+%Y-%m-%d %H:%M:%S') - [2.2] Ros2_sros2 ros2 bag play" >> "$LOG_FILE"

ros2 run my_package my_node &

sleep 10

./monitoring.sh &
./pub.sh &
./sros2.sh &
