#include <OPC.h>
#include <Ethernet.h>
#include <Bridge.h>
#include <SPI.h>

/*
 * Declaring the OPC object
 */
OPCEthernet aOPCEthernet;

/*
 * MAC address from Ethernet shield sticker under board
 */
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xAD, 0x8D };

/*
 * Set your network configuration
 */
IPAddress ip(10, 1, 0, 179);
IPAddress gateway(10,1,0,1);
IPAddress dns_server(10,1,0,1);
IPAddress subnet(255,255,255,0);

/*
 * Server listen port
 */
const int listen_port = 80;

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
  aOPCEthernet.setup(listen_port,mac,ip);     

  /*
   * random OPCItem declaration
   */
  aOPCEthernet.addItem("random",opc_read, opc_int, callback);
}

void loop() {
  /*
   * OPC process commands
   */
  aOPCEthernet.processOPCCommands();   
}




