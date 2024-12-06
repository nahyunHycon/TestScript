#!/bin/bash

LOG_FILE="/home/hyconsoft_rnd/Desktop/TestScript/09_LogVerification/log/systemlog.log"
echo "$(date '+%Y-%m-%d %H:%M:%S') - [11.3] ConfigFile_check Test started" >> "$LOG_FILE"

existing_value1=/home/hyconsoft_rnd/sros2_demo/demo_keystore
existing_value2=true
existing_value3=Enforce

TARGET=/home/hyconsoft_rnd/Desktop/TestScript/11_Network_Firewall_Verification/file/config.txt

value1="ROS_SECURITY_KEYSTORE"
value2="ROS_SECURITY_ENABLE"
value3="ROS_SECURITY_STRATEGY"

extracted_value1=$(grep $value1 $TARGET | cut -d '=' -f 2)
extracted_value2=$(grep $value2 $TARGET | cut -d '=' -f 2)
extracted_value3=$(grep $value3 $TARGET | cut -d '=' -f 2)

# 추출된 값과 기존 값을 비교
if [ "$existing_value1" == "$extracted_value1" ]&&[ "$existing_value2" == "$extracted_value2" ]&&[ "$existing_value3" == "$extracted_value3" ]; then
  echo "0"  
  echo "$(date '+%Y-%m-%d %H:%M:%S') - [11.3] ConfigFile_check Test result = 0" >> "$LOG_FILE" 
else
  echo "1"  
  echo "$(date '+%Y-%m-%d %H:%M:%S') - [11.3] ConfigFile_check Test result = 1" >> "$LOG_FILE" 
fi

sleep 1
echo "$(date '+%Y-%m-%d %H:%M:%S') - [11.3] ConfigFile_check Test finished" >> "$LOG_FILE"

