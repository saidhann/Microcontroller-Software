#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"
#include "temperature.h"


const char * ssi_tags[] = {"myTemp","temp1","temp2","temp3","temp4","light1","light2","light3","light4"};

uint16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen){
    size_t printed;

    switch(iIndex){
        case 0: //myTemp
        {
            const float voltage = adc_read() * 3.3f / (1 << 12);
            const float tempC = 27.0f - (voltage - 0.706f) / 0.001721f;
            printed = snprintf(pcInsert,iInsertLen, "%f",tempC);
        }
        break;
        case 1: //temp1
        {
                printed = snprintf(pcInsert,iInsertLen,"%f",temperatureVector[0]);
        }
        break;
        case 2: //temp2
        {
                printed = snprintf(pcInsert,iInsertLen,"%f",temperatureVector[1]);
        }
        case 3: //temp3
        {
                printed = snprintf(pcInsert,iInsertLen,"%f",temperatureVector[2]);
        }
        case 4: //temp4
        {
                printed = snprintf(pcInsert,iInsertLen,"%f",temperatureVector[3]);
        }
        case 5: //light1
        {
                printed = snprintf(pcInsert,iInsertLen,"%f",lightVector[0]);
        }
        case 6: //light2
        {
                printed = snprintf(pcInsert,iInsertLen,"%f",lightVector[1]);
        }
        case 7: //light3
        {
                printed = snprintf(pcInsert,iInsertLen,"%f",lightVector[2]);
        }
        case 8: //light4
        {
                printed = snprintf(pcInsert,iInsertLen,"%f",lightVector[3]);
        }
        break;
        default:
            printed = 0;
            break;
    }

    return (uint16_t)printed;
}

void ssi_init(){
    //initialise adc first
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    // Configure SSI handler
    http_set_ssi_handler(ssi_handler, ssi_tags,LWIP_ARRAYSIZE(ssi_tags));
}