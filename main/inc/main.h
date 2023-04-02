/**
 * @file main.h
 * @author Matheus Arcangelo Esperanca
 * @brief HTTP with ESP32
 * @version 0.1
 * @date 2023-04-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef MAIN_INC_MAIN_H_
#define MAIN_INC_MAIN_H_

#include <stdio.h>
#include <sys/param.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"

#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_http_server.h"
#include "nvs_flash.h"

#include "wifi.h"
#include "system.h"
#include "http.h"

#include <stdio.h>
#include <sys/param.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"

#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_http_server.h"
#include "nvs_flash.h"

#include "wifi.h"
#include "system.h"
#include "http.h"

#define TAG_MAIN "MAIN"
#define STANDARD_WAIT_TIME 1000
#define LED_PIN   19

// Function prototypes
void initApp(void);
static void vLedUpdateTask(void *pvParameter);
extern bool bGetLedStatus(void);

#endif