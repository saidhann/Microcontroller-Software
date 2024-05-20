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

const char * cgi_temperature_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]){
    return "/temperature.shtml";
}

const char * cgi_light_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]){
    return "/light.shtml";
}

const char * cgi_set_light_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]){
    return "/index.shtml";
}

const char * cgi_set_temperature_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]){
    return "index.shtml";
}

static const tCGI cgi_handlers[] = {
    {
        //HTML request for /led.cgi triggers cgi_led_handler
        "/led.cgi", cgi_led_handler
    },
    {
        "/temperature.cgi", cgi_temperature_handler
    },
    {
        "/sendtemperature.cgi",cgi_set_temperature_handler
    },
    {
        "/light.cgi",cgi_light_handler
    },
    {
        "/sendlight.cgi",cgi_set_light_handler
    }
};

void cgi_init(void) {
    http_set_cgi_handlers(cgi_handlers,5);
}