#include <cstdio>
#include "lwip/apps/httpd.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"
#include "send_request.h"
#include "HD44780_LCD_PCF8574.hpp"
#include "hardware/gpio.h"
#include "modules/pico-onewire/api/one_wire.h"

// Wifi SSID and password

const char WIFI_SSID[] = "UPC1157195";
const char WIFI_PASSWORD[] = "zfsvzf2sJ4dycBjj";
HD44780LCD myLCD(LCD_I2C_ADDRESS, i2c1, 100, 18,19);


//const char WIFI_SSID[] = "POCO F5";
//const char WIFI_PASSWORD[] = "lolxdlolxdlolxd";

void wifi_init(void) {
    if (cyw43_arch_init()) {
        printf("WiFi init failed\n");
        return;
    }

    cyw43_arch_enable_sta_mode();

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("WiFi connection failed\n");
        return;
    }

    printf("WiFi connected\n");
}


int main() {
    stdio_init_all();

    wifi_init();
    One_wire one_wire(0);
    one_wire.init();
    rom_address_t address{};
    one_wire.single_device_read_rom(address);
    printf("HD44780 : Start!\r\n");
    //send_post_request();
    myLCD.PCF8574_LCDInit(myLCD.LCDCursorTypeOn, 2, 16);
	myLCD.PCF8574_LCDBackLightSet(true);

    while (1) {
        //reading temperature
        printf("Device Address: %02x%02x%02x%02x%02x%02x%02x%02x\n", address.rom[0], address.rom[1], address.rom[2], address.rom[3], address.rom[4], address.rom[5], address.rom[6], address.rom[7]);
        one_wire.convert_temperature(address,true,false);
        currentTemperature = one_wire.temperature(address);
        printf("Temp: %f\n",currentTemperature);
        
        //displaying temperature
        char testString[40];
        snprintf(testString,sizeof(testString),"Temp: %f C",currentTemperature);
        myLCD.PCF8574_LCDClearScreen();
        myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberOne, 0);
        printf(testString);
        printf("\n");
	    myLCD.PCF8574_LCDSendString(testString);

        //sending temperature
        send_post_request();
    }

    return 0;
}
