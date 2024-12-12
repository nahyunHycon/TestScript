#!/bin/bash

LOG_FILE="/home/hyconsoft_rnd/Desktop/TestScript/09_LogVerification/log/systemlog.log"
echo "$(date '+%Y-%m-%d %H:%M:%S') - [13.2] Metasploit Telnet Test started" >> "$LOG_FILE"


FILE="/home/hyconsoft_rnd/Desktop/TestScript/13_Metasploit/result/output_telnet.log"

TARGET_DIR="/home/hyconsoft_rnd/Desktop/TestScript/13_Metasploit/result/"

file_count=$(find "$FILE" -type f | wc -l)


if grep -q "A network issue has occurred" "$FILE"; then
  echo "$(date '+%Y-%m-%d %H:%M:%S') - [13.2] Metasploit Telnet Test result = 0" >> "$LOG_FILE" 
  echo 0
else
  echo "$(date '+%Y-%m-%d %H:%M:%S') - [13.2] Metasploit Telnet Test result = 1" >> "$LOG_FILE" 
  echo 1
fi

echo "$(date '+%Y-%m-%d %H:%M:%S') - [13.2] Metasploit Telnet Test finished" >> "$LOG_FILE"
