/*
 * A generic sketch for use with the Arduino OPC or Visual OPC Builder from www.st4makers.com
 */
#include <OPC.h>
#include <Bridge.h>
#include <Ethernet.h>
#include <SPI.h>

OPCSerial myArduinoUno;

opcAccessRights digital_status_input[14], analog_status_input[6];

bool readwrite_digital(const char *itemID, const opcOperation opcOP, const bool value)
{
  byte port;
  
  OPCItemType aOPCItem = myArduinoUno.getOPCItem(itemID);

  port = atoi(&itemID[1]);

   if (opcOP == opc_opwrite) {
    if ((aOPCItem.opcAccessRight == opc_write) || (aOPCItem.opcAccessRight == opc_readwrite)) {
      
      if (digital_status_input[port] != opc_write) {
        pinMode(port, OUTPUT);
        digital_status_input[port] = opc_write;
      }

      digitalWrite(port,value);
    }
  }

  if (opcOP == opc_opread) {
    if ((aOPCItem.opcAccessRight == opc_read) || (aOPCItem.opcAccessRight == opc_readwrite)) {
      
      if (digital_status_input[port] != opc_read) {
       //pinMode(port, INPUT);
        digital_status_input[port] = opc_read;
      }

      return digitalRead(port);
    }
  } 
}

int readwrite_analog(const char *itemID, const opcOperation opcOP, const int value) 
{
 byte port;
    
  OPCItemType aOPCItem = myArduinoUno.getOPCItem(itemID);                     

  port = atoi(&itemID[1]);
           
  if (opcOP == opc_opwrite) {
    if ((aOPCItem.opcAccessRight == opc_write) || (aOPCItem.opcAccessRight == opc_readwrite)) {
      
      if (analog_status_input[port] != opc_write) {
        pinMode(port, OUTPUT);
        analog_status_input[port] = opc_write;
      }

      analogWrite(port,value);
    }
  }

  if (opcOP == opc_opread) {
    if ((aOPCItem.opcAccessRight == opc_read) || (aOPCItem.opcAccessRight == opc_readwrite)) {
      
      if (analog_status_input[port] != opc_read) {
        //pinMode(port, INPUT);
        analog_status_input[port] = opc_read;
      }

      return analogRead(port);
    }
  } 
  
}

void setup() {
  byte k;

  Serial.begin(9600);

  for (k=0;k<14;k++) digital_status_input[k] = opc_undefined; 
  for (k=0;k<5;k++) analog_status_input[k] = opc_undefined; 

  myArduinoUno.setup(); 
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

