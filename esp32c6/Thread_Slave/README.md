# ESP32C6 Thread Slave

<aside>

💡 **프로젝트 개요**

STM32는 Master로, ESP32C6는 Slave로 설정되어 있습니다.

STM32에서 ESP32C6로 SPI를 통해 IoT 제어 명령을 전송하고, ESP32C6는 Thread 네트워크를 통해 Border Router로 명령을 전달합니다.

지원하는 IoT 명령:
- `light on/off` - 조명 제어
- `airconditioner on/off` - 에어컨 제어  
- `tv on/off` - TV 제어
- `speaker on/off` - 스피커 제어

</aside>

## 1. 빌드 환경 설정

```bash
$ git clone --recursive https://github.com/espressif/esp-idf.git
$ cd esp-idf
esp-idf$ git checkout v5.2.4
esp-idf$ git submodule update --init --depth 1
esp-idf$ ./install.ps1
esp-idf$ ./export.ps1
```

- `.export.ps1`은 터미널을 새로 열 때마다 실행해야 합니다
- `idf.py` 명령어를 사용하여 코드를 빌드할 수 있게 해줍니다

## 2. 빌드 타겟 설정

```bash
$ cd BareMetalAI_One\esp32c6\Thread_Slave
Thread_Slave$ idf.py set-target esp32c6
```

## 3. 프로젝트 빌드

```bash
Thread_Slave$ idf.py build
```

## 4. 펌웨어 플래시

```bash
Thread_Slave$ idf.py -p COM5 flash
```

- `COM5`는 장치 관리자에서 확인한 실제 연결된 ESP32-C6의 포트 번호입니다

## 5. 모니터링 진입 & Thread Start

```bash
Thread_Slave$ idf.py -p COM5 monitor
> datset set active [ESP Thread Border Router의 dataset]
> ifconfig up
> thread start

```

- ESP Thread Border Router에 연결합니다
- `> state` 실행 시 child 혹은 router가 출력되면 성공입니다


## 6. 하드웨어 연결

### SPI 핀 연결 (ESP32C6 ↔ STM32)

| ESP32C6 핀 | STM32 핀 | 기능 |
|------------|----------|------|
| GPIO 18 (D10) | PC12 | MOSI |
| GPIO 20 (D9) | PC11 | MISO |
| GPIO 19 (D8) | PC10 | SCLK |
| GPIO 17 (D7) | PA15 | CS |
- GND와 3.3v도 연결되어야 합니다.

## 7. 동작 방식

1. **SPI 통신**: STM32가 ESP32C6로 IoT 명령을 SPI를 통해 전송
2. **명령 처리**: ESP32C6가 받은 명령을 파싱하고 내부 상태 업데이트
3. **Thread 네트워크**: ESP32C6가 Thread 네트워크에 연결되어 있는지 확인
4. **UDP 전송**: Thread 네트워크가 준비되면 Border Router로 명령 전송

## 8. CLI 명령어 (Optional, STM이 자동 제어)

프로젝트가 실행되면 다음과 같은 CLI 명령어를 사용할 수 있습니다:

- `light_on <light_id>` - 특정 조명 켜기 (예: `light_on 1`)
- `light_off <light_id>` - 특정 조명 끄기 (예: `light_off 1`)

## 9. 로그 확인

모니터링 중에 다음과 같은 로그를 확인할 수 있습니다:

```
I (1234) ot_esp_cli: Received data: light on
I (1234) ot_esp_cli: Light turned ON
I (1234) ot_esp_cli: Current Thread role: 2
I (1234) ot_esp_cli: Thread network ready, sending light_on command
I (1234) ot_esp_cli: Light ON command sent to Border Router
```

## 10. 문제 해결

### Thread 네트워크 연결 문제
- ESP32C6가 Thread 네트워크에 제대로 연결되지 않으면 명령이 전송되지 않습니다
- `state` 명령어로 Thread 상태를 확인하세요

### SPI 통신 문제  
- 핀 연결을 다시 확인하세요
- STM32의 SPI 설정이 ESP32C6와 일치하는지 확인하세요

### 빌드 오류
- ESP-IDF 버전이 v5.2.4인지 확인하세요
- `idf.py clean` 후 다시 빌드해보세요
