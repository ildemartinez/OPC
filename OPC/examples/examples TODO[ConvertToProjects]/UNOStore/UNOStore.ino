#include <OPC.h>
#include <Bridge.h>
#include <Ethernet.h>
#include <SPI.h>

/*
 * Declaring the OPC object
 */
OPCSerial aOPC;

/*
 * create a callback function for the OPCItem
 */
int callback(const char *itemID, const opcOperation opcOP, const int value){
  static int store = 0;
  
  if (opcOP == opc_write) 
    store = value;
  else  
    return store;
}

void setup() {
  Serial.begin(9600);
  
  /*
   * OPC Object initialization
   */
  aOPC.setup();
  
  /*
   * random OPCItem declaration
   */
  aOPC.addItem("store",opc_read, opc_int, callback);
}

void loop() {
  /*
   * OPC process commands
   */
  aOPC.processOPCCommands();
}




