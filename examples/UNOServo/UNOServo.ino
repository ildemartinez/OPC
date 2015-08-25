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
Servo myservo1, myservo2;  // create servo object to control a servo                

const char servo1[] = "servo1";
const char servo2[] = "servo2";

/*
 * create a callback function for the OPCItem
 */
int callback(const char *itemID, const opcOperation opcOP, const int value){
  static int pos_servo1, pos_servo2;

  if (!strcmp(itemID,servo1))
  {
    /*
    * if operation is a write command from OPC Client
    */
    if (opcOP == opc_opwrite) {
      pos_servo1 = value;

      myservo1.write(pos_servo1);
    } 
    else 
      /*
      * read the led status
      */
      return pos_servo1;  
  }
  else if (!strcmp(itemID,servo2))
  {
    /*
    * if operation is a write command from OPC Client
    */
    if (opcOP == opc_opwrite) {
      pos_servo2 = value;

      myservo2.write(pos_servo2);
    } 
    else 
      /*
      * read the led status
      */
      return pos_servo2;  
  }

}

void setup() {
  Serial.begin(9600);
  
  // Pin 9 to servo control
  myservo1.attach(9);
  myservo2.attach(10);
  
  /*
   * OPC Object initialization
   */
  aOPCSerial.setup();  
  
  /*
   * led OPCItem declaration
   */
  aOPCSerial.addItem("servo1",opc_readwrite, opc_int, callback);
  aOPCSerial.addItem("servo2",opc_readwrite, opc_int, callback);  
}

void loop() {
  /*
   * OPC process commands
   */
  aOPCSerial.processOPCCommands();
}
