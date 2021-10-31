# LED Lights IoT

## Objective

1. Scan for advertising BLE devices with a specific UUID
2. If within certain range, turn on WS2812B lights 

## Status

In progress. Currently achieves the outlined objectives, albeit with a few bugs

## Features

* Connects to Wifi and MQTT server on startup
* Uses multithreading to do the following simultaneously:
  1. Continuously scans for BLE devices and evaluates proximity through RSSI
  2. Runs FastLED program on second thread (avoids flickering caused by BT processing when on the same thread)

## ToDo
- [ ] Move lightswitch to the server (MQTT + NodeRed)
- [ ] NodeRed only turns off lights when 2 successive readings return negative (avoids false negatives that occur periodically)
- [ ] Add second power wires in series to bypass the breadboard. This way we can turn up the brightness (maybe make the brightness variable depending on the time of day)
