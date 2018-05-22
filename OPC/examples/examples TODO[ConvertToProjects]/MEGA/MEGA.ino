/*
 * A generic sketch for use with the Arduino OPC or Visual OPC Builder from www.st4makers.com
 */
#include <OPC.h>
#include <Bridge.h>
#include <Ethernet.h>
#include <SPI.h>

OPCSerial myArduinoMEGA;

opcAccessRights digital_status_input[54], analog_status_input[16];

bool readwrite_digital(const char *itemID, const opcOperation opcOP, const bool value)
{
  byte port;
    
  OPCItemType aOPCItem = myArduinoMEGA.getOPCItem(itemID);                     

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
        pinMode(port, INPUT);
        digital_status_input[port] = opc_read;
      }

      return digitalRead(port);
    }
  } 
}

int readwrite_analog(const char *itemID, const opcOperation opcOP, const int value) 
{
  byte port;
    
  OPCItemType aOPCItem = myArduinoMEGA.getOPCItem(itemID);                     

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
        pinMode(port, INPUT);
        analog_status_input[port] = opc_read;
      }

      return analogRead(port);
    }
  } 
  
}

void setup() {
  Serial.begin(9600);
  
  myArduinoMEGA.setup(); 
   
  for (byte k=0;k<54;k++) digital_status_input[k] = opc_undefined; 
  for (byte k=0;k<16;k++) analog_status_input[k] = opc_undefined; 

  myArduinoMEGA.setup(); 
  
  myArduinoMEGA.addItem("D2",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoMEGA.addItem("D3",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoMEGA.addItem("D4",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoMEGA.addItem("D5",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoMEGA.addItem("D5",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoMEGA.addItem("D6",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoMEGA.addItem("D7",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoMEGA.addItem("D8",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoMEGA.addItem("D9",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoMEGA.addItem("D10",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoMEGA.addItem("D11",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoMEGA.addItem("D12",opc_readwrite, opc_bool, readwrite_digital);

  //builtin led
  myArduinoMEGA.addItem("D13",opc_write, opc_bool, readwrite_digital); 
  
  myArduinoMEGA.addItem("D22",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoMEGA.addItem("D23",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoMEGA.addItem("D24",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoMEGA.addItem("D25",opc_readwrite, opc_bool, readwrite_digital);  
  myArduinoMEGA.addItem("D26",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoMEGA.addItem("D27",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoMEGA.addItem("D28",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoMEGA.addItem("D29",opc_readwrite, opc_bool, readwrite_digital); 
  myArduinoMEGA.addItem("D30",opc_readwrite, opc_bool, readwrite_digital); 
  myArduinoMEGA.addItem("D31",opc_readwrite, opc_bool, readwrite_digital); 
  myArduinoMEGA.addItem("D32",opc_readwrite, opc_bool, readwrite_digital);  
  myArduinoMEGA.addItem("D33",opc_readwrite, opc_bool, readwrite_digital); 
  myArduinoMEGA.addItem("D34",opc_readwrite, opc_bool, readwrite_digital); 
  myArduinoMEGA.addItem("D35",opc_readwrite, opc_bool, readwrite_digital); 
  myArduinoMEGA.addItem("D36",opc_readwrite, opc_bool, readwrite_digital);
  myArduinoMEGA.addItem("D37",opc_readwrite, opc_bool, readwrite_digital); 
  myArduinoMEGA.addItem("D38",opc_readwrite, opc_bool, readwrite_digital); 
  myArduinoMEGA.addItem("D39",opc_readwrite, opc_bool, readwrite_digital); 
  myArduinoMEGA.addItem("D40",opc_readwrite, opc_bool, readwrite_digital); 
  myArduinoMEGA.addItem("D41",opc_readwrite, opc_bool, readwrite_digital); 
  myArduinoMEGA.addItem("D42",opc_readwrite, opc_bool, readwrite_digital); 
  myArduinoMEGA.addItem("D43",opc_readwrite, opc_bool, readwrite_digital); 
  myArduinoMEGA.addItem("D44",opc_readwrite, opc_bool, readwrite_digital); 
  myArduinoMEGA.addItem("D45",opc_readwrite, opc_bool, readwrite_digital); 
  myArduinoMEGA.addItem("D46",opc_readwrite, opc_bool, readwrite_digital); 
  myArduinoMEGA.addItem("D47",opc_readwrite, opc_bool, readwrite_digital); 
  myArduinoMEGA.addItem("D48",opc_readwrite, opc_bool, readwrite_digital); 
  myArduinoMEGA.addItem("D49",opc_readwrite, opc_bool, readwrite_digital);  
  myArduinoMEGA.addItem("D50",opc_readwrite, opc_bool, readwrite_digital); 
  myArduinoMEGA.addItem("D51",opc_readwrite, opc_bool, readwrite_digital); 
  myArduinoMEGA.addItem("D52",opc_readwrite, opc_bool, readwrite_digital);  
  myArduinoMEGA.addItem("D53",opc_readwrite, opc_bool, readwrite_digital);  

  myArduinoMEGA.addItem("A0",opc_readwrite, opc_int, readwrite_analog);
  myArduinoMEGA.addItem("A1",opc_readwrite, opc_int, readwrite_analog);
  myArduinoMEGA.addItem("A2",opc_readwrite, opc_int, readwrite_analog);
  myArduinoMEGA.addItem("A3",opc_readwrite, opc_int, readwrite_analog);
  myArduinoMEGA.addItem("A4",opc_readwrite, opc_int, readwrite_analog);
  myArduinoMEGA.addItem("A5",opc_readwrite, opc_int, readwrite_analog);
  myArduinoMEGA.addItem("A5",opc_readwrite, opc_int, readwrite_analog);
  myArduinoMEGA.addItem("A6",opc_readwrite, opc_int, readwrite_analog);
  myArduinoMEGA.addItem("A7",opc_readwrite, opc_int, readwrite_analog);
  myArduinoMEGA.addItem("A8",opc_readwrite, opc_int, readwrite_analog);
  myArduinoMEGA.addItem("A9",opc_readwrite, opc_int, readwrite_analog);
  myArduinoMEGA.addItem("A10",opc_readwrite, opc_int, readwrite_analog);
  myArduinoMEGA.addItem("A11",opc_readwrite, opc_int, readwrite_analog);
  myArduinoMEGA.addItem("A12",opc_readwrite, opc_int, readwrite_analog);
  myArduinoMEGA.addItem("A13",opc_readwrite, opc_int, readwrite_analog);
  myArduinoMEGA.addItem("A14",opc_readwrite, opc_int, readwrite_analog);
  myArduinoMEGA.addItem("A15",opc_readwrite, opc_int, readwrite_analog);
}

void loop() {
  myArduinoMEGA.processOPCCommands();
}
