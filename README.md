# Bare Metal AI Project

## 1. Project Overview
STM32H743VIT6와 Seeed Studio XIAO ESP32C6 기반의 Thread IoT 및 딥러닝(Deep Learning) 프로젝트.  
현재는 LCD, SPI, UART만 남기고 불필요한 코드(카메라, 딥러닝 등)를 모두 정리한 상태이며, **양방향 SPI 통신 시스템**이 구현되어 ESP32C6와의 연동 준비가 완료된 상태.

---

## 2. Hardware Setup
- **MCU**: STM32H743VIT6
- **Thread Module**: Seeed Studio XIAO ESP32C6 (SPI로 연결 예정)
- **LCD**: ST7735
- **Debug**: UART1
- **SPI**: SPI4
- **기타**: (필요시 핀맵, 회로도, 연결 사진 등 추가)

---

## 3. Project Structure
```
thread/
  ├── stm32h743/
  │   ├── Src/
  │   │   ├── main.cpp              # 메인 애플리케이션
  │   │   ├── thread_spi.c          # 양방향 SPI 통신 프로토콜
  │   │   ├── lcd.c                 # LCD 드라이버
  │   │   └── ...
  │   ├── Inc/
  │   │   ├── thread_spi.h          # SPI 통신 프로토콜 정의
  │   │   ├── lcd.h                 # LCD 헤더
  │   │   └── ...
  │   ├── Drivers/
  │   │   ├── BSP/ST7735/           # ST7735 LCD 드라이버
  │   │   ├── STM32H7xx_HAL_Driver/ # STM32 HAL 드라이버
  │   │   └── ...
  │   └── ...
  └── README.md
```

---

## 4. Development Phase

### **현재 구현 완료 (2024년)**
- ✅ **양방향 SPI 통신 시스템**
  - 표준화된 패킷 구조 (Header + Command + Length + Data)
  - 명령어 체계: SEND, RECEIVE, PING, PONG, STATUS
  - 에러 처리 및 타임아웃 관리
- ✅ **LCD UI 시스템**
  - 실시간 테스트 진행 상황 표시
  - 깔끔한 화면 업데이트 (clearLine 함수)
  - 송신/수신 상태 모니터링
- ✅ **UART 디버깅 시스템**
  - 최적화된 UART 출력 (UART_Send_String 기반)
  - 시스템 정보, 상태 정보, Ping 실시간 출력
  - 수신 데이터 처리 및 표시

### **다음 목표**
- **ESP32C6 연동**: 실제 Thread End Device와 양방향 통신 테스트
- **Thread 네트워크**: Thread 프로토콜 구현 및 Border Router 연동
- **딥러닝 모델**: TensorFlow Lite Micro 기반 추론 엔진 연동

---

## 5. SPI Communication Protocol

### **패킷 구조**
```c
typedef struct {
    uint8_t header;      // 0x80 (고정)
    uint8_t command;     // 명령어 (SEND, RECEIVE, PING, PONG, STATUS)
    uint8_t length;      // 데이터 길이 (0-32바이트)
    uint8_t data[32];    // 데이터 (최대 32바이트)
} Thread_SPI_Packet_t;
```

### **명령어 체계**
- `THREAD_SPI_CMD_SEND (0x01)`: 데이터 전송
- `THREAD_SPI_CMD_RECEIVE (0x02)`: 데이터 수신
- `THREAD_SPI_CMD_PING (0x03)`: 연결 확인
- `THREAD_SPI_CMD_PONG (0x04)`: Ping 응답
- `THREAD_SPI_CMD_STATUS (0x05)`: 상태 정보

### **시스템 정보 구조**
```c
typedef struct {
    uint32_t uptime;     // 시스템 업타임 (ms)
    uint8_t status;      // 상태 플래그
    uint16_t free_memory; // 사용 가능한 메모리 (KB)
    float cpu_temp;      // CPU 온도 (시뮬레이션)
} Thread_Status_Info_t;
```

---

## 6. Thread Network Setup
- ESP32C6를 Thread End Device로 사용
- ESP Thread Border Router 연동 예정
- Thread 통신 프로토콜 및 메시지 송수신 기능 구현 예정
- 네트워크 구성도, 설정 방법 등(추후 추가)

---

## 7. Application Development

### **현재 기능**
- **실시간 시스템 모니터링 (시뮬레이션)**: 업타임, 온도, 메모리 상태를 UART에 표시
- **양방향 SPI 통신**: 송신/수신 상태 실시간 모니터링
- **LCD UI**: 테스트 진행 상황 및 결과 표시
- **UART 디버깅**: 상세한 통신 로그 출력

### **향후 계획**
- **Thread 통신**: ESP32C6와 Thread 네트워크 구성
- **딥러닝 추론**: TensorFlow Lite Micro 모델 연동
- **IoT 서비스**: 센서 데이터 수집 및 클라우드 연동

---

## 8. Quick Start

### **빌드 환경**
- STM32CubeIDE 또는 Makefile
- STM32 HAL 드라이버
- ST7735 LCD 드라이버

### **빌드 방법**
```bash
# STM32CubeIDE
프로젝트 열기 → Build → Flash

# Makefile
make clean && make all
```

### **실행 결과**
```
SPI bidirectional test OK
Sending System Info: STM32H743_7000
Sending Status - Uptime: 7000 ms, Temp: 24.2C, Memory: 512 KB
Sending PING
```

### **LCD 표시**
- **첫 번째 줄**: "SPI Bidirectional"
- **두 번째 줄**: 송신/수신 상태
- **세 번째 줄**: 수신 데이터 상태
- **네 번째 줄**: 테스트 카운터

---

## 9. 참고/문서
- [ST 공식 문서](https://www.st.com/)
- [Thread Group](https://www.threadgroup.org/)
- [Seeed Studio XIAO ESP32C6](https://wiki.seeedstudio.com/XIAO_ESP32C6/)
- [STM32 HAL SPI](https://www.st.com/resource/en/user_manual/dm00105879-description-of-stm32f4-hal-and-ll-drivers-stmicroelectronics.pdf)
