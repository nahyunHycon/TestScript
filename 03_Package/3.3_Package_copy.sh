#!/bin/bash

LOG_FILE="/home/hyconsoft_rnd/Desktop/TestScript/09_LogVerification/log/systemlog.log"
echo "$(date '+%Y-%m-%d %H:%M:%S') - [3.3] Package_copy Test started" >> "$LOG_FILE"

sudo chattr +i /home/hyconsoft_rnd/Desktop/TestScript/03_Package/File/requirements_curr.txt

mv /home/hyconsoft_rnd/Desktop/TestScript/03_Package/File/requirements_curr.txt  /home/hyconsoft_rnd/Desktop/TestScript/03_Package/result/requirements_curr.txt



# 확인할 폴더와 특정 단어를 변수에 저장합니다.
TARGET_DIR="/home/hyconsoft_rnd/Desktop/TestScript/03_Package/result"
SEARCH_TERM="requirements"

# 해당 폴더에서 특정 단어가 포함된 파일명을 찾습니다.
found=0

for file in "$TARGET_DIR"/*"$SEARCH_TERM"*; do
    if [[ -e "$file" ]]; then
        #echo "Found: $file"
        found=1
    fi
done


# 결과에 따라 exit 코드 반환
if [[ $found -eq 1 ]]; then #있으면 1
    echo "$(date '+%Y-%m-%d %H:%M:%S') - [3.3] Package_copy Test result = 0" >> "$LOG_FILE" 
    echo "1"   
else
    echo "$(date '+%Y-%m-%d %H:%M:%S') - [3.3] Package_copy Test result = 1" >> "$LOG_FILE" 
    echo "0"		    #없으면 0    
fi

sleep 1
echo "$(date '+%Y-%m-%d %H:%M:%S') - [3.3] Package_copy Test finished" >> "$LOG_FILE"