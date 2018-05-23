#include "OPC.h"
#include <Bridge.h>
#include <Ethernet.h>
#include <SPI.h>

OPCNodeMcu myNodeMcu;

opcAccessRights digital_status_input[11], analog_status_input[1];

bool readwrite_digital(const char *itemID, const opcOperation opcOP, const bool value)
{
	byte port;

	OPCItemType aOPCItem = myNodeMcu.getOPCItem(itemID);

	port = atoi(&itemID[1]);

	if (opcOP == opc_opwrite) {
		if ((aOPCItem.opcAccessRight == opc_write) || (aOPCItem.opcAccessRight == opc_readwrite)) {

			if (digital_status_input[port] != opc_write) {
				pinMode(port, OUTPUT);
				digital_status_input[port] = opc_write;
			}

			digitalWrite(port, value);
		}
	}

	if (opcOP == opc_opread) {
		if ((aOPCItem.opcAccessRight == opc_read) || (aOPCItem.opcAccessRight == opc_readwrite)) {

			if (digital_status_input[port] != opc_read) {
				//pinMode(port, INPUT);
				digital_status_input[port] = opc_read;
			}

			return digitalRead(port);
		}
	}
}

int readwrite_analog(const char *itemID, const opcOperation opcOP, const int value)
{
	byte port;

	OPCItemType aOPCItem = myNodeMcu.getOPCItem(itemID);

	port = atoi(&itemID[1]);

	if (opcOP == opc_opwrite) {
		if ((aOPCItem.opcAccessRight == opc_write) || (aOPCItem.opcAccessRight == opc_readwrite)) {

			if (analog_status_input[port] != opc_write) {
				pinMode(port, OUTPUT);
				analog_status_input[port] = opc_write;
			}

			analogWrite(port, value);
		}
	}

	if (opcOP == opc_opread) {
		if ((aOPCItem.opcAccessRight == opc_read) || (aOPCItem.opcAccessRight == opc_readwrite)) {

			if (analog_status_input[port] != opc_read) {
				//pinMode(port, INPUT);
				analog_status_input[port] = opc_read;
			}

			return analogRead(port);
		}
	}

}

void setup() {
	byte k;

	Serial.begin(9600);

	for (k = 0; k<11; k++) digital_status_input[k] = opc_undefined;
	for (k = 0; k<1; k++) analog_status_input[k] = opc_undefined;

	//myNodeMcu.setup("NETGEAR98", "luckyshoe351", 80);
	myNodeMcu.setup("Internet_57", "123456789", 80);
	myNodeMcu.addItem("D0", opc_readwrite, opc_bool, readwrite_digital);
	myNodeMcu.addItem("D1", opc_readwrite, opc_bool, readwrite_digital);
	myNodeMcu.addItem("D2", opc_readwrite, opc_bool, readwrite_digital);
	myNodeMcu.addItem("D3", opc_readwrite, opc_bool, readwrite_digital);
	myNodeMcu.addItem("D4", opc_readwrite, opc_bool, readwrite_digital);
	myNodeMcu.addItem("D5", opc_readwrite, opc_bool, readwrite_digital);
	myNodeMcu.addItem("D5", opc_readwrite, opc_bool, readwrite_digital);
	myNodeMcu.addItem("D6", opc_readwrite, opc_bool, readwrite_digital);
	myNodeMcu.addItem("D7", opc_readwrite, opc_bool, readwrite_digital);
	myNodeMcu.addItem("D8", opc_readwrite, opc_bool, readwrite_digital);
	myNodeMcu.addItem("D9", opc_readwrite, opc_bool, readwrite_digital);
	myNodeMcu.addItem("D10", opc_readwrite, opc_bool, readwrite_digital);

	myNodeMcu.addItem("A0", opc_readwrite, opc_int, readwrite_analog);

}

void loop() {
	myNodeMcu.processOPCCommands();
}
