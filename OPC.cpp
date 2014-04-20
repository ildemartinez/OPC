#include <OPC.h>
#include <Arduino.h>
#include <HardwareSerial.h>

/************************************* OPC */

OPC::OPC() : OPCItemList(NULL) ,  OPCItemsCount(0) {
}

void OPC::addItem(const char *itemID, opcAccessRights opcAccessRight, opctypes opctype, bool (*function)(const char *itemID, const opcOperation opcOP, const bool value))
{ 
  internaladdItem(itemID, opcAccessRight, opctype, int(function));
}

void OPC::addItem(const char *itemID, opcAccessRights opcAccessRight, opctypes opctype, byte (*function)(const char *itemID, const opcOperation opcOP, const byte value))
{
  internaladdItem(itemID, opcAccessRight, opctype, int(function));  
}

void OPC::addItem(const char *itemID, opcAccessRights opcAccessRight, opctypes opctype, int (*function)(const char *itemID, const opcOperation opcOP, const int value))
{ 
  internaladdItem(itemID, opcAccessRight, opctype, int(function));
}

void OPC::addItem(const char *itemID, opcAccessRights opcAccessRight, opctypes opctype, float  (*function)(const char *itemID, const opcOperation opcOP, const float value))
{
  internaladdItem(itemID, opcAccessRight, opctype, int(function));
}

void OPC::internaladdItem(const char *itemID, opcAccessRights opcAccessRight, opctypes opctype, int callback_function)  
{
  //extern int __heap_start, *__brkval; 
  //int v; 
  //Serial.print(F("Free memory:"));
  //Serial.println( (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval));
  
  OPCItemList = (OPCItemType *) realloc(OPCItemList, (OPCItemsCount + 1) * sizeof(OPCItemType));
  if (OPCItemList != NULL) {
    OPCItemList[OPCItemsCount].itemType = opctype; 

    OPCItemList[OPCItemsCount].itemID = (char *) malloc(strlen(itemID)+1);
    strncpy(&OPCItemList[OPCItemsCount].itemID[0], itemID, strlen(itemID)+1);

    OPCItemList[OPCItemsCount].opcAccessRight = opcAccessRight; 
    OPCItemList[OPCItemsCount].ptr_callback = callback_function;
    OPCItemsCount++;
  } else {
     Serial.println(F("Not enough memory")); 
  }
  
}

/************************************* OPCSerial */

void OPCSerial::setup()
{
}

void OPCSerial::sendOPCItemsMap()
{ 
  Serial.print(F("<0"));

  for(int k=0;k<OPCItemsCount;k++) {    
    Serial.print(F(","));
    Serial.print(OPCItemList[k].itemID);
    Serial.print(F(","));
    Serial.print(int(OPCItemList[k].opcAccessRight));
    Serial.print(F(","));
    Serial.print(int(OPCItemList[k].itemType));
  }

  Serial.println(F(">"));
}

OPCSerial::OPCSerial()  {
    buffer[0] = '\0';
}

