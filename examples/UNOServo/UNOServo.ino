#include <OPC.h>
#include <Bridge.h>
#include <Ethernet.h>
#include <SPI.h>
#include <Servo.h>

/*
 * Declaring the OPC object
 */
OPCSerial aOPCSerial;

/*
 * Declarin the servo information
 */
Servo myservo;  // create servo object to control a servo                

/*
 * create a callback function for the OPCItem
 */
int callback(const char *itemID, const opcOperation opcOP, const int value){
  static int pos;

  /*
   * if operation is a write command from OPC Client
   */
  if (opcOP == opc_opwrite) {
    pos = value;

    myservo.write(pos);
  } 
  else 
    /*
     * read the led status
     */
    return pos;  

}

void setup() {
  Serial.begin(9600);
  
  // Pin 9 to servo control
  myservo.attach(9);
  
  /*
   * OPC Object initialization
   */
  aOPCSerial.setup();  
  
  /*
   * led OPCItem declaration
   */
  aOPCSerial.addItem("servo",opc_readwrite, opc_int, callback);
}

void loop() {
  /*
   * OPC process commands
   */
  aOPCSerial.processOPCCommands();
}
