/*
 * A generic sketch for use with the Arduino OPC DA Serial Server
 */
#include <OPC.h>
#include <Bridge.h>

OPCSerial aOPCSerial;

bool readwrite_digital(const char *itemID, const opcOperation opcOP, const bool value){
  byte port;
  
  if (!strcmp(itemID, "D2")) port = 2;
  if (!strcmp(itemID, "D13")) port = 13;  
  
  if (opcOP == opc_opwrite) {
      pinMode(port,OUTPUT);
      Serial.println("write");
      digitalWrite(port,value);  
    }
  else
  {
    pinMode(port,INPUT);   
 Serial.println("read");   
    return digitalRead(port); 
  }  

}

void setup() {
  Serial.begin(9600);

  aOPCSerial.setup(); 
  aOPCSerial.addItem("D2",opc_readwrite, opc_bool, readwrite_digital);
  aOPCSerial.addItem("D13",opc_readwrite, opc_bool, readwrite_digital);
}

void loop() {
  aOPCSerial.processOPCCommands();
}

