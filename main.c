#include "lwip/apps/httpd.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"
#include "ssi.h"
#include "cgi.h"
#include "post.h"

// Wifi SSID and password

const char WIFI_SSID[] = "UPC1157195";
const char WIFI_PASSWORD[] = "zfsvzf2sJ4dycBjj";

//const char WIFI_SSID[] = "POCO F5";
//const char WIFI_PASSWORD[] = "lolxdlolxdlolxd";

int main() {

    //inicialization
    stdio_init_all();

    cyw43_arch_init();

    cyw43_arch_enable_sta_mode();

    while(cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID,WIFI_PASSWORD,CYW43_AUTH_WPA2_AES_PSK,30000)) {
        printf("Attempting to connect...\n");
    }

    printf("Connecion success\n");
    httpd_init();
    printf("Http server initialised\n");

    ssi_init();
    printf("SSI initialised\n");

    cgi_init();
    printf("CGI initialised\n");

    //loop

    while(true) {
        
        while (ledBlink) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(250);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(250);
    }
    }
    /*
    stdio_init_all();
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }
    
    */
}