void OPCSerial::processOPCCommands() {
  bool matched = false;
  char *p,*j;

  bool (*bool_callback)(const char *itemID, const opcOperation opcOP, const bool value);
  byte (*byte_callback)(const char *itemID, const opcOperation opcOP, const byte value);  
  int (*int_callback)(const char *itemID, const opcOperation opcOP, const int value);
  float (*float_callback)(const char *itemID, const opcOperation opcOP, const float value);  

  while (Serial.available() > 0) {
    char inChar = Serial.read(); 
    
    if (inChar == '\r') {     
      if (buffer[0] == '\0') 
        sendOPCItemsMap();
      else {
        // Lets search for read
        for (int i = 0; i < OPCItemsCount; i++) {            
         if (!strncmp(buffer, OPCItemList[i].itemID, SERIALCOMMAND_MAXCOMMANDLENGTH)) {                      
                        
          // Execute the stored handler function for the command  
          switch (OPCItemList[i].itemType) {
            case opc_bool :
                      bool_callback = (bool (*)(const char *itemID, const opcOperation opcOP, const bool value))(OPCItemList[i].ptr_callback);
                      Serial.println(bool_callback(OPCItemList[i].itemID,opc_opread,NULL));                      
                      break;
            case opc_byte :
                      byte_callback = (byte (*)(const char *itemID, const opcOperation opcOP, const byte value))(OPCItemList[i].ptr_callback);
                      Serial.println(byte_callback(OPCItemList[i].itemID,opc_opread,NULL));
                      break;
            case opc_int : 
                      int_callback = (int (*)(const char *itemID, const opcOperation opcOP, const int value))(OPCItemList[i].ptr_callback);
                      Serial.println(int_callback(OPCItemList[i].itemID,opc_opread,NULL));
                      break;
            case opc_float : 
                      float_callback = (float (*)(const char *itemID, const opcOperation opcOP, const float value))(OPCItemList[i].ptr_callback);
                      Serial.println(float_callback(OPCItemList[i].itemID,opc_opread,NULL));
                      break;                      
          }          

          matched = true;
          break;
          } /* endif */
        } /* endfor */

        if (!matched) {
          // Lets search for write
          p = strtok_r(buffer,"=",&j);
          for (int i = 0; i < OPCItemsCount; i++) {            
            if (!strncmp(p, OPCItemList[i].itemID, SERIALCOMMAND_MAXCOMMANDLENGTH)) {                      

              // Call the stored handler function for the command                          
              switch (OPCItemList[i].itemType) {
              case opc_bool :
                      bool_callback = (bool (*)(const char *itemID, const opcOperation opcOP, const bool value))(OPCItemList[i].ptr_callback);
                      bool_callback(OPCItemList[i].itemID,opc_opwrite,atoi(j));
                      break;
              case opc_byte :
                      byte_callback = (byte (*)(const char *itemID, const opcOperation opcOP, const byte value))(OPCItemList[i].ptr_callback);
                      byte_callback(OPCItemList[i].itemID,opc_opwrite,atoi(j));
                      break;
              case opc_int : 
                      int_callback = (int (*)(const char *itemID, const opcOperation opcOP, const int value))(OPCItemList[i].ptr_callback);
                      int_callback(OPCItemList[i].itemID,opc_opwrite,atoi(j));
                      break;
              case opc_float : 
                      float_callback = (float (*)(const char *itemID, const opcOperation opcOP, const float))(OPCItemList[i].ptr_callback);
                      float_callback(OPCItemList[i].itemID,opc_opwrite,atof(j));
                      break;                      
              } 

              break;
            } /* endif */         
          } /* endfor */
        }
      }

      buffer[0] = '\0';
      bufPos = 0;
    }
    else {
      if (bufPos < SERIALCOMMAND_BUFFER) {
        buffer[bufPos++] = inChar;
        buffer[bufPos] = '\0';
      }
    }
  }
}

/************************************* OPCNet */

OPCNet::OPCNet() {
      
}

void OPCNet::setup() {
  Bridge.begin();
  server.listenOnLocalhost();
  server.begin();
}

void OPCNet::sendOPCItemsMap()
{ 
  client.print(F("["));

  for(int k=0;k<OPCItemsCount;k++) {
    if (k) client.print(F(","));    
    client.print(F("{"));
    client.print("\"ItemId\":"); 
    client.print("\""); client.print(OPCItemList[k].itemID); client.print("\"");
    client.print(",\"AccessRight\":");
    client.print("\""); client.print(int(OPCItemList[k].opcAccessRight)); client.print("\"");
    client.print(",\"ItemType\":");
    client.print("\"");client.print(int(OPCItemList[k].itemType));    client.print("\"");
    client.print(F("}"));
  }

  client.print(F("]"));
}

