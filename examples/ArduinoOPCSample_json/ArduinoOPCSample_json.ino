/*
 * A sample Arduino sketch to use with the Arduino OPC DA Serial Server
 *
 * Steps:
 * 1 .- Include OPC Libraries
 * 2 .- Declare the OPC Object
 * 3 .- Code your feedback OPC Items functions
 * 4 .- Define your OPC Items 
 * 5 .- Process OPC Commands
 */
 
/*
 * 1 .- Include OPC Libraries
 */
#include <OPC.h>
#include <Bridge.h>
#include <Ethernet.h>
#include <SPI.h>

/*
 * 2.- Declaring the OPC object
 */
OPCNet aOPC;

/*
 * Some opcitem vars for playing
 */
int itembyteasc = 0;
float itemfloatasc = 0.01;

/*
 * 3 .- Feedback function shared between OPC Items
 */
bool item_bool(const char *itemID, const opcOperation opcOP, const bool value){
  if (opcOP == opc_opwrite) { }
  else {
    if (!strcmp(itemID, "item_bool_true")) return true; 
    else if (!strcmp(itemID, "item_bool_false")) return false; 
      else if (!strcmp(itemID, "item_bool_random")) return random(0,2);  
  }
}

byte item_byte(const char *itemID, const opcOperation opcOP, const byte value){
  return random(0,255);
}

byte item_byte_asc(const char *itemID, const opcOperation opcOP, const byte value){
  return itembyteasc++;
}

int item_int(const char *itemID, const opcOperation opcOP, const int value){
  return random(-32768,32768);
}

float item_float_asc(const char *itemID, const opcOperation opcOP, const float value){
  if (opcOP == opc_opwrite) { 
    Serial.println(value); 
  }
  else {
    itemfloatasc = itemfloatasc * -1.2 ;
    return itemfloatasc;
  }
}

void setup() {

  aOPC.setup();
  
  /*
   * 4 .- Declare your OPC Items  
   */
  
  // boolean OPC item
  // http://arduino.cc/en/Reference/BooleanVariables
  aOPC.addItem("item_bool_random",opc_read, opc_bool, item_bool);
  aOPC.addItem("item_bool_true",opc_read, opc_bool, item_bool);  
  aOPC.addItem("item_bool_false",opc_read, opc_bool, item_bool);    
  
  // byte OPC items
  // http://arduino.cc/en/Reference/Byte
  aOPC.addItem("item_byte_random",opc_write, opc_byte, item_byte);  
  aOPC.addItem("item_byte_asc",opc_read, opc_byte, item_byte_asc); 
  
  // int OPC items
  // http://arduino.cc/en/Reference/Int
  aOPC.addItem("item_int_random",opc_readwrite, opc_int, item_int);  
  
  // float OPC items
  // http://arduino.cc/en/Reference/Float
  aOPC.addItem("item_float_asc",opc_read, opc_float, item_float_asc);

}

void loop() {
  /*
   * 5 .- just do the OPC job
   */
  aOPC.processOPCCommands();
}

