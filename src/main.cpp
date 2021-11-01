
#include <Arduino.h>
#include <iostream>
#include <lights.h>
#include <ble.h>
#include <wifi_connector.h>
#include <mqtt2.h>

/**
 * Apparently tasks 
*/

TaskHandle_t TaskRunBLEChecks;
TaskHandle_t TaskFastLED;

void codeForTaskRunBLEChecks(void *parameter)
{
    for (;;)
    {
        if (pBLEScan->isScanning() == false)
        {
            // Start scan with: duration = 0 seconds(forever), no scan end callback, not a continuation of a previous scan.
            doBLEScans(pBLEScan);
            delay(100);
        }
        if (deviceProximityHolder.sum() == 0)
        {
            if (isCloseVal)
            {
                isCloseVal = !isCloseVal;
            }
            mqttClient.publish("BeaconProximity", 0, false, "no");
        }
        else
        {
            mqttClient.publish("BeaconProximity", 0, false, "yes");
        }
        delay(100);
    }
}

void codeForTaskFastLED(void *parameter)
{
    for (;;)
    {
        lighter(isCloseVal);
    }
}

void setup()
{
    Serial.begin(115200);

    mqttSetups();

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
