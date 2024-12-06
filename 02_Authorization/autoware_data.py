import subprocess
import os

def run_autoware_launch():
    try:
        # 명령어를 실행할 셸 스크립트 작성
        command = (
            "source /home/hyconsoft_rnd/autoware-1/install/setup.bash;ros2 run my_package my_node &"
        )

        # 쉘 환경에서 명령 실행
        process = subprocess.Popen(
            command,
            shell=True,
            executable="/bin/bash",
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            close_fds=True
        )

    except Exception as e:
        print(f"명령 실행 중 오류 발생: {e}")

if __name__ == "__main__":
    run_autoware_launch()