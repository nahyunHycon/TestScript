#!/bin/bash

LOG_FILE="/home/hyconsoft_rnd/Desktop/TestScript/09_LogVerification/log/systemlog.log"
echo "$(date '+%Y-%m-%d %H:%M:%S') - [11.1] SSHD_check Test started" >> "$LOG_FILE"

existing_value=192.168.0.6
# 검색할 문자열
string="sshd"

ip_pattern="192\.168\.0\.[0-9]+"

# .bashrc에서 특정 문자가 포함된 행에서 숫자만 추출하여 변수에 저장
extracted_value=$(grep "$string" /etc/hosts.allow | awk -v pattern="$ip_pattern" '{if ($0 ~ pattern) print $0}' | grep -oE "$ip_pattern")

# 추출된 값과 기존 값을 비교
if [ "$existing_value" == "$extracted_value" ]; then
  echo "$(date '+%Y-%m-%d %H:%M:%S') - [11.1] SSHD_check Test result = 0" >> "$LOG_FILE" 
  echo "0"  
else
  echo "$(date '+%Y-%m-%d %H:%M:%S') - [11.1] SSHD_check Test result = 1" >> "$LOG_FILE" 
  echo "1"  
fi

sleep 1
echo "$(date '+%Y-%m-%d %H:%M:%S') - [11.1] SSHD_check Test finished" >> "$LOG_FILE"
