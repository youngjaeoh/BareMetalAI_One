# Bare Metal AI Project

## 1. Project Overview
STM32H743VIT6와 Seeed Studio XIAO ESP32C6 기반의 Thread IoT 및 딥러닝(Deep Learning) 프로젝트.  
현재는 LCD, SPI, UART만 남기고 불필요한 코드(카메라, 딥러닝 등)를 모두 정리한 상태이며, 앞으로 딥러닝 모델 개발과 Thread 통신 기능이 추가될 예정.

---

## 2. Hardware Setup
- **MCU**: STM32H743VIT6
- **Thread Module**: Seeed Studio XIAO ESP32C6 (SPI로 연결 예정)
- **LCD**: ST7735
- **Debug**: UART1
- **기타**: (필요시 핀맵, 회로도, 연결 사진 등 추가)

---

## 3. Project Structure
```
thread/
  ├── stm32h743/
  │   ├── Src/      # 메인 소스 코드
  │   ├── Inc/      # 헤더 파일
  │   ├── Drivers/  # HAL, BSP, LCD 드라이버 등
  │   └── ...
  └── README.md
```
- 각 폴더/파일 설명 추가
- 향후 딥러닝 모델 및 Thread 통신 관련 소스/헤더/드라이버 파일이 추가될 예정

---

## 4. Development Phase
- **현재**: LCD에 "running!!!!"만 출력, 불필요한 코드 모두 정리
- **다음 목표**: SPI 기반 ESP32C6와 Thread 통신, 딥러닝 네트워크 연동
- **추가 예정**: 자체 딥러닝 모델 개발 및 임베디드 추론 기능, Thread 네트워크 통신 구현
- **TODO**: Thread 네트워크 구성, Border Router 연동, 애플리케이션 개발, 딥러닝 모델 학습/적용 등

---

## 5. Thread Network Setup
- ESP32C6를 Thread End Device로 사용
- ESP Thread Border Router 연동 예정
- Thread 통신 프로토콜 및 메시지 송수신 기능 구현 예정
- 네트워크 구성도, 설정 방법 등(추후 추가)

---

## 6. Application Development
- **현재**: LCD에 "running!!!!"만 출력
- **향후**: 딥러닝 네트워크 아웃풋 기반 IoT 제어, SPI 통신, Thread 메시지 송수신 등
- **추가 예정**: 자체 딥러닝 모델 개발 및 임베디드 추론, Thread 통신 기반 IoT 서비스 구현

---

## 7. Quick Start
1. **빌드 환경**: STM32CubeIDE 또는 Makefile
2. **빌드 방법**:  
   - STM32CubeIDE: 프로젝트 열기 → Build → Flash  
   - Makefile: `make clean && make all`
3. **실행**:  
   - LCD에 "running!!!!" 문구가 반복 출력됨
4. **의존성**: STM32 HAL, BSP, ST7735 드라이버 등

---

## 8. 참고/문서
- [ST 공식 문서](https://www.st.com/)
- [Thread Group](https://www.threadgroup.org/)
- [Seeed Studio XIAO ESP32C6](https://wiki.seeedstudio.com/XIAO_ESP32C6/)
