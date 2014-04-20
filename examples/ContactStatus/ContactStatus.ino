#include <OPC.h>

OPCSerial aOPCSerial;

int contactPin = 7;

bool item_bool(const char *itemID, const opcOperation opcOP, const bool value){
  if (digitalRead(contactPin) == HIGH) 
    return true;
  else
    return false;
}

void setup() {
  Serial.begin(9600);

  pinMode(contactPin, INPUT);
  
  aOPCSerial.setup();
  
  aOPCSerial.addItem("contact_status",opc_read, opc_bool, item_bool);
}

void loop() {
  aOPCSerial.processOPCCommands();
}

