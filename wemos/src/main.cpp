#include <Arduino.h>
#include "server.h"

void setup()
{
	Serial.begin(115200);
	server_setup();
	// TODO: matrix_controller_setup();
}

void loop()
{
	server_handler();
	//TODO: matrix_controller_handler();
}