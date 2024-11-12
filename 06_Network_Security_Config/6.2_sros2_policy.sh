#!/bin/bash


sudo chmod 000 /home/hyconsoft_rnd/Desktop/TestScript/06_Network_Security_Config/file/policy.xml


cp /home/hyconsoft_rnd/Desktop/TestScript/06_Network_Security_Config/file/policy.xml  /home/hyconsoft_rnd/Desktop/TestScript/06_Network_Security_Config/result/policy.xml



# 확인할 폴더와 특정 단어를 변수에 저장합니다.
TARGET_DIR="/home/hyconsoft_rnd/Desktop/TestScript/06_Network_Security_Config/result"
SEARCH_TERM="policy"

# 해당 폴더에서 특정 단어가 포함된 파일명을 찾습니다.
found=0

for file in "$TARGET_DIR"/*"$SEARCH_TERM"*; do
    if [[ -e "$file" ]]; then
        #echo "Found: $file"
        found=1
    fi
done

sudo chmod 700 /home/hyconsoft_rnd/Desktop/TestScript/06_Network_Security_Config/file/policy.xml

# 결과에 따라 exit 코드 반환
if [[ $found -eq 1 ]]; then #있으면 1
    echo "1"
    exit 1
else
    echo "0"		    #없으면 0
    exit 0
fi


