/*
 * A generic sketch for use with the Arduino OPC DA Serial Server
 */
#include <OPC.h>

OPCSerial aOPCSerial;

bool readwrite_digital(const char *itemID, const opcOperation opcOP, const bool value){
  byte port;
  
  if (!strncmp(itemID, "D2",MAXOPCITEMNAME)) port = 2;
  if (!strncmp(itemID, "D13",MAXOPCITEMNAME)) port = 13;  
  
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


