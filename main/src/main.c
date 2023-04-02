/**
 * @file main.c
 * @author Matheus Arcangelo Esperanca
 * @brief HTTP with ESP32
 * @version 0.1
 * @date 2023-04-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "main.h"

/**
 * @brief Main function
 * 
 */
void app_main(void)
{
    // ESP32 system initialization
    initSystem();

    // Application initialization
    initApp();

    // main loop
    while(true){ vTaskDelay( STANDARD_WAIT_TIME / portTICK_PERIOD_MS ); }
}

/**
 * @brief Application initialization
 * 
 */
void initApp(void)
{

   // Wifi initialization
   initWifi();

   initHTTP();

   // led control task initialization
   xTaskCreate(&vLedUpdateTask, "LedUpdateTask", 1024*2, NULL, 5, NULL);
}

/**
 * @brief Led pin update task
 * 
 * @param pvParameter 
 */
static void vLedUpdateTask(void *pvParameter)
{   
   static bool led_status;
   gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
   while(true)
   {
      led_status = bGetLedStatus();
      gpio_set_level(LED_PIN, led_status);
   }
}