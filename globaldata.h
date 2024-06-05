//storage for temperature data and other
#pragma once

enum mode{
    manual = 0,
    automatic = 1
};

float confortableTemperature = 23.0;

float temperatureVector[] = {23.000,21.000,19.553416,22.195571};

float lightVector[] = {91.456385,50.405036,14.162817,27.258451};

float historicTempeartureData[4][14400];
float historicTemperatureData[4][14400];

int relayMode = manual;

bool relayState = false;

bool ledBlink = false;