#!/bin/bash

# 확인할 폴더와 특정 단어를 변수에 저장합니다.
TARGET_DIR="/home/hyconsoft_rnd/Desktop/TestScript/CAN_Protocol/result"
SEARCH_TERM="Detect_"

# 해당 폴더에서 특정 단어가 포함된 파일명을 찾습니다.
found=0

for file in "$TARGET_DIR"/*"$SEARCH_TERM"*; do
    if [[ -e "$file" ]]; then
        #echo "Found: $file"
        found=1
    fi
done

# 결과에 따라 exit 코드 반환
if [[ $found -eq 1 ]]; then #있으면 0
    echo "0"
    exit 0
else
    echo "1"		    #없으면 1	
    exit 1
fi
