#include <OPC.h>
#include <Arduino.h>
#include <HardwareSerial.h>
#include <Ethernet.h>

/************************************* OPC */

OPC::OPC() : OPCItemList(NULL) ,  OPCItemsCount(0) {}

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

void OPCSerial::setup() {}

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

OPCNet::OPCNet() {}

void OPCNet::setup() {
  Bridge.begin();
  server.listenOnLocalhost();
  server.begin();
}

void OPCNet::sendOPCItemsMap()
{  
  buffer[0]='\0';
  strcat(buffer,"{\"M\":[");   // old ItemsMap tag
  
  for(int k=0;k<OPCItemsCount;k++) {
    if (k) strcat(buffer,",");    
    
    strcat(buffer,"{\"I\":\""); // old ItemId tag
    strcat(buffer,OPCItemList[k].itemID);
    strcat(buffer,"\",\"R\":\""); // old AccessRight tag
    
    bufPos = strlen(buffer);
    buffer[bufPos] = 48 + int(OPCItemList[k].opcAccessRight);
    buffer[bufPos+1] = '\0';

    strcat(buffer,"\",\"T\":\""); // old ItemType tag

    bufPos = strlen(buffer);
    buffer[bufPos] = 48 + int(OPCItemList[k].itemType);
    buffer[bufPos+1] = '\0';

    strcat(buffer,"\"}");  

    if (k==OPCItemsCount-1) strcat(buffer,"]");

    client.write((unsigned char *) buffer,strlen(buffer));
    buffer[0]='\0';
  }

  client.write((unsigned char *) "}",1);
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
          p = strtok_r(buffer,"=",&j);
          if (!j[0])  {
            for (int i = 0; i < OPCItemsCount; i++) {   
              if (!strcmp(buffer, OPCItemList[i].itemID))  {                             
                // Execute the stored handler function for the command  
                buffer[0] = '\0';
                strcat(buffer,"{\"I\":\""); // old ItemId tag
                strcat(buffer,OPCItemList[i].itemID);
                strcat(buffer,"\",\"V\":\""); // old ItemValue tag
                client.write((unsigned char *) buffer,strlen(buffer)); 
                  
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
                client.print(F("\"}"));

                matched = true;
                break;
              } /* end if */
            } /* end for */
          } /* end if */
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
                } /* end case */

                matched = true;
                break;
              } /* end if */
            } /* end for */
          } /* end else */
         }
        } /* end else */
      } /* end if */
               
    // Close connection and free resources.
  client.stop();
  }

  delay(50); // Poll every 50ms
}

/************************************* OPCEthernet */

OPCEthernet::OPCEthernet() {}

void OPCEthernet::after_setup(uint8_t listen_port)
{
  internal_ethernet_server = new EthernetServer(listen_port);   
  internal_ethernet_server->begin();  
}

int OPCEthernet::setup(uint8_t listen_port, uint8_t *mac_address)
{
  Ethernet.begin(mac_address); 
  after_setup(listen_port);
}

void OPCEthernet::setup(uint8_t listen_port, uint8_t *mac_address, IPAddress local_ip)
{  
  Ethernet.begin(mac_address,local_ip);
  after_setup(listen_port);
}

void OPCEthernet::setup(uint8_t listen_port, uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server)
{
  Ethernet.begin(mac_address,local_ip,dns_server);
  after_setup(listen_port);
}

void OPCEthernet::setup(uint8_t listen_port, uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway)
{
  Ethernet.begin(mac_address,local_ip,dns_server,gateway);  
  after_setup(listen_port);
}

void OPCEthernet::setup(uint8_t listen_port, uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet)
{
  Ethernet.begin(mac_address,local_ip,dns_server,gateway,subnet);    
  after_setup(listen_port);
}

void OPCEthernet::sendOPCItemsMap()
{ 
  buffer[0]='\0';
  strcat(buffer,"{\"M\":[");  // Old ItemsMap

  for(int k=0;k<OPCItemsCount;k++) {
    if (k) strcat(buffer,",");    
    
    strcat(buffer,"{\"I\":\""); // old ItemId tag
    strcat(buffer,OPCItemList[k].itemID);
    strcat(buffer,"\",\"R\":\""); // old AccessRight tag
    
    bufPos = strlen(buffer);
    buffer[bufPos] = 48 + int(OPCItemList[k].opcAccessRight);
    buffer[bufPos+1] = '\0';

    strcat(buffer,"\",\"T\":\""); // old ItemType tag

    bufPos = strlen(buffer);
    buffer[bufPos] = 48 + int(OPCItemList[k].itemType);
    buffer[bufPos+1] = '\0';

    strcat(buffer,"\"}");  

    if (k==OPCItemsCount-1) strcat(buffer,"]");

    client.write((unsigned char *) buffer,strlen(buffer));
    buffer[0]='\0';
  }

  client.write((unsigned char *) "}",1);
}

void OPCEthernet::processClientCommand()
{ 
  char *p,*j;
  bool matched = false;
  bool (*bool_callback)(const char *itemID, const opcOperation opcOP, const bool value);
  byte (*byte_callback)(const char *itemID, const opcOperation opcOP, const byte value);  
  int (*int_callback)(const char *itemID, const opcOperation opcOP, const int value);
  float (*float_callback)(const char *itemID, const opcOperation opcOP, const float value);  
  
  client.println(F("HTTP/1.1 200 OK\r\nContent-Type: text/json\r\nConnection: close\r\n"));

  if (!strcmp(buffer, "itemsmap")) {           
    sendOPCItemsMap();
  }   
  else
  { 
    p = strtok_r(buffer,"=",&j);
    if (!j[0])  {
      for (int i = 0; i < OPCItemsCount; i++) {   
        if (!strcmp(buffer, OPCItemList[i].itemID))  {                             
          // Execute the stored handler function for the command  
          client.print(F("{\"I\":\"")); // old ItemId tag
          client.print(buffer);
          client.print(F("\",\"V\":\"")); // old ItemValue tag
                  
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
          } /* end switch */
                
          client.print(F("\"}"));

          matched = true;
          break;
        } /* end if */
      } /* end for */
    } /* end if */
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
          } /* end case */

          matched = true;
          break;
        } /* end if */
      } /* end for */
    } /* end else */
  } /* end else */                
}

void OPCEthernet::processOPCCommands() 
{ 
  client = internal_ethernet_server->available(); 

  if (client) { 
    boolean currentLineIsBlank = true; 
      
    byte s = 0; 
    boolean responsed = false;

    while (!responsed && client.connected()) { 
      if (client.available()) {
        char c = client.read();       

        if (c == '\n' && currentLineIsBlank) {    
          processClientCommand();                       
          responsed = true;         
        }
        else if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {          
          currentLineIsBlank = false;
          
          switch (s) {
            case 0 : if (c == 'G') s++; break;
            case 1 : if (c == 'E') s++; else s = 0; break;
            case 2 : if (c == 'T') s++; else s = 0; break;
            case 3 : if (c == ' ') s++; else s = 0; break;
            case 4 : if (c == '/') { s++; bufPos = 0;} else s = 0; break;
            case 5 : if (c != ' ') {
                      buffer[bufPos++] = c;
                      buffer[bufPos] = '\0';
                      }
                      else s = 0;            
          }
        }
      }
    }
    delay(1); // Espera para dar tiempo al navegador a recibir los datos.
    client.stop(); // Cierra la conexión.
  }
}


