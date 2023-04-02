/**
 * @file http.h
 * @author Matheus Arcangelo Esperança
 * @brief HTTP driver
 * @version 0.1
 * @date 2023-04-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef PLATFORM_DRIVERS_HTTP_INC_HTTP_H_
#define PLATFORM_DRIVERS_HTTP_INC_HTTP_H_

#include <stdio.h>
#include <sys/param.h>

#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_http_server.h"
#include "nvs_flash.h"

void initHTTP(void);

#endif