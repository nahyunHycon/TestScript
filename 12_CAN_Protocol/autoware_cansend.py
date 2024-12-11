import can
import time

def send_can_messages():
    # CAN 인터페이스 설정
    bus = can.interface.Bus(channel='vcan0', bustype='socketcan')

    # 송신할 CAN 메시지 정의
    messages = [
        #can.Message(arbitration_id=0x580, data=[0xFF, 0x00, 0x00, 0x00, 0x00], is_extended_id=False),
        can.Message(arbitration_id=0x581, data=[0xFF, 0x00, 0x00, 0x00, 0x00], is_extended_id=False),
        can.Message(arbitration_id=0x582, data=[0xFF, 0xFF, 0x00, 0x00], is_extended_id=False),
        can.Message(arbitration_id=0x583, data=[0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00], is_extended_id=False),
        #can.Message(arbitration_id=0x580, data=[0x00, 0x00, 0x00, 0x00, 0x00], is_extended_id=False),
        can.Message(arbitration_id=0x581, data=[0x00, 0x00, 0x00, 0x00, 0x00], is_extended_id=False),
        can.Message(arbitration_id=0x582, data=[0x00, 0x00, 0x00, 0x00], is_extended_id=False),
        can.Message(arbitration_id=0x583, data=[0x00, 0x00, 0x00, 0x00, 0x00], is_extended_id=False),
    ]

    try:
        while True:
            for msg in messages:
                bus.send(msg)
                #print(f"Sent: {msg}")
                time.sleep(0.1)  # 메시지 사이에 100ms 대기
    except KeyboardInterrupt:
        print("Stopped by user")
    finally:
        bus.shutdown()

if __name__ == "__main__":
    send_can_messages()