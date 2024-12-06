#!/bin/bash

export ROS_SECURITY_KEYSTORE=~/sros2_demo/demo_keystore
export ROS_SECURITY_ENABLE=true
export ROS_SECURITY_STRATEGY=Enforce

cd /home/hyconsoft_rnd/autoware_monitoring

source install/setup.bash

ros2 run monitoring_topic_pkg monitoring_topic --ros-args --enclave /talker_listener/talker &
