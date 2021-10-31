#pragma once

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <wifi_connector.h>

const char *mqtt_server = "10.0.0.54";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void reconnect()
{
    // Loop until we're reconnected

    while (!client.connected())
    {
        int reconnect_count = 0;
        while (reconnect_count < 10)
        {
            Serial.print("Attempting MQTT connection...");
            // Create a random client ID
            String clientId = "ESP8266Client-";
            clientId += String(random(0xffff), HEX);
            // Attempt to connect
            if (client.connect(clientId.c_str()))
            {
                Serial.println("connected");
                // Once connected, publish an announcement...
                client.publish("LIGHTSENSOR/status", "booted");
                // ... and resubscribe
                client.subscribe("inTopic");
                reconnect_count = 10;
            }
            else
            {
                reconnect_count++;
            }
        }
        // Serial.print("failed, rc=");
        // Serial.print(client.state());
        // Serial.println(" Going to sleep");
        // delay(100);
        // ESP.deepSleep(MINUTES_BETWEEN * 30e6);
    }
}

void subscribe_to_topic(const char *topic)
{
    client.subscribe(topic);
}

void sendMqttMsg(String topic, byte *payload)
{
    if (client.connected())
    {
    }
    else
    {
        reconnect();
    }
}