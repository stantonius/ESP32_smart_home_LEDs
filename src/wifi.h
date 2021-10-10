#pragma once

#include <WiFi.h>
#include <credentials.h>

#define MINUTES_BETWEEN 1

const char *ssid = mySSID;
const char *password = myPassword;

unsigned long entry;

/*
TODO: Add reconnect function/method if internet drops
*/

void setup_wifi()
{
    delay(10);
    Serial.println();
    Serial.print("Connection to..");
    Serial.print(ssid);

    WiFi.begin(ssid, password);

    entry = millis();
    while ((WiFi.status() != WL_CONNECTED) && (millis() - entry < 15000))
    {
        delay(500);
        Serial.print(".");
    }
    if (millis() - entry > 15000)
    {
        ESP.deepSleep((MINUTES_BETWEEN * 30e6));
    }
    randomSeed(micros());

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}
