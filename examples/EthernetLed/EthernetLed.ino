#include <OPC.h>
#include <Bridge.h>
#include <Ethernet.h>
#include <SPI.h>

/*
 * Declaring the OPC object
 */
OPCEthernet aOPCEthernet;

/*
 * set led status from OPC Client - Ethernet Shield uses pin13 :( 
 */
int ledPin = 6;  

/*
 * MAC address from Ethernet shield sticker under board
 */
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xAD, 0x8D };

/*
 * Set your network configuration
 */
IPAddress ip(192, 168, 1, 179);
IPAddress gateway(192,168,1,1);
IPAddress dns_server(192,168,1,1);
IPAddress subnet(255,255,255,0);

/*
 * Server listen port
 */
const int listen_port = 80;
/*
 * create a callback function for the OPCItem
 */
bool callback(const char *itemID, const opcOperation opcOP, const bool value){
  static bool ledValue = false;
  
  /*
   * if operation is a write command from OPC Client
   */
  if (opcOP == opc_opwrite) {
    ledValue = value;
    
    if (ledValue)  
      digitalWrite(ledPin, HIGH);
    else
      digitalWrite(ledPin, LOW);
  } 
  else 
    /*
     * read the led status
     */
    return ledValue;  
}

void setup() {
  Serial.begin(9600);
  
  pinMode(ledPin, OUTPUT);
  
  /*
   * OPC Object initialization
   */
  aOPCEthernet.setup(listen_port,mac,ip);     
  /*
   * led OPCItem declaration
   */
  aOPCEthernet.addItem("led",opc_readwrite, opc_bool, callback);
}

void loop() {
  /*
   * OPC process commands
   */
  aOPCEthernet.processOPCCommands();   
}