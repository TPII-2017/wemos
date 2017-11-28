#include <Arduino.h>
#include <WebServer.h>
#include <Letter.h>

void setup()
{
	Letter::init(1);
	WebServer::init();
}

void loop()
{
	WebServer::tick();
	Letter::tick();
}