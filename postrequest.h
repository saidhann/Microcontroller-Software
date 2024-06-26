#include "lwip/opt.h"
#include "globaldata.h"
#include "lwip/apps/httpd.h"
#include "lwip/def.h"
#include "lwip/mem.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/** define LWIP_HTTPD_EXAMPLE_GENERATEDFILES to 1 to enable this file system */
#ifndef LWIP_HTTPD_EXAMPLE_SIMPLEPOST
#define LWIP_HTTPD_EXAMPLE_SIMPLEPOST 1
#define LWIP_HTTPD_SUPPORT_POST 1
#endif

#if LWIP_HTTPD_EXAMPLE_SIMPLEPOST

#if !LWIP_HTTPD_SUPPORT_POST
#error This needs LWIP_HTTPD_SUPPORT_POST
#endif

#define USER_PASS_BUFSIZE 16

static void *current_connection;
static void *valid_connection;
static char last_user[USER_PASS_BUFSIZE];

static char* strchrn( char* string, char character, size_t maxLength) {
    for (size_t i = 0; i< maxLength; ++i) {
        if (string[i] == character) {
            return &string[i];
        }
    }
    return NULL;
}

err_t
httpd_post_begin(void *connection, const char *uri, const char *http_request,
                 u16_t http_request_len, int content_len, char *response_uri,
                 u16_t response_uri_len, u8_t *post_auto_wnd)
{
  printf("runing httpd_post_begin\n");
  LWIP_UNUSED_ARG(connection);
  LWIP_UNUSED_ARG(http_request);
  LWIP_UNUSED_ARG(http_request_len);
  LWIP_UNUSED_ARG(content_len);
  LWIP_UNUSED_ARG(post_auto_wnd);
  if (!memcmp(uri, "/send_info.cgi", 11)) {
    if (current_connection != connection) {
      current_connection = connection;
      valid_connection = NULL;
      /* default page is "login failed" */
      snprintf(response_uri, response_uri_len, "/info.shtml");
      /* e.g. for large uploads to slow flash over a fast connection, you should
         manually update the rx window. That way, a sender can only send a full
         tcp window at a time. If this is required, set 'post_aut_wnd' to 0.
         We do not need to throttle upload speed here, so: */
      *post_auto_wnd = 1;
      return ERR_OK;
    }
  }
  return ERR_VAL;
}

err_t
httpd_post_receive_data(void *connection, struct pbuf *p)
{
  
  printf("runing httpd_post_recive\n");

  //int len = pbuf_copy_partial(p,myData,p->tot_len,0);
  //printf("Total len: %d, String:", len);
  //for (int i = 0;i < len; ++i) {
  //  printf(" 0x%2x", myData[i]);
  //}
printf("\n");
  if (current_connection == connection) {    

    char inputBuffer[1024];
    int bufferSize = pbuf_copy_partial(p, inputBuffer, p->tot_len, 0);

    inputBuffer[bufferSize] = 0;

    if (bufferSize < 1) {
        return ERR_VAL;
    }


    if (inputBuffer[0] == 'i') {
      char* idStr = inputBuffer + 2;
      char* tempStr = strchrn(idStr+1, ' ', bufferSize);
      char* lightStr = strchrn(tempStr+1, ' ', bufferSize);
      if (!tempStr || !lightStr) {
        return ERR_VAL;
    }
    
    
    
    *(tempStr++) = *(lightStr++) = '\0';

    //printf(idStr);
    //printf("\n");
    //printf(tempStr);
    //printf("\n");
    //printf(lightStr);
    //printf("\n");

    int id = atoi(idStr);
    float temp = atof(tempStr);
    float light = atof(lightStr);


    temperatureVector[id] = temp;
    lightVector[id] = light;
    }
    else if(inputBuffer[0] == 's')
    {
      
      relayState = (inputBuffer[2]=='1');
    }
    
    valid_connection = connection;
    
    
    /* not returning ERR_OK aborts the connection, so return ERR_OK unless the
       conenction is unknown */
    return ERR_OK;
  }
  return ERR_VAL;
}

void
httpd_post_finished(void *connection, char *response_uri, u16_t response_uri_len)
{
  printf("runing httpd_post_finished\n");
  /* default page is "login failed" */
  snprintf(response_uri, response_uri_len, "/info.shtml");
  if (current_connection == connection) {
    if (valid_connection == connection) {
      /* login succeeded */
      snprintf(response_uri, response_uri_len, "/info.shtml");
    }
    current_connection = NULL;
    valid_connection = NULL;
  }
}

#endif /* LWIP_HTTPD_EXAMPLE_SIMPLEPOST*/