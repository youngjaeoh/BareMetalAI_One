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
#include "esp_openthread.h"
#include "openthread/thread.h"
#include "openthread/udp.h"
#include "openthread/ip6.h"
#include "freertos/queue.h"

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

#define UDP_DEST_ADDR "fd35:fae5:cbb2:1:c1df:bd10:4a1a:bc75" // Border Router 주소
#define UDP_DEST_PORT 3333
#define UDP_SRC_PORT 3333

static otInstance *ot_instance = NULL;
static QueueHandle_t spi2thread_queue;
static otUdpSocket udp_socket;

// SPI로부터 받은 명령 구조체
typedef struct {
    uint8_t data[64];
    size_t len;
} spi_cmd_t;

// Thread UDP 송신 함수 (실제 구현)
void send_udp_over_thread(const uint8_t *data, size_t len) {
    otMessageInfo msgInfo;
    memset(&msgInfo, 0, sizeof(msgInfo));
    otIp6AddressFromString(UDP_DEST_ADDR, &msgInfo.mPeerAddr);
    msgInfo.mPeerPort = UDP_DEST_PORT;
    otMessage *msg = otUdpNewMessage(ot_instance, NULL);
    if (msg && otMessageAppend(msg, data, len) == OT_ERROR_NONE) {
        otError err = otUdpSend(ot_instance, &udp_socket, msg, &msgInfo);
        if (err == OT_ERROR_NONE) {
            printf("[Thread] UDP sent to %s:%d!\n", UDP_DEST_ADDR, UDP_DEST_PORT);
        } else {
            printf("[Thread] UDP send failed! otUdpSend error: %d\n", err);
            otMessageFree(msg);
        }
    } else {
        printf("[Thread] UDP message alloc/append failed!\n");
        if (msg) otMessageFree(msg);
    }
}

void thread_task(void *arg) {
    // OpenThread 메인루프를 먼저 실행해야 함
    printf("[Thread] Starting OpenThread mainloop...\n");
    esp_openthread_launch_mainloop();
    
    // 이 코드는 실행되지 않음 (mainloop가 무한 루프)
}

void thread_init_task(void *arg) {
    // OpenThread 초기화 후 잠시 대기
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    // UDP 소켓 오픈 및 바인딩
    otSockAddr bindAddr = {0};
    bindAddr.mPort = UDP_SRC_PORT;
    otError err = otUdpOpen(ot_instance, &udp_socket, NULL, NULL);
    if (err != OT_ERROR_NONE) {
        printf("[Thread] otUdpOpen failed: %d\n", err);
        vTaskDelete(NULL);
        return;
    }
    err = otUdpBind(ot_instance, &udp_socket, &bindAddr, OT_NETIF_THREAD);
    if (err != OT_ERROR_NONE) {
        printf("[Thread] otUdpBind failed: %d\n", err);
        vTaskDelete(NULL);
        return;
    } else {
        printf("[Thread] UDP socket bound to port %d\n", UDP_SRC_PORT);
    }
    
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void spi_slave_task(void *arg) {
    esp_err_t ret;
    WORD_ALIGNED_ATTR char recvbuf[64] = {0};
    WORD_ALIGNED_ATTR char sendbuf[64] = {0};
    spi_slave_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 64 * 8;
    t.tx_buffer = sendbuf;
    t.rx_buffer = recvbuf;
    while (1) {
        memset(recvbuf, 0, 64);
        ret = spi_slave_transmit(RCV_HOST, &t, portMAX_DELAY);
        if (ret == ESP_OK) {
            // 1. 받은 데이터 UART로 16진수 출력
            printf("[SPI] Received raw: ");
            for (int i = 0; i < 64; i++) {
                printf("%02X ", (uint8_t)recvbuf[i]);
            }
            printf("\n");

            // 2. ASCII로도 출력 (가독성용)
            printf("[SPI] Received ASCII: ");
            for (int i = 0; i < 64; i++) {
                char c = recvbuf[i];
                if (c >= 32 && c <= 126) printf("%c", c);
                else printf(".");
            }
            printf("\n");

            // 3. 큐로 전달
            spi_cmd_t cmd;
            memcpy(cmd.data, recvbuf, 64);
            cmd.len = 64;
            xQueueSend(spi2thread_queue, &cmd, portMAX_DELAY);
            printf("[SPI] Forwarded to Thread task\n");
        }
    }
}

void spi_to_thread_task(void *arg) {
    spi_cmd_t cmd;
    while (1) {
        if (xQueueReceive(spi2thread_queue, &cmd, portMAX_DELAY) == pdTRUE) {
            send_udp_over_thread(cmd.data, cmd.len);
        }
    }
}

void app_main(void) {
    // 1. OpenThread 초기화
    esp_openthread_platform_config_t config = {
        .radio_config = { .radio_mode = RADIO_MODE_NATIVE },
        .host_config = { .host_connection_mode = HOST_CONNECTION_MODE_NONE },
    };
    esp_openthread_init(&config);
    ot_instance = esp_openthread_get_instance();

    // 2. SPI Slave 초기화 (pingpong 예제 코드 활용)
    spi_bus_config_t buscfg = {
        .mosi_io_num = GPIO_MOSI,
        .miso_io_num = GPIO_MISO,
        .sclk_io_num = GPIO_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };
    spi_slave_interface_config_t slvcfg = {
        .mode = 0,  // Mode 0: CPOL=0, CPHA=0
        .spics_io_num = GPIO_CS,
        .queue_size = 3,
        .flags = 0,
        .post_setup_cb = NULL,
        .post_trans_cb = NULL
    };
    gpio_set_pull_mode(GPIO_MOSI, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_SCLK, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_CS, GPIO_PULLUP_ONLY);
    esp_err_t ret = spi_slave_initialize(RCV_HOST, &buscfg, &slvcfg, SPI_DMA_CH_AUTO);
    assert(ret == ESP_OK);
    printf("SPI Slave initialized successfully\n");
    printf("MOSI: GPIO%d, MISO: GPIO%d, SCLK: GPIO%d, CS: GPIO%d\n", GPIO_MOSI, GPIO_MISO, GPIO_SCLK, GPIO_CS);
    printf("SPI Mode: 0 (CPOL=0, CPHA=0)\n");
    printf("Waiting for SPI transactions...\n");

    // 3. FreeRTOS 큐 생성 (크기 줄임)
    spi2thread_queue = xQueueCreate(2, sizeof(spi_cmd_t));

    // 4. Task 생성 (스택 크기 증가)
    xTaskCreate(thread_task, "thread_task", 8192, NULL, 5, NULL);
    xTaskCreate(thread_init_task, "thread_init_task", 8192, NULL, 5, NULL);
    xTaskCreate(spi_slave_task, "spi_slave_task", 4096, NULL, 5, NULL);
    xTaskCreate(spi_to_thread_task, "spi_to_thread_task", 4096, NULL, 5, NULL);
}
