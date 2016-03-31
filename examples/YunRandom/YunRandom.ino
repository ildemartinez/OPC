#include <OPC.h>
#include <Bridge.h>
#include <Ethernet.h>
#include <SPI.h>

/*
 * Declaring the OPC object
 */
OPCNet aOPC;

/*
 * create a callback function for the OPCItem
 */
int callback(const char *itemID, const opcOperation opcOP, const int value){
  return random(-32768,32768);
}

void setup() {
  
  /*
   * OPC Object initialization
   */
  aOPC.setup();
  
  /*
   * random OPCItem declaration
   */
  aOPC.addItem("random",opc_read, opc_int, callback);
}

void loop() {
  /*
   * OPC process commands
   */
  aOPC.processOPCCommands();
}




