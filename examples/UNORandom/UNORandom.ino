#include <OPC.h>
#include <Bridge.h>
#include <Ethernet.h>
#include <SPI.h>

/*
 * Declaring the OPC object
 */
OPCSerial aOPCSerial;

/*
 * create a callback function for the OPCItem
 */
int callback(const char *itemID, const opcOperation opcOP, const int value){
  return random(-32768,32768);
}

void setup() {
  Serial.begin(9600);
  
  /*
   * OPC Object initialization
   */
  aOPCSerial.setup();
  
  /*
   * random OPCItem declaration
   */
  aOPCSerial.addItem("random",opc_read, opc_int, callback);
}

void loop() {
  /*
   * OPC process commands
   */
  aOPCSerial.processOPCCommands();
}




