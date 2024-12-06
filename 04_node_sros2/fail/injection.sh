#!/bin/bash

cd /home/hyconsoft_rnd/autoware-1

source install/setup.bash

ros2 topic pub --times 10 /topic1 std_msgs/msg/String '{data: "Error_message"}'
