/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// 전구 상태 전역 변수 선언
extern bool g_light_status;

/**
 * @brief Start border router web server, which provides REST APIs and GUI
 *
 * @param[in] base_path is the virtual file path of web server
 */
void esp_br_web_start(char *base_path);

/**
 * @brief Update light status from external sources
 *
 * @param[in] status is the new light status (true for ON, false for OFF)
 */
void update_light_status(bool status);

/**
 * @brief Handle CLI command from Thread network
 *
 * @param[in] command is the CLI command string
 */
void handle_cli_command(const char *command);

#ifdef __cplusplus
}
#endif
