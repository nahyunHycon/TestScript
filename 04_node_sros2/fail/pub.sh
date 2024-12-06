#!/bin/bash

export ROS_SECURITY_KEYSTORE=~/sros2_demo/demo_keystore
export ROS_SECURITY_ENABLE=true
export ROS_SECURITY_STRATEGY=Enforce

cd /home/hyconsoft_rnd/autoware_node/autoware_pubsub

source install/setup.bash

ros2 run autoware_pubsub pub --ros-args --enclave /talker_listener/talker

