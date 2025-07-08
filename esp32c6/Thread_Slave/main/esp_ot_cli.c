/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 *
 * OpenThread Command Line Example
 *
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 *
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "sdkconfig.h"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_netif_types.h"
#include "esp_openthread.h"
#include "esp_openthread_cli.h"
#include "esp_openthread_lock.h"
#include "esp_openthread_netif_glue.h"
#include "esp_openthread_types.h"
#include "esp_ot_config.h"
#include "esp_vfs_eventfd.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hal/uart_types.h"
#include "nvs_flash.h"
#include "openthread/cli.h"
#include "openthread/instance.h"
#include "openthread/logging.h"
#include "openthread/tasklet.h"
#include "driver/spi_slave.h"
#include "driver/gpio.h"
#include "esp_heap_caps.h"
#include "openthread/udp.h"
#include "openthread/ip6.h"

#if CONFIG_OPENTHREAD_STATE_INDICATOR_ENABLE
#include "ot_led_strip.h"
#endif

#if CONFIG_OPENTHREAD_CLI_ESP_EXTENSION
#include "esp_ot_cli_extension.h"
#endif // CONFIG_OPENTHREAD_CLI_ESP_EXTENSION

#if CONFIG_IDF_TARGET_ESP32C6
#define GPIO_MOSI 18       // D10 - Connected to STM32 PC12
#define GPIO_MISO 20       // D9  - Connected to STM32 PC11
#define GPIO_SCLK 19       // D8  - Connected to STM32 PC10
#define GPIO_CS 17         // D7  - Connected to STM32 PA15
#endif

#ifndef RCV_HOST
#define RCV_HOST SPI2_HOST
#endif

#define TAG "ot_esp_cli"
#define SPI_BUFFER_SIZE 64

// 전등 상태를 저장하는 전역 변수
static bool light_status = false;

#define BORDER_ROUTER_THREAD_ADDR "fd39:9080:bc00:1:9859:137a:d8d0:ffd9" // 실제 Border Router Thread IPv6 주소로 교체 필요
#define BORDER_ROUTER_UDP_PORT 80

void send_light_command(const char *cmd) {
    otInstance *instance = esp_openthread_get_instance();
    otUdpSocket socket;
    otMessageInfo messageInfo;
    memset(&messageInfo, 0, sizeof(messageInfo));

    // Border Router 주소와 포트 설정
    otIp6AddressFromString(BORDER_ROUTER_THREAD_ADDR, &messageInfo.mPeerAddr);
    messageInfo.mPeerPort = BORDER_ROUTER_UDP_PORT;

    otUdpOpen(instance, &socket, NULL, NULL);
    otMessage *msg = otUdpNewMessage(instance, NULL);
    otMessageAppend(msg, cmd, strlen(cmd));
    otUdpSend(instance, &socket, msg, &messageInfo);
    otUdpClose(instance, &socket);
}

static esp_netif_t *init_openthread_netif(const esp_openthread_platform_config_t *config)
{
    esp_netif_config_t cfg = ESP_NETIF_DEFAULT_OPENTHREAD();
    esp_netif_t *netif = esp_netif_new(&cfg);
    assert(netif != NULL);
    ESP_ERROR_CHECK(esp_netif_attach(netif, esp_openthread_netif_glue_init(config)));

    return netif;
}

