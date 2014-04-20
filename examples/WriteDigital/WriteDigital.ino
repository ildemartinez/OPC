#include <OPC.h>
#include <Bridge.h>

/*
 * Declaring the OPC object
 */
OPCSerial aOPCSerial;

/*
 * set led status from OPC Client
 */
int ledPin = 13;

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
  aOPCSerial.setup();  
  
  /*
   * led OPCItem declaration
   */
  aOPCSerial.addItem("led",opc_read, opc_bool, callback);
}

void loop() {
  /*
   * OPC process commands
   */
  aOPCSerial.processOPCCommands();
}




