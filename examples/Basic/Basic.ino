#include <OPC.h>
#include <Bridge.h>

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