static void ot_task_worker(void *aContext)
{
    esp_openthread_platform_config_t config = {
        .radio_config = ESP_OPENTHREAD_DEFAULT_RADIO_CONFIG(),
        .host_config = ESP_OPENTHREAD_DEFAULT_HOST_CONFIG(),
        .port_config = ESP_OPENTHREAD_DEFAULT_PORT_CONFIG(),
    };

    // Initialize the OpenThread stack
    ESP_ERROR_CHECK(esp_openthread_init(&config));

#if CONFIG_OPENTHREAD_STATE_INDICATOR_ENABLE
    ESP_ERROR_CHECK(esp_openthread_state_indicator_init(esp_openthread_get_instance()));
#endif

#if CONFIG_OPENTHREAD_LOG_LEVEL_DYNAMIC
    // The OpenThread log level directly matches ESP log level
    (void)otLoggingSetLevel(CONFIG_LOG_DEFAULT_LEVEL);
#endif
    // Initialize the OpenThread cli
#if CONFIG_OPENTHREAD_CLI
    esp_openthread_cli_init();
#endif

    esp_netif_t *openthread_netif;
    // Initialize the esp_netif bindings
    openthread_netif = init_openthread_netif(&config);
    esp_netif_set_default_netif(openthread_netif);

#if CONFIG_OPENTHREAD_CLI_ESP_EXTENSION
    esp_cli_custom_command_init();
#endif // CONFIG_OPENTHREAD_CLI_ESP_EXTENSION



    // Run the main loop
#if CONFIG_OPENTHREAD_CLI
    esp_openthread_cli_create_task();
#endif
#if CONFIG_OPENTHREAD_AUTO_START
    otOperationalDatasetTlvs dataset;
    otError error = otDatasetGetActiveTlvs(esp_openthread_get_instance(), &dataset);
    ESP_ERROR_CHECK(esp_openthread_auto_start((error == OT_ERROR_NONE) ? &dataset : NULL));
#endif



    esp_openthread_launch_mainloop();

    // Clean up
    esp_openthread_netif_glue_deinit();
    esp_netif_destroy(openthread_netif);

    esp_vfs_eventfd_unregister();
    vTaskDelete(NULL);
}



// SPI slave 초기화 함수 (Thread_Slave 예제 스타일)
static void init_spi_slave(void) {
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
    if (ret != ESP_OK) {
        printf("[SPI] spi_slave_initialize failed: %d\n", ret);
        return;
    }
    printf("[SPI] SPI Slave initialized. MOSI:%d, MISO:%d, SCLK:%d, CS:%d\n", GPIO_MOSI, GPIO_MISO, GPIO_SCLK, GPIO_CS);
}

// 체크섬 검증 함수
static bool check_packet_checksum(const uint8_t *buf, int header_idx, int length) {
    int packet_len = 3 + length; // 헤더+커맨드+길이+데이터
    if (header_idx + packet_len >= SPI_BUFFER_SIZE) return false;
    uint8_t sum = 0;
    for (int i = header_idx; i < header_idx + 3 + length; i++) sum += buf[i];
    uint8_t checksum = buf[header_idx + 3 + length];
    return (sum & 0xFF) == checksum;
}

