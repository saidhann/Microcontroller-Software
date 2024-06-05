#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "globaldata.h"




// CGI handler for /led.cgi request
const char * cgi_led_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]){
    // Check if request for led has been made /led.cgi?led=x
    if(strcmp(pcParam[0],"led")==0) {
        //check prameter value
        if(strcmp(pcValue[0],"0")==0)
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN,0);
        if(strcmp(pcValue[0],"1")==0)
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN,1);
        if(strcmp(pcValue[0],"3")==0) {
            ledBlink = !ledBlink;
        }
    }

    // Send the index page to the user
    return "/index.shtml";
}

const char * cgi_relay_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]){
    if(strcmp(pcParam[0],"relay")==0) {
        //check prameter value
        if(strcmp(pcValue[0],"0")==0)
            relayState=!relayState;
        if(strcmp(pcValue[0],"1")==0)
            relayMode = !relayMode;
    }
    return "/index.shtml";
}

const char * cgi_info_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]){
    return "/info.shtml";
}

const char * cgi_send_info_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]){
    return "info.shtml";
}

static const tCGI cgi_handlers[] = {
    {
        //HTML request for /led.cgi triggers cgi_led_handler
        "/led.cgi", cgi_led_handler
    },
    {
        "/info.cgi", cgi_info_handler
    },
    {
        "/send_info.cgi",cgi_send_info_handler
    },
    {
        "/relay.cgi", cgi_relay_handler
    }
};

void cgi_init(void) {
    http_set_cgi_handlers(cgi_handlers,4);
}