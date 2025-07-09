# ESP32 Thread Border Router

<aside>

💡 **프로젝트 개요**

ESP32 기반 Thread Border Router로, Thread 네트워크의 중심 역할을 합니다.  
추후 Matter 기반의 hub에 연결되어 다른 IoT 기기들에 연동될 수 있습니다.

주요 기능:
- Thread 네트워크 생성 및 관리
- 웹 인터페이스를 통한 IoT 디바이스 제어
- ESP32C6 Thread Slave로부터 받은 명령 처리, 연결된 다른 IoT 디바이스 제어
- 현재 조명, 에어컨, TV, 스피커 제어 지원

</aside>

## 1. ESP Thread Border Router - PC 연결

- ESP Thread Border Router의 **USB2** 포트에 USB C 연결

## 2. Git Clone

```bash
$ git clone --recursive https://github.com/espressif/esp-idf.git
```

- 개발 환경 스크립트 & build를 위한 기본 이미지 제공

## 3. Git checkout

```bash
$ cd esp-idf
esp-idf$ git checkout v5.2.4
esp-idf$ git submodule update --init --depth 1
esp-idf$ ./install.ps1
esp-idf$ ./export.ps1
```

- esp-idf는 esp 회사 칩 전체의 코드가 관리되기에 정확하게 checkout 진행해야 합니다
- install / export는 .sh, .bat, .ps1 제공됩니다
  - .sh: linux
  - .bat: cmd  
  - .ps1: powershell
- `.export.ps1`은 터미널을 새로 열 때마다 실행해야 합니다
- `idf.py` 명령어를 사용하여 코드를 빌드할 수 있게 해줍니다

## 4. Build Image

- 장치 관리자에서 연결한 ESP Thread Border Router가 몇번째 포트로 잡혀있는지 확인
  - 예시에서는 COM4

```bash
$ git clone https://github.com/youngjaeoh/BareMetalAI_One.git
$ cd thread_border_router/src/basic_thread_border_router
basic_thread_border_router$ idf.py -p COM4 build flash monitor
```

## 5. Configure Border Router

```bash
$ cd esp-thread-br/examples/basic_thread_border_router
$ idf.py menuconfig
```

### WiFi 설정
- `Example Connection Configuration` 진입
  - WiFi SSID 입력 (예: FASTFIVE_3F-C)
  - WiFi Password 입력

### 부팅 시 자동 Border Router 동작 설정
- `ESP Thread Border Router Example` 진입
- `Enable the automatic start mode in Thread Border Router` 설정
- `Enable the web server in Thread Border Router` 설정

## 6. Monitor device

```bash
basic_thread_border_router$ idf.py monitor

I (5595) obtr_web: <=======================server start========================>
I (5595) obtr_web: http://10.56.131.1:80/index.html
I (5595) obtr_web: <===========================================================>
```

- 위의 obtr_web 주소 접속하여 GUI 확인
  - Form network / Network Status / Network Topology 등 사용 가능
- 콘솔에서도 사용 가능

```bash
> state
leader
Done
```

## 8. 웹 인터페이스 기능

### 8.1 메인 페이지
- Thread 네트워크 상태 확인
- 연결된 디바이스 목록
- 네트워크 토폴로지 확인

### 8.2 IoT 제어 페이지
- **Lights**: 조명 제어 (`[otbr_web 주소]/lights.html`)
- **Air Conditioner**: 에어컨 제어 (`[otbr_web 주소]/airconditioner.html`)
- **TV**: TV 제어 (`[otbr_web 주소]/tv.html`)
- **Speaker**: 스피커 제어 (`[otbr_web 주소]/speaker.html`)

### 8.3 네트워크 관리
- Thread 네트워크 스캔 및 조인
- 네트워크 설정 및 구성
- 디바이스 상태 모니터링
- 실제 연결된 Thread 기기 Topology 확인

## 9. dataset 확인
```
> dataset active -x
```
- 확인된 dataset을 esp32-c6에서 dataset set active [ESP Thread Border Router의 dataset] 으로 설정

### 9.2 Thread 네트워크 명령
```
state                   # Thread 상태 확인
wifi
ipaddr
reset
...
```

## 10. 동작 방식

1. **Thread 네트워크 생성**: ESP32가 Thread Border Router로 동작하여 네트워크를 생성
2. **웹 서버 시작**: 웹 인터페이스를 통해 IoT 디바이스 제어 가능
3. **명령 수신**: ESP32C6 Thread Slave로부터 Thread Network을 통해 명령 수신
4. **상태 관리**: 각 IoT 디바이스의 상태를 전역 변수로 관리
5. **웹 업데이트**: 웹 인터페이스에서 실시간으로 상태 변경 확인 가능

## 11. 네트워크 구성

### 11.1 Thread 네트워크
- **Border Router**: ESP Thread Border Router
- **End Device**: STM + ESP32C6 Thread Slave
- **Protocol**: Thread (IEEE 802.15.4 기반)

### 11.2 웹 서버
- **URL**: monitor에서 출력된 otbr_web 주소
- **Port**: 80 (HTTP)
- **mDNS**: `esp-ot-br.local`로 자동 검색 가능

## 12. 로그 확인

모니터링 중에 다음과 같은 로그를 확인할 수 있습니다:

```
I (1234) esp_ot_br: Thread Border Router started
I (1234) esp_ot_br: Web server started on port 80
I (1234) esp_ot_br: Received command: light_on
I (1234) esp_ot_br: Light status updated: ON
```

## 13. 문제 해결

### Thread 네트워크 연결 문제
- `state` 명령어로 Thread 상태 확인
- `dataset active`로 네트워크 설정 확인

### 빌드 오류
- ESP-IDF 버전이 v5.2.4인지 확인
- `idf.py clean` 후 다시 빌드

### 웹 인터페이스 문제
- 브라우저 캐시 삭제 후 새로고침
- 다른 브라우저로 접속 시도 