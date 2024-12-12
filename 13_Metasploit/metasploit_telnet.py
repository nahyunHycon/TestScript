import os
import subprocess

def check_command_exists(command):
    """Check if a command exists on the system."""
    result = subprocess.run(["which", command], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return result.returncode == 0

def create_auto_rc_file(exploit_module, rhost, filename="telnet.rc"):
    """Create an automation file for msfconsole."""
    with open(filename, "w") as file:
        file.write(f"use {exploit_module}\n")
        file.write(f"set RHOST {rhost}\n")
        file.write("run\n")

def main():
    # Variables (User-defined)
    exploit_module = "auxiliary/scanner/telnet/telnet_version"
    rhost = "192.168.0.12"
    auto_rc_file = "/home/hyconsoft_rnd/Desktop/TestScript/13_Metasploit/result/telnet.rc"

    # Check if msfconsole command exists
    if not check_command_exists("msfconsole"):
        print("Metasploit(msfconsole)을 찾을 수 없습니다. 설치 여부를 확인하세요.")
        exit(1)

    # Create the automation file
    create_auto_rc_file(exploit_module, rhost, auto_rc_file)

    # Run msfconsole with the automation file
    with open("/home/hyconsoft_rnd/Desktop/TestScript/13_Metasploit/result/output_telnet.log", "w") as stdout_file, open("/home/hyconsoft_rnd/Desktop/TestScript/13_Metasploit/result/error_telnet.log", "w") as stderr_file:
        try:
            subprocess.run(["msfconsole", "-r", auto_rc_file], stdout=stdout_file, stderr=stderr_file)
        except Exception as e:
            print(f"An error occurred: {e}")
    
 
    if os.path.exists(auto_rc_file):
        os.remove(auto_rc_file)

if __name__ == "__main__":
    main()
