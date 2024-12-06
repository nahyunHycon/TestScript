#!/bin/bash

LOG_FILE="/home/hyconsoft_rnd/Desktop/TestScript/09_LogVerification/log/systemlog.log"
echo "$(date '+%Y-%m-%d %H:%M:%S') - [11.2] Firewall_check Test started" >> "$LOG_FILE"

target_string="ALL : "  # 검색할 문자열

# 문자만 추출
extracted_chars=$(awk "/$target_string/ {gsub(/[^a-zA-Z]/, \"\", \$0); print}" /etc/hosts.deny)
#echo $extracted_chars

# 추출된 값과 기존 값을 비교
if [ $extracted_chars == "ALLALL" ]; then
  echo "0"  
  echo "$(date '+%Y-%m-%d %H:%M:%S') - [11.2] Firewall_check Test result = 0" >> "$LOG_FILE" 
else
  echo "1"  
  echo "$(date '+%Y-%m-%d %H:%M:%S') - [11.2] Firewall_check Test result = 1" >> "$LOG_FILE" 
fi

sleep 1
echo "$(date '+%Y-%m-%d %H:%M:%S') - [11.2] Firewall_check Test finished" >> "$LOG_FILE"

