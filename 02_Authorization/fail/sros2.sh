#!/bin/bash

export ROS_SECURITY_KEYSTORE=~/sros2_demo/demo_keystore
export ROS_SECURITY_ENABLE=true
export ROS_SECURITY_STRATEGY=Enforce

source /home/hyconsoft_rnd/autoware_node/install/setup.bash

ros2 run sros2_pkg sros2_node --ros-args --enclave /talker_listener/talker &

