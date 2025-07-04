/* SPI Slave example, receiver (uses SPI Slave driver to communicate with sender)

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "driver/spi_slave.h"
#include "driver/gpio.h"

/*
SPI receiver (slave) example.

This example is supposed to work together with the SPI sender. It uses the standard SPI pins (MISO, MOSI, SCLK, CS) to
transmit data over in a full-duplex fashion, that is, while the master puts data on the MOSI pin, the slave puts its own
data on the MISO pin.

This example uses one extra pin: GPIO_HANDSHAKE is used as a handshake pin. After a transmission has been set up and we're
ready to send/receive data, this code uses a callback to set the handshake pin high. The sender will detect this and start
sending a transaction. As soon as the transaction is done, the line gets set low again.
*/

/*
Pins in use. The SPI Master can use the GPIO mux, so feel free to change these if needed.
*/
#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
#define GPIO_HANDSHAKE 2
#define GPIO_MOSI 12
#define GPIO_MISO 13
#define GPIO_SCLK 15
#define GPIO_CS 14

#elif CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C2
#define GPIO_HANDSHAKE 3
#define GPIO_MOSI 7
#define GPIO_MISO 2
#define GPIO_SCLK 6
#define GPIO_CS 10

#elif CONFIG_IDF_TARGET_ESP32C6
#define GPIO_HANDSHAKE -1  // Not used for this test
#define GPIO_MOSI 18       // D10 - Connected to STM32 PC12
#define GPIO_MISO 20       // D9  - Connected to STM32 PC11
#define GPIO_SCLK 19       // D8  - Connected to STM32 PC10
#define GPIO_CS 17         // D7  - Connected to STM32 PA15

#elif CONFIG_IDF_TARGET_ESP32H2
#define GPIO_HANDSHAKE 2
#define GPIO_MOSI 5
#define GPIO_MISO 0
#define GPIO_SCLK 4
#define GPIO_CS 1

#elif CONFIG_IDF_TARGET_ESP32S3
#define GPIO_HANDSHAKE 2
#define GPIO_MOSI 11
#define GPIO_MISO 13
#define GPIO_SCLK 12
#define GPIO_CS 10

#endif //CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2

#ifdef CONFIG_IDF_TARGET_ESP32
#define RCV_HOST    HSPI_HOST

#else
#define RCV_HOST    SPI2_HOST

#endif

//Called after a transaction is queued and ready for pickup by master. We use this to set the handshake line high.
void my_post_setup_cb(spi_slave_transaction_t *trans)
{
    gpio_set_level(GPIO_HANDSHAKE, 1);
}

//Called after transaction is sent/received. We use this to set the handshake line low.
void my_post_trans_cb(spi_slave_transaction_t *trans)
{
    gpio_set_level(GPIO_HANDSHAKE, 0);
}

