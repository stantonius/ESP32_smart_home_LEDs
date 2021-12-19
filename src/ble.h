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
#define rssiThreshold -90

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

// Note this service UUID was randomly generated via a free online tool
// This ID is also referenced in the Flutter app, where it broadcasts with this UUID
// Comparing service UUIDs to this ID is how we know an iBeacon (our phone, broadcasting this UUID) is present
#define SERVICE_UUID "c8c706b9-879a-4682-ba7f-56346f4d800e"

static NimBLEUUID dataUuid(SERVICE_UUID);

class MyAdvertisedDeviceCallbacks : public NimBLEAdvertisedDeviceCallbacks
{

    void onResult(NimBLEAdvertisedDevice *advertisedDevice)
    {

        // store whether ibeacon was found and return result at end of callback
        // start off assuming the beacon is not close
        int beaconPresent = 0;

        // I'm not entirely sure why, but these iBeacons won't have a service UUID
        // Therefore we can use this factor as an exclusion criteria of other advertising devices
        if (!advertisedDevice->haveServiceUUID())
        {
            // iBeacon always shows Apple as the manufacturer, therefore used as filter criteria
            if (advertisedDevice->haveManufacturerData() == true)
            {
                std::string strManufacturerData = advertisedDevice->getManufacturerData();

                uint8_t cManufacturerData[100];
                strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);

                // check if the manufacturer data is actually Apple
                if (strManufacturerData.length() == 25 && cManufacturerData[0] == 0x4C && cManufacturerData[1] == 0x00)
                {
                    Serial.println("Found an iBeacon!");
                    BLEBeacon oBeacon = BLEBeacon();
                    oBeacon.setData(strManufacturerData);
                    Serial.printf("iBeacon Frame\n");
                    // Serial.printf("ID: %04X Major: %d Minor: %d UUID: %s Power: %d\n", oBeacon.getManufacturerId(), ENDIAN_CHANGE_U16(oBeacon.getMajor()), ENDIAN_CHANGE_U16(oBeacon.getMinor()), oBeacon.getProximityUUID().toString().c_str(), oBeacon.getSignalPower());
                    Serial.printf("ID: %04X UUID: %s Power: %d\n", oBeacon.getManufacturerId(), oBeacon.getProximityUUID().toString().c_str(), oBeacon.getSignalPower());
                    Serial.printf("RSSI: %i", advertisedDevice->getRSSI());

                    // check if the beacon is the one we set the service UUID for
                    // this will exclude iTags for example
                    if (oBeacon.getProximityUUID().toString() == SERVICE_UUID)
                    {
                        if (advertisedDevice->getRSSI() > rssiThreshold)
                        {
                            // if (!isCloseVal)
                            // {
                            //     isCloseVal = true;
                            // }
                            beaconPresent = 1;
                        }
                        // else
                        // {
                        //     isCloseVal = false;
                        // }
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
    // pBLEScan->setInterval(100);
    // pBLEScan->setWindow(99);

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
     * 3. If a beacon is found, add 1 to the holder vector; if not, add 0 to the holder vector
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

    // stopping might be key as we want to free up the device antenna to use MQTT over Wifi
    pScan->stop();
    pScan->clearResults(); // delete results fromBLEScan buffer to release memory
}