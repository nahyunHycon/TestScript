import subprocess
import os

def run_dashboard():
    try:
        # Autoware 환경 설정 스크립트 경로
        #setup_script = "/home/hyconsoft_rnd/Autoware-tool-subscriber/install/setup.bash"
        # 실행하려는 프로그램 경로
        #subscriber_executable = "/home/hyconsoft_rnd/Autoware-tool-subscriber/build/autoware_subscriber/autoware_subscriber"

        # 환경 변수 설정
        #command = f"source {setup_script};{subscriber_executable}"
        command = (
            "/home/hyconsoft_rnd/Desktop/TestScript/12_CAN_Protocol/dashboard.sh &"
        ) 
        # 쉘 환경에서 명령 실행
                     # 쉘 환경에서 명령 실행
        process = subprocess.Popen(
            command,
            shell=True,
            executable="/bin/bash",
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            close_fds=True
        )    

        # 출력 및 에러 메시지 확인
        stdout, stderr = process.communicate()

        if stdout:
           print(f"출력 결과:\n{stdout.decode()}")
        if stderr:
           print(f"에러 메시지:\n{stderr.decode()}")

    except Exception as e:
        print(f"명령 실행 중 오류 발생: {e}")

if __name__ == "__main__":
    run_dashboard()