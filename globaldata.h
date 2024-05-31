//storage for temperature data and other
#pragma once

float temperatureVector[] = {20.000,18.000,14.000,27.000};

float lightVector[] = {90.000,50.000,4.000,27.000};

float historicTempeartureData[4][14400];
float historicTemperatureData[4][14400];


bool relayState = false;

bool ledBlink = false;