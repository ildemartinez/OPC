#include <OPC.h>
#include <Bridge.h>
#include <Ethernet.h>
#include <SPI.h>

/*
 * Declaring the OPC object
 */
OPCNet myYun;

/*
 * set led status from OPC Client
 */
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
  
  /*
   * OPC Object initialization
   */
  myYun.setup();  
  
  /*
   * OPCItems declaration
   */
  myYun.addItem("D0",opc_readwrite, opc_bool, readwrite_digital);
  myYun.addItem("D1",opc_readwrite, opc_bool, readwrite_digital);
  myYun.addItem("D2",opc_readwrite, opc_bool, readwrite_digital);
  myYun.addItem("D3",opc_readwrite, opc_bool, readwrite_digital);
  myYun.addItem("D4",opc_readwrite, opc_bool, readwrite_digital);
  myYun.addItem("D5",opc_readwrite, opc_bool, readwrite_digital);
  myYun.addItem("D5",opc_readwrite, opc_bool, readwrite_digital);
  myYun.addItem("D6",opc_readwrite, opc_bool, readwrite_digital);
  myYun.addItem("D7",opc_readwrite, opc_bool, readwrite_digital);
  myYun.addItem("D8",opc_readwrite, opc_bool, readwrite_digital);
  myYun.addItem("D9",opc_readwrite, opc_bool, readwrite_digital);
  myYun.addItem("D10",opc_readwrite, opc_bool, readwrite_digital);
  myYun.addItem("D11",opc_readwrite, opc_bool, readwrite_digital);
  myYun.addItem("D12",opc_readwrite, opc_bool, readwrite_digital);
  myYun.addItem("D13",opc_readwrite, opc_bool, readwrite_digital);
  
  myYun.addItem("A0",opc_readwrite, opc_int, readwrite_analog);
  myYun.addItem("A1",opc_readwrite, opc_int, readwrite_analog);
  myYun.addItem("A2",opc_readwrite, opc_int, readwrite_analog);
  myYun.addItem("A3",opc_readwrite, opc_int, readwrite_analog);
  myYun.addItem("A4",opc_readwrite, opc_int, readwrite_analog);
  myYun.addItem("A5",opc_readwrite, opc_int, readwrite_analog); 
}

void loop() {
  /*
   * OPC process commands
   */
  myYun.processOPCCommands();
}




