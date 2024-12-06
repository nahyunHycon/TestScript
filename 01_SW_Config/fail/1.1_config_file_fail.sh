#!/bin/bash

sudo chattr -i /home/hyconsoft_rnd/Desktop/TestScript/Package/File/requirements.txt
echo "1234567890" >> /home/hyconsoft_rnd/Desktop/TestScript/Package/File/requirements.txt

# 파일 경로와 예상 MD5 체크섬을 인수로 받습니다.
file_path="/home/hyconsoft_rnd/Desktop/TestScript/01_SW_Config/File/requirements.txt"
expected_md5="b364fe5b44e4c08696f73e2ebd2b80e3"


# 실제 MD5 체크섬을 계산합니다.
md5_checksum=$(md5sum "$file_path" | awk '{ print $1 }')

echo "$md5_checksum"
echo "$expected_md5"

# 예상 MD5와 비교하여 결과를 출력합니다.
if [ "$md5_checksum" == "$expected_md5" ]; then
    echo "0"
else
    echo "1"
fi

sed -i '$d' /home/hyconsoft_rnd/Desktop/TestScript/Package/File/requirements.txt
