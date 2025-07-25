/*
 * SPDX-FileCopyrightText: 2021-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_ot_cli_extension.h"
#include "esp_openthread.h"
#include "esp_ot_br_lib_compati_check.h"
#include "esp_ot_curl.h"
#include "esp_ot_dns64.h"
#include "esp_ot_heap_diag.h"
#include "esp_ot_ip.h"
#include "esp_ot_iperf.h"
#include "esp_ot_loglevel.h"
#include "esp_ot_nvs_diag.h"
#include "esp_ot_ota_commands.h"
#include "esp_ot_rcp_commands.h"
#include "esp_ot_tcp_socket.h"
#include "esp_ot_udp_socket.h"
#include "esp_ot_wifi_cmd.h"
#include "esp_br_web.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "openthread/cli.h"

// Light control command handlers
static otError esp_openthread_process_light_on(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    ESP_LOGI("CLI", "Light ON command received");
    update_light_status(true);
    return OT_ERROR_NONE;
}

static otError esp_openthread_process_light_off(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    ESP_LOGI("CLI", "Light OFF command received");
    update_light_status(false);
    return OT_ERROR_NONE;
}

// Air Conditioner control command handlers
static otError esp_openthread_process_ac_on(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    ESP_LOGI("CLI", "Air Conditioner ON command received");
    update_ac_status(true);
    return OT_ERROR_NONE;
}

static otError esp_openthread_process_ac_off(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    ESP_LOGI("CLI", "Air Conditioner OFF command received");
    update_ac_status(false);
    return OT_ERROR_NONE;
}

// TV control command handlers
static otError esp_openthread_process_tv_on(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    ESP_LOGI("CLI", "TV ON command received");
    update_tv_status(true);
    return OT_ERROR_NONE;
}

static otError esp_openthread_process_tv_off(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    ESP_LOGI("CLI", "TV OFF command received");
    update_tv_status(false);
    return OT_ERROR_NONE;
}

// Speaker control command handlers
static otError esp_openthread_process_speaker_on(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    ESP_LOGI("CLI", "Speaker ON command received");
    update_speaker_status(true);
    return OT_ERROR_NONE;
}

static otError esp_openthread_process_speaker_off(void *aContext, uint8_t aArgsLength, char *aArgs[])
{
    ESP_LOGI("CLI", "Speaker OFF command received");
    update_speaker_status(false);
    return OT_ERROR_NONE;
}

static const otCliCommand kCommands[] = {
    {"curl", esp_openthread_process_curl},
#if CONFIG_OPENTHREAD_DNS64_CLIENT
    {"dns64server", esp_openthread_process_dns64_server},
#endif // CONFIG_OPENTHREAD_DNS64_CLIENT
    {"heapdiag", esp_ot_process_heap_diag},
    {"ip", esp_ot_process_ip},
    {"iperf", esp_ot_process_iperf},
    {"loglevel", esp_ot_process_logset},
    {"mcast", esp_ot_process_mcast_group},
#if CONFIG_OPENTHREAD_NVS_DIAG
    {"nvsdiag", esp_ot_process_nvs_diag},
#endif // CONFIG_OPENTHREAD_NVS_DIAG
#if CONFIG_OPENTHREAD_CLI_OTA
    {"ota", esp_openthread_process_ota_command},
#endif // CONFIG_OPENTHREAD_CLI_OTA
#if CONFIG_OPENTHREAD_RCP_COMMAND
    {"otrcp", esp_openthread_process_rcp_command},
#endif // CONFIG_OPENTHREAD_RCP_COMMAND
    {"tcpsockclient", esp_ot_process_tcp_client},
    {"tcpsockserver", esp_ot_process_tcp_server},
    {"udpsockclient", esp_ot_process_udp_client},
    {"udpsockserver", esp_ot_process_udp_server},
#if CONFIG_OPENTHREAD_CLI_WIFI
    {"wifi", esp_ot_process_wifi_cmd},
#endif // CONFIG_OPENTHREAD_CLI_WIFI
#if CONFIG_OPENTHREAD_BR_LIB_CHECK
    {"brlibcheck", esp_openthread_process_br_lib_compatibility_check},
#endif // CONFIG_OPENTHREAD_CLI_WIFI
    {"light_on", esp_openthread_process_light_on},
    {"light_off", esp_openthread_process_light_off},
    {"ac_on", esp_openthread_process_ac_on},
    {"ac_off", esp_openthread_process_ac_off},
    {"tv_on", esp_openthread_process_tv_on},
    {"tv_off", esp_openthread_process_tv_off},
    {"speaker_on", esp_openthread_process_speaker_on},
    {"speaker_off", esp_openthread_process_speaker_off},
};

void esp_cli_custom_command_init()
{
    esp_ot_heap_diag_init();
    otInstance *instance = esp_openthread_get_instance();
    otCliSetUserCommands(kCommands, (sizeof(kCommands) / sizeof(kCommands[0])), instance);
}
