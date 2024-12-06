#!/bin/bash

LOG_FILE="/home/hyconsoft_rnd/Desktop/TestScript/09_LogVerification/log/systemlog.log"
echo "$(date '+%Y-%m-%d %H:%M:%S') - [2.2] Ros2_sros2 Test started" >> "$LOG_FILE"

EXISTING_VALUE="13"

# 검색할 문자열
string="ROS_DOMAIN_ID"

# .bashrc에서 특정 문자가 포함된 행에서 숫자만 추출하여 변수에 저장
extracted_value=$(grep "$string" /home/hyconsoft_rnd/.bashrc | grep -o '[0-9]\+')

# 추출된 값과 기존 값을 비교
if [ "$extracted_value" == "$EXISTING_VALUE" ]; then
  echo "$(date '+%Y-%m-%d %H:%M:%S') - [2.2] Ros2_sros2 Test result = 0" >> "$LOG_FILE" 
  echo 0
else
  echo "$(date '+%Y-%m-%d %H:%M:%S') - [2.2] Ros2_sros2 Test result = 1" >> "$LOG_FILE" 
  echo 1
fi

sleep 1
echo "$(date '+%Y-%m-%d %H:%M:%S') - [2.2] Ros2_sros2 Test finished" >> "$LOG_FILE"
