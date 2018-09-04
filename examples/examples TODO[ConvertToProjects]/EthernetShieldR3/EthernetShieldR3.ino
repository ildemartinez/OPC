/*
 * A generic sketch for use with the Arduino OPC or Visual OPC Builder from www.st4makers.com
 */
#include <OPC.h>
#include <Bridge.h>
#include <Ethernet.h>
#include <SPI.h>

/*
 * Declaring the OPC object
 */
OPCEthernet myArduinoUno;

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

opcOperation digital_status_input[14], analog_status_input[6];

bool readwrite_digital(const char *itemID, const opcOperation opcOP, const bool value)
{
  byte port;
  
  port = atoi(&itemID[1]);

  if (opcOP == opc_opwrite) {
    if (digital_status_input[port] == opc_opread) {
      digital_status_input[port] = opc_opwrite;
      pinMode(port,OUTPUT);
    }
      
    digitalWrite(port,value);  
  }
  else
  {
    if (digital_status_input[port] == opc_opwrite) {
      digital_status_input[port] = opc_opread;
     // pinMode(port,INPUT);
    } 

    return digitalRead(port); 
  }  

}

int readwrite_analog(const char *itemID, const opcOperation opcOP, const int value) {
  byte port;
  
  port = atoi(&itemID[1]);

  if (opcOP == opc_opwrite) {
    if (analog_status_input[port] == opc_opread) {
      analog_status_input[port] = opc_opwrite;
      pinMode(port,OUTPUT);   
    }
     
    analogWrite(port,value);  
  }
  else
  {
    if (analog_status_input[port] == opc_opwrite) {
      analog_status_input[port] = opc_opread;
      //pinMode(port,INPUT);   
    } 

    return analogRead(port); 
  }  

}

void setup() {
  byte k;

  for (k=0;k<14;k++) digital_status_input[k] = opc_opread; 
  for (k=0;k<5;k++)  analog_status_input[k] = opc_opread; 
  
  /*
   * OPC Object initialization
   */
  myArduinoUno.setup(listen_port,mac,ip); 

  myArduinoUno.addItem("D0",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoUno.addItem("D1",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoUno.addItem("D2",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoUno.addItem("D3",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoUno.addItem("D4",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoUno.addItem("D5",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoUno.addItem("D5",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoUno.addItem("D6",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoUno.addItem("D7",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoUno.addItem("D8",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoUno.addItem("D9",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoUno.addItem("D10",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoUno.addItem("D11",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoUno.addItem("D12",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoUno.addItem("D13",opc_readwrite, opc_bool, readwrite_digital);
  
  myArduinoUno.addItem("A0",opc_readwrite, opc_int, readwrite_analog);
  myArduinoUno.addItem("A1",opc_readwrite, opc_int, readwrite_analog);
  myArduinoUno.addItem("A2",opc_readwrite, opc_int, readwrite_analog);
  myArduinoUno.addItem("A3",opc_readwrite, opc_int, readwrite_analog);
  myArduinoUno.addItem("A4",opc_readwrite, opc_int, readwrite_analog);
  myArduinoUno.addItem("A5",opc_readwrite, opc_int, readwrite_analog);    
}

void loop() {
  myArduinoUno.processOPCCommands();
}

