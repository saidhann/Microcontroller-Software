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
    
    printf("HD44780 : Start!\r\n");
    //send_post_request();
    myLCD.PCF8574_LCDInit(myLCD.LCDCursorTypeOff, 4, 20);
	myLCD.PCF8574_LCDBackLightSet(true);

    while (1) {
        //reading temperature
        printf("Device Address: %02x%02x%02x%02x%02x%02x%02x%02x\n", address.rom[0], address.rom[1], address.rom[2], address.rom[3], address.rom[4], address.rom[5], address.rom[6], address.rom[7]);
        one_wire.single_device_read_rom(address);
        one_wire.convert_temperature(address,true,false);
        currentTemperature = one_wire.temperature(address);
        
        send_post_request();
        cyw43_arch_poll();
        //printf(inputBuffer);
        printf("\n");

        //displaying temperature
        char line1[64];
        char line2[64];
        char line3[64];
        char line4[64];

        char *token;
        const char delimiter[2] = "\n";
        float floats[100];
        int index = 0;
        printf("%s\n",inputBuffer);
        if(inputBuffer[1]=='n'){
            relayState = true;
            *inputBuffer+=3;
        }
        else if(inputBuffer[1]=='f') {
            relayState = false;
            *inputBuffer+=4;
        }
        token = strtok(inputBuffer, delimiter);

        while (token != NULL) {
            floats[index++] = strtof(token, NULL);
            token = strtok(NULL, delimiter);
        }
        for(int i = 0;i<4;i++) if(floats[i]<0.0||floats[i]>70.0) floats[i] = 0;
        for(int i = 4;i<8;i++) if(floats[i]<0.0||floats[i]>100.0) floats[i] = 0;
        snprintf(line1,sizeof(line1),"T0:%0.1fC T1:%0.1fC",floats[1],currentTemperature);
        snprintf(line2,sizeof(line2),"T2:%0.1fC T3:%0.1fC",floats[3],floats[4]);
        snprintf(line3,sizeof(line3),"L0:%0.1f%% L1:%0.1f%%",floats[5],currentLight);
        snprintf(line4,sizeof(line4),"L3:%0.1f%% L4:%0.1f%%",floats[7],floats[8]);
        /*
        printf(line1);
        printf("\n");
        printf(line2);
        printf("\n");
        printf(line3);
        printf("\n");
        printf(line4);
        printf("\n");
        */
        myLCD.PCF8574_LCDClearScreen();
        myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberOne, 0);
	    myLCD.PCF8574_LCDSendString(line1);
        myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo, 0);
        myLCD.PCF8574_LCDSendString(line2);
        myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberThree, 0);
	    myLCD.PCF8574_LCDSendString(line3);
        myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberFour, 0);
        myLCD.PCF8574_LCDSendString(line4);
        myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberOne, 18);
        myLCD.PCF8574_LCDSendChar('B');
        myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo, 18);
        myLCD.PCF8574_LCDSendChar('O');
        myLCD.PCF8574_LCDSendChar('O');
        myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberThree, 18);
        myLCD.PCF8574_LCDSendChar('I');
        if(relayState) myLCD.PCF8574_LCDSendChar('N');
        else myLCD.PCF8574_LCDSendChar('F');
        myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberFour, 18);
        myLCD.PCF8574_LCDSendChar('L');
        if(!relayState) myLCD.PCF8574_LCDSendChar('F');
        
        sleep_ms(10000);
        //sending temperature
    }

    return 0;
}
