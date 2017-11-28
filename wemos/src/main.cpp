#include <Arduino.h>
#include <WebServer.h>
#include <Letter.h>

void setup()
{
	Letter::init(LETTERS_COUNT);
	WebServer::init();
}

void loop()
{
	WebServer::tick();
	Letter::tick();
}