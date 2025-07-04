# Bare Metal AI Project

## 1. Project Overview
STM32H743VIT6 기반의 Bare Metal AI 프로젝트.  
현재는 카메라 관련 코드를 모두 정리한 상태로, **양방향 SPI 통신 시스템**, **Queue 기반 데이터 처리**, **Sleep Breathing 모니터링** 등이 추가되어 실제 AI 애플리케이션 구현 준비가 완료된 상태.

**🔧 현재 상태**: mmWave 레이더 IO PIN 사이즈 불일치로 하드웨어 테스트 대기 중

---

## 2. Hardware Setup
- **MCU**: STM32H743VIT6
- **LCD**: ST7735
- **Debug**: UART1
- **SPI**: SPI4
- **SPI3**: STM - ESP32C6 SPI Connection
- **mmWave Radar**: 연결 대기 중 (IO PIN 사이즈 불일치 해결 필요)
- **기타**: (필요시 핀맵, 회로도, 연결 사진 등 추가)

---

## 3. Project Structure
```
BareMetalAI_One/
  ├── datasheet/                  # 데이터시트 및 참고자료
  ├── esp32c6/
  │   ├── pingpong/              # ESP32C6 Master-Slave Pingpong
  │   └── Thread_Slave/          # ESP32C6 Thread End Device + Master-Slave
  ├── stm32h743/
  │   ├── Inc/                   # STM32 헤더 파일
  │   ├── Src/                   # STM32 소스 파일
  │   ├── Drivers/               # BSP, HAL, CMSIS 등 드라이버
  │   ├── TFLM/                  # TensorFlow Lite Micro
  │   ├── models/                # (딥러닝 모델 등)
  │   ├── SPI_Protocol_Documentation.md # SPI 프로토콜 문서
  │   └── ...
  ├── stm_compare/               # STM32 비교/테스트 자료
  ├── thread/                    # (Thread 네트워크 관련 자료)
  ├── Scenario.md                # 시나리오/기획 문서
  ├── README.md                  # 프로젝트 설명서
  └── LICENSE
```

---

## 4. Development Phase

### **현재 구현 완료 (2025년 7월)**
- ✅ **양방향 SPI 통신 시스템**
  - 표준화된 패킷 구조 (Header + Command + Length + Data)
  - 명령어 체계: SEND, RECEIVE, PING, PONG, STATUS
  - 에러 처리 및 타임아웃 관리
- ✅ **Queue 기반 데이터 처리 시스템**
  - 효율적인 데이터 버퍼링 및 처리
  - 실시간 데이터 스트림 관리
- ✅ **Sleep Breathing 모니터링 모듈**
  - 수면 패턴 분석 및 호흡 모니터링
  - AI 기반 건강 상태 분석 준비
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

## 6. SPI Communication Protocol

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

## 7. Hardware Integration Status

### **✅ 구현 완료**
- STM32H743VIT6 메인 보드 설정
- ST7735 LCD 디스플레이
- UART 디버깅 시스템
- SPI 통신 프로토콜

### **🔧 진행 중**
- mmWave 레이더 모듈 연결 (IO PIN 사이즈 문제 해결 중)
- 하드웨어 테스트 및 검증

### **📋 대기 중**
- 실제 센서 데이터를 이용한 AI 모델 테스트
- 장기간 모니터링 시스템 검증

---

## 8. Thread Network Setup
- ESP32C6를 Thread End Device로 사용
- ESP Thread Border Router 연동 예정
- Thread 통신 프로토콜 및 메시지 송수신 기능 구현 예정
- 네트워크 구성도, 설정 방법 등(추후 추가)

---

## 9. Application Development

### **현재 기능**
- **Queue 기반 데이터 처리**: 효율적인 실시간 데이터 스트림 관리
- **Sleep Breathing 모니터링**: 호흡 패턴 분석 및 수면 상태 감지
- **양방향 SPI 통신**: 송신/수신 상태 실시간 모니터링
- **LCD UI**: 테스트 진행 상황 및 결과 표시
- **UART 디버깅**: 상세한 통신 로그 출력

### **향후 계획**
- **Thread 통신**: ESP32C6와 Thread 네트워크 구성
- **딥러닝 추론**: TensorFlow Lite Micro 모델 연동
- **IoT 서비스**: 센서 데이터 수집 및 클라우드 연동
- **mmWave 레이더 통합**: 하드웨어 연결 후 실시간 생체신호 감지
- **AI 추론 최적화**: TensorFlow Lite Micro 모델 성능 향상
- **데이터 분석**: 장기간 수집된 데이터를 활용한 패턴 분석
- **알림 시스템**: 이상 상황 감지 시 즉각적인 알림 기능


---

## 10. Quick Start

### **빌드 환경**
- STM32CubeIDE (권장)
- STM32 HAL 드라이버
- ST7735 LCD 드라이버
- TensorFlow Lite Micro

### **빌드 방법**
```bash
# STM32CubeIDE
프로젝트 열기 → Build → Flash

# 현재 상태: 빌드 완료 ✅
# 하드웨어 테스트: mmWave 레이더 연결 대기 중 🔧
```

### **실행 결과**
```
Queue system initialized
Sleep breathing monitor ready
SPI bidirectional test OK
Sending System Info: BAREMETALAI_ONE_7000
Monitoring sleep patterns...
```

### **LCD 표시**
- **첫 번째 줄**: "BAREMETALAI_ONE"
- **두 번째 줄**: Queue 상태 및 데이터 처리
- **세 번째 줄**: Sleep breathing 모니터링 상태
- **네 번째 줄**: 시스템 상태 및 카운터

---

## 11. 참고/문서
- [ST 공식 문서](https://www.st.com/)
- [TensorFlow Lite Micro](https://www.tensorflow.org/lite/microcontrollers)
- [STM32H743VIT6 Reference Manual](https://www.st.com/resource/en/reference_manual/rm0433-stm32h742-stm32h743753-and-stm32h750-value-line-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
- [STM32 HAL SPI](https://www.st.com/resource/en/user_manual/dm00105879-description-of-stm32f4-hal-and-ll-drivers-stmicroelectronics.pdf)

---

## 🚀 Recent Updates (2025.07.02)
- ✅ Queue 시스템 구현 완료
- ✅ Sleep breathing 모니터링 모듈 추가
- ✅ STM32CubeIDE 빌드 검증 완료
- 🔧 mmWave 레이더 하드웨어 연결 이슈 해결 중
- 📋 하드웨어 테스트 및 AI 모델 통합 대기