// spi_slave_task는 순수 데이터 송수신만 담당 (Thread_Slave 예제 스타일)
static void spi_slave_task(void *pvParameters)
{
    esp_err_t ret;
    char *recvbuf = (char *)heap_caps_malloc(SPI_BUFFER_SIZE, MALLOC_CAP_DMA);
    char *sendbuf = (char *)heap_caps_malloc(SPI_BUFFER_SIZE, MALLOC_CAP_DMA);
    if (!recvbuf || !sendbuf) {
        ESP_LOGE(TAG, "[SPI] Failed to allocate DMA buffers");
        if (recvbuf) heap_caps_free(recvbuf);
        if (sendbuf) heap_caps_free(sendbuf);
        vTaskDelete(NULL);
        return;
    }

    printf("[SPI] Task started, waiting for data...\n");

    while (1) {
        memset(recvbuf, 0, SPI_BUFFER_SIZE);
        // sendbuf는 항상 OK로 초기화
        memset(sendbuf, 0, SPI_BUFFER_SIZE);
        memcpy(sendbuf, "OK", 3);

        spi_slave_transaction_t t;
        memset(&t, 0, sizeof(t));
        t.length = SPI_BUFFER_SIZE * 8;
        t.tx_buffer = sendbuf;
        t.rx_buffer = recvbuf;

        ret = spi_slave_transmit(RCV_HOST, &t, portMAX_DELAY);
        if (ret == ESP_OK) {
            // printf("[SPI] Received raw: ");
            // for (int i = 0; i < SPI_BUFFER_SIZE; i++) {
            //     printf("%02X ", (uint8_t)recvbuf[i]);
            // }
            // printf("\n");

            // 버퍼 내에서 헤더(0xA5) 탐색
            int header_idx = -1;
            for (int i = 0; i < SPI_BUFFER_SIZE; i++) {
                if ((uint8_t)recvbuf[i] == 0xA5) {
                    header_idx = i;
                    break;
                }
            }
            if (header_idx >= 0 && header_idx + 2 < SPI_BUFFER_SIZE) {
                uint8_t length = recvbuf[header_idx + 2];
                if (header_idx + 3 + length < SPI_BUFFER_SIZE) {
                    if (check_packet_checksum((uint8_t*)recvbuf, header_idx, length)) {
                        // printf("[SPI] Checksum OK!\n");
                        // printf("[SPI] Received ASCII: [%02X %02X %02X] ", (uint8_t)recvbuf[header_idx], (uint8_t)recvbuf[header_idx+1], (uint8_t)recvbuf[header_idx+2]);
                        // for (int i = header_idx + 3; i < header_idx + 3 + length; i++) {
                        //     char c = recvbuf[i];
                        //     printf("%c", (c >= 32 && c <= 126) ? c : '.');
                        // }
                        // printf("\n");
                        
                        // IoT 명령 처리
                        if (length > 0) {
                            char received_data[64];
                            memcpy(received_data, &recvbuf[header_idx + 3], length);
                            received_data[length] = '\0';
                            
                            ESP_LOGI(TAG, "Received data: %s", received_data);
                            
                            // "light on" 또는 "light off" 명령 처리
                            if (strstr(received_data, "light on") != NULL) {
                                light_status = true;
                                ESP_LOGI(TAG, "Light turned ON");
                                otInstance *ot_instance = esp_openthread_get_instance();
                                if (ot_instance) {
                                    otDeviceRole role = otThreadGetDeviceRole(ot_instance);
                                    ESP_LOGI(TAG, "Current Thread role: %d", role);
                                    if (role == OT_DEVICE_ROLE_CHILD || role == OT_DEVICE_ROLE_ROUTER || role == OT_DEVICE_ROLE_LEADER) {
                                        ESP_LOGI(TAG, "Thread network ready, sending light_on command via UDP");
                                        send_light_command("light_on");
                                        ESP_LOGI(TAG, "Light ON command sent to Border Router via UDP");
                                    } else {
                                        ESP_LOGW(TAG, "Thread network not ready (role: %d), command queued", role);
                                    }
                                }
                            } else if (strstr(received_data, "light off") != NULL) {
                                light_status = false;
                                ESP_LOGI(TAG, "Light turned OFF");
                                otInstance *ot_instance = esp_openthread_get_instance();
                                if (ot_instance) {
                                    otDeviceRole role = otThreadGetDeviceRole(ot_instance);
                                    ESP_LOGI(TAG, "Current Thread role: %d", role);
                                    if (role == OT_DEVICE_ROLE_CHILD || role == OT_DEVICE_ROLE_ROUTER || role == OT_DEVICE_ROLE_LEADER) {
                                        ESP_LOGI(TAG, "Thread network ready, sending light_off command via UDP");
                                        send_light_command("light_off");
                                        ESP_LOGI(TAG, "Light OFF command sent to Border Router via UDP");
                                    } else {
                                        ESP_LOGW(TAG, "Thread network not ready (role: %d), command queued", role);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } else {
            ESP_LOGE(TAG, "[SPI] spi_slave_transmit error: %d", ret);
        }
    }

    heap_caps_free(recvbuf);
    heap_caps_free(sendbuf);
    vTaskDelete(NULL);
}

void app_main(void)
{
    // Used eventfds:
    // * netif
    // * ot task queue
    // * radio driver
    esp_vfs_eventfd_config_t eventfd_config = {
        .max_fds = 3,
    };

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_vfs_eventfd_register(&eventfd_config));
    // SPI slave 반드시 먼저 초기화
    init_spi_slave();
    xTaskCreate(ot_task_worker, "ot_cli_main", 10240, xTaskGetCurrentTaskHandle(), 5, NULL);
    // SPI slave 수신 Task 생성 (우선순위 4, 스택 4096)
    xTaskCreate(spi_slave_task, "spi_slave_task", 4096, NULL, 4, NULL);
}
