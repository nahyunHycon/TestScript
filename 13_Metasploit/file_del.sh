#!/bin/bash


TARGET_DIR="/home/hyconsoft_rnd/Desktop/TestScript/13_Metasploit/result/"

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
