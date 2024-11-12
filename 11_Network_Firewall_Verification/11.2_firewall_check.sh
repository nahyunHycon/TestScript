#!/bin/bash

existing_value="활성"

# .bashrc에서 특정 문자가 포함된 행에서 숫자만 추출하여 변수에 저장
extracted_value=$(sudo sudo ufw status | cut -d ':' -f 2)
echo $extracted_value
echo $existing_value

# 추출된 값과 기존 값을 비교
if [ $extracted_value == $existing_value ]; then
  echo "0"  
else
  echo "1"  
fi

