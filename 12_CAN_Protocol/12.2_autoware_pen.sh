#!/bin/bash

LOG_FILE="/home/hyconsoft_rnd/Desktop/TestScript/09_LogVerification/systemlog.log"
echo "$(date '+%Y-%m-%d %H:%M:%S') - [12.2] CAN_Pen Test started" >> "$LOG_FILE"

bash_file_path="/home/hyconsoft_rnd/Desktop/TestScript/12_CAN_Protocol/start_up.sh"

# bash 파일을 실행합니다.
bash "$bash_file_path"

cd ~/ICSimulator-and-QtDashBoard/build/Dashboard
./dashboard &


cansend vcan0 580#ffff000000

if [ "$extracted_value" == "$EXISTING_VALUE" ]; then
  echo "$(date '+%Y-%m-%d %H:%M:%S') - [12.2] CAN_Pen Test result = 0" >> "$LOG_FILE" 
  echo 0
else
  echo "$(date '+%Y-%m-%d %H:%M:%S') - [12.2] CAN_Pen Test result = 1" >> "$LOG_FILE" 
  echo 1
fi

echo "$(date '+%Y-%m-%d %H:%M:%S') - [12.2] CAN_Pen Test finished" >> "$LOG_FILE"
