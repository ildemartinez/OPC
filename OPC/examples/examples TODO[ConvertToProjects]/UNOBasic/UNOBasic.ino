#include <OPC.h>
#include <Bridge.h>
#include <Ethernet.h>
#include <SPI.h>

/*
 * Declaring the OPC object
 */
OPCSerial aOPCSerial;

void setup() {
  /*
   * OPC Object initialization
   */
  aOPCSerial.setup();
}

void loop() {
  /*
   * OPC process commands
   */
  aOPCSerial.processOPCCommands();
}




