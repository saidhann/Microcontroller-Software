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

err_t
httpd_post_begin(void *connection, const char *uri, const char *http_request,
                 u16_t http_request_len, int content_len, char *response_uri,
                 u16_t response_uri_len, u8_t *post_auto_wnd)
{
  LWIP_UNUSED_ARG(connection);
  LWIP_UNUSED_ARG(http_request);
  LWIP_UNUSED_ARG(http_request_len);
  LWIP_UNUSED_ARG(content_len);
  LWIP_UNUSED_ARG(post_auto_wnd);
  if (!memcmp(uri, "/sendtemperature.cgi", 11)) {
    if (current_connection != connection) {
      current_connection = connection;
      valid_connection = NULL;
      /* default page is "login failed" */
      snprintf(response_uri, response_uri_len, "/index.html");
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
  if (current_connection == connection) {
    u16_t token_client_id = pbuf_memfind(p, "id=", 3, 0);
    u16_t token_temperature = pbuf_memfind(p, "temp=", 5, 0);
    u16_t token_light = pbuf_memfind(p, "light=", 6, 0);

    int id_length = 1;
    int temp_length = 9;
    int light_length = 9;

    char id_buf[id_length];
    char temp_buf[temp_length];
    char light_buf[light_length];
    pbuf_get_contiguous(p,id_buf,sizeof(id_buf),id_length,token_client_id+3);
    pbuf_get_contiguous(p,temp_buf,sizeof(temp_buf),temp_length,token_temperature+5);
    pbuf_get_contiguous(p,light_buf,sizeof(light_buf),light_length,token_light+3);

    temperatureVector[(int)id_buf] = atof(temp_buf);
    lightVector[(int)id_buf] = atof(light_buf);

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
  /* default page is "login failed" */
  snprintf(response_uri, response_uri_len, "/index.html");
  if (current_connection == connection) {
    if (valid_connection == connection) {
      /* login succeeded */
      snprintf(response_uri, response_uri_len, "/index.html");
    }
    current_connection = NULL;
    valid_connection = NULL;
  }
}

#endif /* LWIP_HTTPD_EXAMPLE_SIMPLEPOST*/