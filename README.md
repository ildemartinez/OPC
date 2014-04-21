OPC Library for Arduino
=======================

OPC Library is an Arduino library that is used by the OPC Server for Arduino for sharing information but can be used alone.

It provide to you with a easy way to interchange information between Arduino and Serial or Ethernet ports (Wifi or Wired) in a unified way.

How to use it
-------------

First of all you have to have in mind what do you want provide to the external world using Serial, Ethernet or even Wifi.

At this moment you can publish your items using serial or http ways (wifi and wired). To do this you have some objects that you can use.

Declare 
```
OPCSerial aOPC; 
```
for serial use or for http use:
```
OPCNet aOPC;
```

Declare your items
------------------

An item can be a real or virtual input/output pin of your Arduino. For example, you would want to publish your analog input A0 to the external world or a virtual value based in some operations made with your Arduino using real pins or calculations.

You can declare your item this way:

```
aOPC.addItem("random",opc_read, opc_int, callback);
````

where the first param declares your item name
second one tell to the library if is read, read/write or write only item (valid values are opc_read, opc_write, opc_readwrite)
third is the type of the item you are declaring (opc_bool, opc_byte, opc_int, opc_float)
last is the callback function. The callback will be executed when external client want to read/write a item so you can return or store the desired value.

Here you can see a complete example

````
#include <OPC.h>

/*
 * Declaring the OPC object
 */
OPCSerial aOPCSerial;

/*
 * We want to publish analogPin 3 to OPC Client
 */
int analogPin = 3;

/*
 * create a callback function for the OPCItem
 */
int callback(const char *itemID, const opcOperation opcOP, const int value){
  return analogRead(analogPin);
}

void setup() {
  Serial.begin(9600);
  
  /*
   * OPC Object initialization
   */
  aOPCSerial.setup();
  
  /*
   * A0 OPCItem declaration
   */
  aOPCSerial.addItem("A0",opc_read, opc_int, callback);
}

void loop() {
  /*
   * OPC process commands
   */
  aOPCSerial.processOPCCommands();
}
````

You can upload this code and open a terminal. If you send intro to your Arduino it will return all items declared, if you send the name of declared item you will get the value returned by the callback function.

Here you could find more information and a videotutorial about this topic http://www.st4makers.com/arduino-opc-server/installation-guides/easy-testing-of-your-opc-sketch


At this moment you can visit the project page at http://www.st4makers.com and download it.

Here you can find the libraries and samples sketchs to start learning and get fun.

