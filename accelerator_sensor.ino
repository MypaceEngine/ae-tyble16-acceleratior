// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
#include <BLEPeripheral.h>

// define pins (varies per shield/board)
#define BLE_REQ   10
#define BLE_RDY   2
#define BLE_RST   9

BLEPeripheral blePeripheral = BLEPeripheral(BLE_REQ, BLE_RDY, BLE_RST);

BLEService xService = BLEService("181A");
BLEUnsignedLongCharacteristic xCharacteristic = BLEUnsignedLongCharacteristic("2A67", BLERead | BLENotify);
BLEDescriptor xDescriptor = BLEDescriptor("290C", "Acceleration");

void setup() {
  Serial.begin(115200);
  
  blePeripheral.setLocalName("AccelerationSensor");

  blePeripheral.setAdvertisedServiceUuid(xService.uuid());
  blePeripheral.addAttribute(xService);
  blePeripheral.addAttribute(xCharacteristic);
  blePeripheral.addAttribute(xDescriptor);
  
  blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  blePeripheral.begin();

  pinMode(4, INPUT);
  pinMode(6, INPUT);
  pinMode(5, INPUT);
}

void loop() {

  long x=(long)analogRead(4);
  long y=(long)analogRead(5);
  long z=(long)analogRead(6);

  xCharacteristic.setValue(((z*1024+y)*1024)+x);
  blePeripheral.poll();
  delay(100);
}
void blePeripheralConnectHandler(BLECentral& central) {
  Serial.print(F("Connected event, central: "));
  Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLECentral& central) {
  Serial.print(F("Disconnected event, central: "));
  Serial.println(central.address());
}
