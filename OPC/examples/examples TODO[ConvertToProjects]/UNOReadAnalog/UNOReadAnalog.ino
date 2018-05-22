#include <OPC.h>
#include <Bridge.h>
#include <Ethernet.h>
#include <SPI.h>

/*
 * Declaring the OPC object
 */
OPCSerial aOPCSerial;

/*
 * We want to publish analogPin 3 to OPC Client
 */
int analogPin = 3;

/*
 * create a callback function for the OPCItem
 */
int callback(const char *itemID, const opcOperation opcOP, const int value){
  return analogRead(analogPin);
}

void setup() {
  Serial.begin(9600);
  
  /*
   * OPC Object initialization
   */
  aOPCSerial.setup();
  
  /*
   * A0 OPCItem declaration
   */
  aOPCSerial.addItem("A0",opc_read, opc_int, callback);
}

void loop() {
  /*
   * OPC process commands
   */
  aOPCSerial.processOPCCommands();
}




