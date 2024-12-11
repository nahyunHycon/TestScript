#!/bin/bash

LOG_FILE="/home/hyconsoft_rnd/Desktop/TestScript/09_LogVerification/log/systemlog.log"
echo "$(date '+%Y-%m-%d %H:%M:%S') - [12.1] CAN_Pen Test started" >> "$LOG_FILE"


dir="/home/hyconsoft_rnd/Desktop/TestScript/12_CAN_Protocol/result/"

file_count=$(find "$dir" -type f | wc -l)
extracted_value="4"

if [ "$extracted_value" == "$file_count" ]; then
  echo "$(date '+%Y-%m-%d %H:%M:%S') - [12.1] CAN_Pen Test result = 0" >> "$LOG_FILE" 
  echo 0
else
  echo "$(date '+%Y-%m-%d %H:%M:%S') - [12.1] CAN_Pen Test result = 1" >> "$LOG_FILE" 
  echo 1
fi

echo "$(date '+%Y-%m-%d %H:%M:%S') - [12.1] CAN_Pen Test finished" >> "$LOG_FILE"


TARGET_DIR="/home/hyconsoft_rnd/Desktop/TestScript/12_CAN_Protocol/result/"

# 디렉토리가 존재하는지 확인
if [ -d "$TARGET_DIR" ]; then
    #echo "Deleting all files in $TARGET_DIR..."
    # 디렉토리 내 모든 파일 삭제
    find "$TARGET_DIR" -type f -delete
    #echo "All files have been deleted."
else
    #echo "Error: Directory $TARGET_DIR does not exist."
    exit 1
fi