//Main application
void app_main(void)
{
    int n = 0;
    esp_err_t ret;

    //Configuration for the SPI bus
    spi_bus_config_t buscfg = {
        .mosi_io_num = GPIO_MOSI,
        .miso_io_num = GPIO_MISO,
        .sclk_io_num = GPIO_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };

    //Configuration for the SPI slave interface
    spi_slave_interface_config_t slvcfg = {
        .mode = 0,  // Mode 0: CPOL=0, CPHA=0 to match STM32's configuration
        .spics_io_num = GPIO_CS,
        .queue_size = 3,
        .flags = 0,
        .post_setup_cb = NULL,  // No handshake needed
        .post_trans_cb = NULL   // No handshake needed
    };

    //Enable pull-ups on SPI lines so we don't detect rogue pulses when no master is connected.
    gpio_set_pull_mode(GPIO_MOSI, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_SCLK, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_CS, GPIO_PULLUP_ONLY);

    //Initialize SPI slave interface
    ret = spi_slave_initialize(RCV_HOST, &buscfg, &slvcfg, SPI_DMA_CH_AUTO);
    assert(ret == ESP_OK);
    
    printf("SPI Slave initialized successfully\n");
    printf("MOSI: GPIO%d, MISO: GPIO%d, SCLK: GPIO%d, CS: GPIO%d\n", 
           GPIO_MOSI, GPIO_MISO, GPIO_SCLK, GPIO_CS);
    printf("SPI Mode: 0 (CPOL=0, CPHA=0)\n");
    printf("Waiting for SPI transactions...\n");
    
    // GPIO 레벨 초기 확인
    printf("Initial GPIO Levels - MOSI:%d SCLK:%d CS:%d\n", 
           gpio_get_level(GPIO_MOSI), gpio_get_level(GPIO_SCLK), gpio_get_level(GPIO_CS));
    
    // 간단한 GPIO 입력 테스트 (MOSI 핀 신호 감지)
    printf("Starting GPIO input test for MOSI pin...\n");
    for (int i = 0; i < 50; i++) {
        int mosi_val = gpio_get_level(GPIO_MOSI);
        if (mosi_val != 1) {
            printf("MOSI signal detected! Level: %d at iteration %d\n", mosi_val, i);
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // 100ms 대기
    }
    printf("GPIO input test completed.\n");
    
    // 핀 매핑 테스트 - 여러 GPIO 핀을 동시에 모니터링
    printf("Starting pin mapping test...\n");
    printf("Testing GPIO pins: 7, 8, 9, 10, 17, 18, 19, 20\n");
    for (int i = 0; i < 100; i++) {
        int gpio7 = gpio_get_level(7);
        int gpio8 = gpio_get_level(8);
        int gpio9 = gpio_get_level(9);
        int gpio10 = gpio_get_level(10);
        int gpio17 = gpio_get_level(17);
        int gpio18 = gpio_get_level(18);
        int gpio19 = gpio_get_level(19);
        int gpio20 = gpio_get_level(20);
        
        // 변화가 있는 핀 감지
        static int last_gpio7 = -1, last_gpio8 = -1, last_gpio9 = -1, last_gpio10 = -1;
        static int last_gpio17 = -1, last_gpio18 = -1, last_gpio19 = -1, last_gpio20 = -1;
        
        if (last_gpio7 != -1) {
            if (gpio7 != last_gpio7) printf("GPIO7 changed: %d->%d\n", last_gpio7, gpio7);
            if (gpio8 != last_gpio8) printf("GPIO8 changed: %d->%d\n", last_gpio8, gpio8);
            if (gpio9 != last_gpio9) printf("GPIO9 changed: %d->%d\n", last_gpio9, gpio9);
            if (gpio10 != last_gpio10) printf("GPIO10 changed: %d->%d\n", last_gpio10, gpio10);
            if (gpio17 != last_gpio17) printf("GPIO17 changed: %d->%d\n", last_gpio17, gpio17);
            if (gpio18 != last_gpio18) printf("GPIO18 changed: %d->%d\n", last_gpio18, gpio18);
            if (gpio19 != last_gpio19) printf("GPIO19 changed: %d->%d\n", last_gpio19, gpio19);
            if (gpio20 != last_gpio20) printf("GPIO20 changed: %d->%d\n", last_gpio20, gpio20);
        }
        
        last_gpio7 = gpio7; last_gpio8 = gpio8; last_gpio9 = gpio9; last_gpio10 = gpio10;
        last_gpio17 = gpio17; last_gpio18 = gpio18; last_gpio19 = gpio19; last_gpio20 = gpio20;
        
        vTaskDelay(pdMS_TO_TICKS(50)); // 50ms 대기
    }
    printf("Pin mapping test completed.\n");
    
    // 연결 테스트 - 실제 연결된 핀들만 모니터링
    printf("Starting connection test with updated pin mapping...\n");
    printf("Monitoring: MOSI(GPIO18), SCLK(GPIO19), CS(GPIO17)\n");
    for (int i = 0; i < 200; i++) {
        int mosi_val = gpio_get_level(GPIO_MOSI);  // GPIO18
        int sclk_val = gpio_get_level(GPIO_SCLK);  // GPIO19
        int cs_val = gpio_get_level(GPIO_CS);      // GPIO17
        
        static int last_mosi = -1, last_sclk = -1, last_cs = -1;
        
        if (last_mosi != -1) {
            if (mosi_val != last_mosi) printf("MOSI(GPIO18) changed: %d->%d\n", last_mosi, mosi_val);
            if (sclk_val != last_sclk) printf("SCLK(GPIO19) changed: %d->%d\n", last_sclk, sclk_val);
            if (cs_val != last_cs) printf("CS(GPIO17) changed: %d->%d\n", last_cs, cs_val);
        }
        
        last_mosi = mosi_val;
        last_sclk = sclk_val;
        last_cs = cs_val;
        
        vTaskDelay(pdMS_TO_TICKS(25)); // 25ms 대기 (더 빠른 감지)
    }
    printf("Connection test completed.\n");

    WORD_ALIGNED_ATTR char sendbuf[4] = "";
    WORD_ALIGNED_ATTR char recvbuf[4] = "";
    memset(recvbuf, 0, 4);
    spi_slave_transaction_t t;
    memset(&t, 0, sizeof(t));

    // ping/pong 패턴 정의
    const char ping[3] = {0xA5, 0x03, 0x00};
    const char pong[3] = {0xA5, 0x04, 0x00};
    bool pong_next = false;

    while (1) {
        memset(recvbuf, 0, 4);

        // sendbuf 설정: pong_next가 true면 pong 송신, 아니면 "OK"
        if (pong_next) {
            memcpy(sendbuf, pong, 3);
            pong_next = false;
            printf("Pong sent! 0x%02X 0x%02X 0x%02X\n", sendbuf[0], sendbuf[1], sendbuf[2]);
        } else {
            memcpy(sendbuf, "OK", 3);
        }

        t.length = 3 * 8;
        t.tx_buffer = sendbuf;
        t.rx_buffer = recvbuf;
        ret = spi_slave_transmit(RCV_HOST, &t, portMAX_DELAY);

        // ping 패킷 수신 시, 다음 트랜잭션에서 pong 송신
        if (memcmp(recvbuf, ping, 3) == 0) {
            printf("Ping received! Will send Pong next.\n");
            pong_next = true;
        }

        //spi_slave_transmit does not return until the master has done a transmission, so by here we have sent our data and
        //received data from the master. Print it.
        printf("Received %d bytes: 0x%02X 0x%02X 0x%02X\n", 3, recvbuf[0], recvbuf[1], recvbuf[2]);
        
        // Debug: Check GPIO levels every 10 transactions
        if (n % 10 == 0) {
            int mosi_level = gpio_get_level(GPIO_MOSI);
            int sclk_level = gpio_get_level(GPIO_SCLK);
            int cs_level = gpio_get_level(GPIO_CS);
            printf("GPIO Levels - MOSI:%d SCLK:%d CS:%d\n", mosi_level, sclk_level, cs_level);
            
            // 추가: GPIO 변화 감지 테스트
            static int last_mosi = -1, last_sclk = -1, last_cs = -1;
            if (last_mosi != -1 && (mosi_level != last_mosi || sclk_level != last_sclk || cs_level != last_cs)) {
                printf("*** GPIO CHANGE DETECTED! ***\n");
            }
            last_mosi = mosi_level;
            last_sclk = sclk_level;
            last_cs = cs_level;
        }
        
        n++;
    }

}