void OPCNet::processOPCCommands() {
  char *p,*j;
  bool matched = false;
  bool (*bool_callback)(const char *itemID, const opcOperation opcOP, const bool value);
  byte (*byte_callback)(const char *itemID, const opcOperation opcOP, const byte value);  
  int (*int_callback)(const char *itemID, const opcOperation opcOP, const int value);
  float (*float_callback)(const char *itemID, const opcOperation opcOP, const float value);  

 client = server.accept();

 if (client) {
  //String command = client.readStringUntil('/');
  bufPos = 0;
  while (client.available())
    buffer[bufPos++] = client.read();

  if (bufPos > 2) { // avoid 13 10 chars
    buffer[bufPos-2] = '\0';

    p = strtok_r(buffer,"/",&j);

    if (!j[0]) {
      if (!strcmp(buffer, "itemsmap")) { 
       sendOPCItemsMap();
      }   
      else
      { 
        for (int i = 0; i < OPCItemsCount; i++) {   
        if (!strcmp(buffer, OPCItemList[i].itemID))  {                             
            // Execute the stored handler function for the command  
            client.print(F("[{\"ItemId\":\"")); 
            client.print(buffer); 
            client.print(F("\",\"ItemValue\":\""));  
              
            switch (OPCItemList[i].itemType) {  
              case opc_bool :
                        bool_callback = (bool (*)(const char *itemID, const opcOperation opcOP, const bool value))(OPCItemList[i].ptr_callback);
                        client.print(bool_callback(OPCItemList[i].itemID,opc_opread,NULL));                      
                        break;
              case opc_byte :
                        byte_callback = (byte (*)(const char *itemID, const opcOperation opcOP, const byte value))(OPCItemList[i].ptr_callback);
                        client.print(byte_callback(OPCItemList[i].itemID,opc_opread,NULL));
                        break;
              case opc_int : 
                        int_callback = (int (*)(const char *itemID, const opcOperation opcOP, const int value))(OPCItemList[i].ptr_callback);
                        client.print(int_callback(OPCItemList[i].itemID,opc_opread,NULL));
                        break;
              case opc_float : 
                        float_callback = (float (*)(const char *itemID, const opcOperation opcOP, const float value))(OPCItemList[i].ptr_callback);
                        client.print(float_callback(OPCItemList[i].itemID,opc_opread,NULL));
                        break;                      
            }          
            client.print(F("\"}]"));

            matched = true;
            break;
          }
        } 
      } 
    }
    else
    {
      for (int i = 0; i < OPCItemsCount; i++) {   
        if (!strcmp(buffer, OPCItemList[i].itemID))  {                                    

            // Call the stored handler function for the command                          
            switch (OPCItemList[i].itemType) {
            case opc_bool :
                    bool_callback = (bool (*)(const char *itemID, const opcOperation opcOP, const bool value))(OPCItemList[i].ptr_callback);
                    bool_callback(OPCItemList[i].itemID,opc_opwrite,atoi(j));
                    break;
            case opc_byte :
                    byte_callback = (byte (*)(const char *itemID, const opcOperation opcOP, const byte value))(OPCItemList[i].ptr_callback);
                    byte_callback(OPCItemList[i].itemID,opc_opwrite,atoi(j));
                    break;
            case opc_int : 
                    int_callback = (int (*)(const char *itemID, const opcOperation opcOP, const int value))(OPCItemList[i].ptr_callback);
                    int_callback(OPCItemList[i].itemID,opc_opwrite,atoi(j));
                    break;
            case opc_float : 
                    float_callback = (float (*)(const char *itemID, const opcOperation opcOP, const float))(OPCItemList[i].ptr_callback);
                    float_callback(OPCItemList[i].itemID,opc_opwrite,atof(j));
                    break;                      
            } 
            matched = true;
            break;
          }
        } 
    }  
  }               
               
    // Close connection and free resources.
  client.stop();
  }

  delay(50); // Poll every 50ms
}



