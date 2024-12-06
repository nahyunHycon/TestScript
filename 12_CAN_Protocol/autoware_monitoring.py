import subprocess
import os


def set_environment_variables():
    # 설정할 환경 변수
    os.environ["ROS_SECURITY_KEYSTORE"] = os.path.expanduser("/home/hyconsoft_rnd/sros2_demo/demo_keystore")
    os.environ["ROS_SECURITY_ENABLE"] = "true"
    os.environ["ROS_SECURITY_STRATEGY"] = "Enforce"

def run_autoware_monitoring():
    try:
        # 명령어를 실행할 셸 스크립트 작성
        command = (
            "source /home/hyconsoft_rnd/autoware_monitoring/install/setup.bash;ros2 run monitoring_topic_pkg monitoring_topic --ros-args --enclave /talker_listener/talker &"
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
    set_environment_variables()
    run_autoware_monitoring()








