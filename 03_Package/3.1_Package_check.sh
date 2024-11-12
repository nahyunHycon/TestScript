#!/bin/bash

pip freeze > /home/hyconsoft_rnd/Desktop/TestScript/03_Package/File/requirements_curr.txt

file_path="/home/hyconsoft_rnd/Desktop/TestScript/03_Package/File/requirements_curr.txt"

#기존 Requirements.txt의 해시값
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


