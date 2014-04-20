#include <OPC.h>

/*
 * Declaring the OPC object
 */
OPCNet aOPC;

void setup() {
  /*
   * OPC Object initialization
   */
  aOPC.setup();
}

void loop() {
  /*
   * OPC process commands
   */
  aOPC.processOPCCommands();
}