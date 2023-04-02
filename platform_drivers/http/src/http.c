/**
 * @file http.c
 * @author Matheus Arcangelo Esperança
 * @brief HTTP driver
 * @version 0.1
 * @date 2023-04-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "http.h"

// cJSON library includes
#include <cJSON.h>

// definitions
#define TAG_HTTP "HTTP"
#define CONTENTLEN 255
#define LEDSTATUS_QUEUE_SIZE 1
#define UNKNOWNCMDRECEIVED "Unknown command received"
#define CMDRECEIVED "Led command received"
#define LEDSTATUSCMD "led_cmd"

// functions prototypes
httpd_handle_t start_esp32_webserver(void);
static esp_err_t get_handler(httpd_req_t *req);
static esp_err_t post_handler(httpd_req_t *req);
bool bGetLedStatus(void);

// variables initialization
static QueueHandle_t fLedStatusQueue;
static bool bLedStatus = false;

/**
 * @brief HTTP Initialization
 * 
 */
void initHTTP(void)
{
   // WebServer initialization
   start_esp32_webserver();
   fLedStatusQueue = xQueueCreate(LEDSTATUS_QUEUE_SIZE,sizeof(bool));
}

/**
 * @brief Return the led status received by the queue
 * 
 * @return true 
 * @return false 
 */
bool bGetLedStatus(void)
{
    bool led_status;
    xQueueReceive(fLedStatusQueue, &led_status, portMAX_DELAY);
    return led_status; 
}

/**
 * @brief Post requisition handler - responsible by manage the get requisitions
 * 
 * @param req 
 * @return esp_err_t 
 */
static esp_err_t get_handler(httpd_req_t *req)
{
   char *buf;
   size_t buf_len;
   buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
   if(buf_len > 1)
   {
      buf = malloc(buf_len);
      if(httpd_req_get_hdr_value_str(req, "Host", buf, buf_len) == ESP_OK)
      {
         ESP_LOGI(TAG_HTTP, "Host: %s", buf);
      }
      free(buf);
   }

   buf_len = httpd_req_get_url_query_len(req) + 1;
   if(buf_len > 1)
   {
      buf = malloc(buf_len);
      if(httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK)
      {
         ESP_LOGI(TAG_HTTP, "Query string: %s", buf);
         char param[32];
         if(httpd_query_key_value(buf, "Test", param, sizeof(param)) == ESP_OK)
         {
            ESP_LOGI(TAG_HTTP, "Test value is: %s", param);
         }
      }
      free(buf);
   }

   cJSON *json;
   char *json_str;
   char *conv_str;
   // json creating
   json = cJSON_CreateObject();
   asprintf(&conv_str, "%d", bLedStatus);
   cJSON_AddStringToObject(json, "led_status", conv_str);
   json_str = cJSON_Print(json);

   httpd_resp_send(req, json_str, HTTPD_RESP_USE_STRLEN);
   ESP_LOGI(TAG_HTTP,"Led Status Updated");

   cJSON_Delete(json); // release memory
   free(json_str); 
   free(conv_str); 

   return ESP_OK;
}

/**
 * @brief Post requisition handler - responsible by manage the post requisitions
 * 
 * @param req 
 * @return esp_err_t 
 */
static esp_err_t post_handler(httpd_req_t *req)
{
   char content[CONTENTLEN];
   memset(content,0,255);

   size_t recv_size = MIN(req->content_len, sizeof(content));

   int ret = httpd_req_recv(req, content, recv_size);

   if(ret <= 0)
   {
      if(ret == HTTPD_SOCK_ERR_TIMEOUT)
      {
         httpd_resp_send_408(req);
      }
      return ESP_FAIL;
   }
   content[recv_size] = '\0';

   // creating json objects
   cJSON *jdata;
   cJSON *jcmd;
   char resp[30];

   // parsing data to json objects
   jdata = cJSON_Parse(content);
   jcmd = cJSON_GetObjectItem(jdata, LEDSTATUSCMD);

   if(jcmd)
   {
      int led_cmd = atoi(jcmd->valuestring);
      // reading cmd from json objects
      ESP_LOGI(TAG_HTTP,"code: %d",led_cmd);

      // queue data send (led status value)
      xQueueSend(fLedStatusQueue, &led_cmd, pdMS_TO_TICKS(0));
      bLedStatus = (bool)led_cmd;
      strcpy(resp,CMDRECEIVED);
      ESP_LOGI(TAG_HTTP,"%s",resp);
   }
   else
   {
      strcpy(resp,UNKNOWNCMDRECEIVED);
      ESP_LOGE(TAG_HTTP,"%s",resp);
   }

   cJSON_Delete(jdata); // releasing memory

   httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
   return ESP_OK;
}

/**
 * @brief This function is responsible by start de webserver
 * 
 * @return httpd_handle_t 
 */
httpd_handle_t start_esp32_webserver(void)
{
   ESP_LOGI(TAG_HTTP,"start_esp32_webserver");
   httpd_config_t config = HTTPD_DEFAULT_CONFIG();
   httpd_handle_t server = NULL;

   httpd_uri_t uri_get = {
      .uri = "/",
      .method = HTTP_GET,
      .handler = get_handler,
      .user_ctx = NULL
   };

   httpd_uri_t uri_post = {
      .uri = "/",
      .method = HTTP_POST,
      .handler = post_handler,
      .user_ctx = NULL
   };

   if(httpd_start(&server, &config) == ESP_OK)
   {
      httpd_register_uri_handler(server, &uri_get);
      httpd_register_uri_handler(server, &uri_post);
   }
   return server;
}