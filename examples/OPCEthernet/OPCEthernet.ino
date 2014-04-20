#include <OPCNet.h>
#include <SPI.h>        
#include <Ethernet.h>

/*
 * Declaring the OPC object
 */
OPCNet aOPCNet;

// MAC address from Ethernet shield sticker under board
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xAD, 0x8D };
IPAddress ip(192, 168, 1, 179);
IPAddress gateway(192,168,1,1);
IPAddress dns_server(192,168,1,179);
IPAddress subnet(255,255,255,0);

/*
 * create a callback function for the OPCItem
 */
int callback(const char *itemID, const opcOperation opcOP, const int value){
  return random(-32768,32768);
}

void setup() {
  Serial.begin(9600);
  
  /*
   * OPCNet Object initialization
   */
  aOPCNet.setup(mac,ip,dns,gateway,subnet);
  
  /*
   * random OPCItem declaration
   */
  aOPCNet.addItem("random",opc_read, opc_int, callback);
}

void loop() {
  /*
   * OPC process commands
   */
  aOPCNet.processOPCCommands(); 
}




