
#include <Arduino.h>
#include <iostream>
#include <lights.h>
#include <ble.h>
#include <wifi_connector.h>
#include <mqtt.h>
#include <chrono>

/**
 * Apparently tasks
 */

TaskHandle_t TaskRunBLEChecks;
TaskHandle_t TaskFastLED;

void codeForTaskRunBLEChecks(void *parameter)
{

    for (;;)

    {

        doBLEScans(pBLEScan);
        if (pBLEScan->isScanning() == false)
        {
            // Start scan with: duration = 0 seconds(forever), no scan end callback, not a continuation of a previous scan.
            doBLEScans(pBLEScan);
        }

        if (deviceProximityHolder.sum() == 0)
        {
            // if (isCloseVal)
            // {
            //     isCloseVal = !isCloseVal;
            // }
            // LOG("TRIGGERED");
            sendMqttMsg("BeaconProximity", "no");
        }
        else
        {
            // LOG("NOT TRIGGERED");
            sendMqttMsg("BeaconProximity", "yes");
        }
    }
}

void codeForTaskFastLED(void *parameter)
{
    for (;;)
    {
        client.loop();
        sleep(50);
        lighter(isCloseVal);
    }
}

void setup()
{
    Serial.begin(115200);

    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);

    if (!client.connected())
    {
        reconnect();
    }

    pBLEScan = scanBLESetup();

    lightSetups();

    xTaskCreatePinnedToCore(
        codeForTaskRunBLEChecks,
        "TaskRunBLEChecks",
        10000,
        NULL,
        1,
        &TaskRunBLEChecks,
        0);

    isCloseQueue = xQueueCreate(1, sizeof(int));

    xTaskCreatePinnedToCore(
        codeForTaskFastLED,
        "TaskFastLED",
        10000,
        isCloseQueue,
        1,
        &TaskFastLED,
        1);
}

void loop()
{
    // Not needed with the multithreading
}
