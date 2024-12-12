#!/bin/bash

LOG_FILE="/home/hyconsoft_rnd/Desktop/TestScript/09_LogVerification/log/systemlog.log"
echo "$(date '+%Y-%m-%d %H:%M:%S') - [13.1] Metasploit TCP Test started" >> "$LOG_FILE"


FILE="/home/hyconsoft_rnd/Desktop/TestScript/13_Metasploit/result/output_tcp.log"

TARGET_DIR="/home/hyconsoft_rnd/Desktop/TestScript/13_Metasploit/result/"

file_count=$(find "$FILE" -type f | wc -l)


if grep -q "TCP OPEN" "$FILE"; then
  echo "$(date '+%Y-%m-%d %H:%M:%S') - [13.1] Metasploit TCP Test result = 0" >> "$LOG_FILE" 
  echo 0
else
  echo "$(date '+%Y-%m-%d %H:%M:%S') - [13.1] Metasploit TCP Test result = 1" >> "$LOG_FILE" 
  echo 1
fi

echo "$(date '+%Y-%m-%d %H:%M:%S') - [13.1] Metasploit TCP Test finished" >> "$LOG_FILE"
