#!/bin/bash

existing_value=192.168.0.6
# 검색할 문자열
string="sshd"

# .bashrc에서 특정 문자가 포함된 행에서 숫자만 추출하여 변수에 저장
extracted_value=$(grep "$string" /etc/hosts.deny | cut -d ':' -f 2)

# 추출된 값과 기존 값을 비교
if [ "$existing_value" == "$extracted_value" ]; then
  echo "0"  
else
  echo "1"  
fi

