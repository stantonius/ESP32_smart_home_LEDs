#pragma once

#include <Arduino.h>

#include <NimBLEDevice.h>
#include "NimBLEBeacon.h"
#include <sstream>
#include "device_proximity_holder.h"
#include <unordered_set>

using namespace std;

#define LOG(x) cout << x << endl
#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00) >> 8) + (((x)&0xFF) << 8))
#define rssiThreshold -100

NimBLEScan *pBLEScan;

// Queue for multicore processing
QueueHandle_t isCloseQueue;

bool isCloseVal = 0;
DeviceProximityHolder deviceProximityHolder;

// Create an empty unordered_set to store the results for each scan
unordered_set<int> scanResults;

/*
* Code taken from:
* https://github.com/filipsPL/cat-localizer/blob/main/2-ESP32/esp32-ds18b20-living_room.ino
* https://github.com/h2zero/NimBLE-Arduino/blob/master/examples/NimBLE_Scan_Continuous
* /NimBLE_Scan_Continuous.ino
https://github.com/h2zero/NimBLE-Arduino/blob/master/examples/BLE_Beacon_Scanner/BLE_Beacon_Scanner.ino
*/

int scanTime = 5;
#define SERVICE_UUID "c8c706b9-879a-4682-ba7f-56346f4d800e"

static NimBLEUUID dataUuid(SERVICE_UUID);

class MyAdvertisedDeviceCallbacks : public NimBLEAdvertisedDeviceCallbacks
{

    void onResult(NimBLEAdvertisedDevice *advertisedDevice)
    {

        // store whether ibeacon was found and return result at end of callback
        // start off assuming the beacon is not close
        int beaconPresent = 0;

        if (!advertisedDevice->haveServiceUUID())
        // {
        //     // BLEUUID devUUID = advertisedDevice->getServiceUUID();
        //     // Serial.print("Found ServiceUUID: ");
        //     // Serial.println(devUUID.toString().c_str());
        //     // Serial.println("");
        // }
        // else
        {
            if (advertisedDevice->haveManufacturerData() == true)
            {
                std::string strManufacturerData = advertisedDevice->getManufacturerData();

                uint8_t cManufacturerData[100];
                strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);

                if (strManufacturerData.length() == 25 && cManufacturerData[0] == 0x4C && cManufacturerData[1] == 0x00)
                {
                    Serial.println("Found an iBeacon!");
                    BLEBeacon oBeacon = BLEBeacon();
                    oBeacon.setData(strManufacturerData);
                    Serial.printf("iBeacon Frame\n");
                    Serial.printf("ID: %04X Major: %d Minor: %d UUID: %s Power: %d\n", oBeacon.getManufacturerId(), ENDIAN_CHANGE_U16(oBeacon.getMajor()), ENDIAN_CHANGE_U16(oBeacon.getMinor()), oBeacon.getProximityUUID().toString().c_str(), oBeacon.getSignalPower());
                    Serial.printf("RSSI: %i", advertisedDevice->getRSSI());
                    if (oBeacon.getProximityUUID().toString() == SERVICE_UUID)
                    {
                        if (advertisedDevice->getRSSI() > rssiThreshold)
                        {
                            if (!isCloseVal)
                            {
                                isCloseVal = true;
                            }
                            beaconPresent = 1;
                        }
                        else
                        {
                            isCloseVal = false;
                        }
                    }
                }
            }
            scanResults.emplace(beaconPresent);
            return;
        }
    };
};

NimBLEScan *
scanBLESetup()
{

    Serial.print("Scanning...");

    NimBLEDevice::init("NimBLE device");
    pBLEScan = NimBLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);

    return pBLEScan;
}

void doBLEScans(NimBLEScan *pScan)
{
    /* 
    * Get results of scan;
    * Note that the scan results each trigger the callback when there are results regardless if its a beacon
    * Therefore we need to:
    * 1. Empty unordered_set to store the results for each scan
    * 2. Check the set to see if it includes a beacon
    * */
    scanResults.clear();

    NimBLEScanResults foundDevices = pScan->start(scanTime, false);

    if (scanResults.find(1) != scanResults.end())
    {
        deviceProximityHolder.add(1);
        Serial.println("Beacon presence recorded");
    }
    else
    {
        deviceProximityHolder.add(0);
        Serial.println("No beacon presence recorded");
    };

    Serial.print("Devices found: ");
    Serial.println(foundDevices.getCount());
    Serial.println("THE SUM IS: ");
    Serial.println(scanResults.size());

    // non-zero devices found
    pScan->clearResults(); // delete results fromBLEScan buffer to release memory
}