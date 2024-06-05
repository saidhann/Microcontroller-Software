#include "lwip/apps/httpd.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"
#include "ssi.h"
#include "cgi.h"
#include "postrequest.h"

// Wifi SSID and password

float adc_to_temperature(uint16_t raw) {
    const float conversion_factor = 3.3f / (1 << 12);
    float voltage = raw * conversion_factor;
    // The voltage value should be calibrated for accurate temperature reading
    float temperature = 27.0 - (voltage - 0.706) / 0.001721;
    return temperature;
}

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

    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    //loop

    while(true) {
        
        if(relayMode==automatic) {
            if((temperatureVector[0]+temperatureVector[1]+temperatureVector[2]+temperatureVector[3])/4<confortableTemperature) relayState = 1;
            else relayState = 0;
        }
        uint16_t raw = adc_read();

        temperatureVector[0] = adc_to_temperature(raw);

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