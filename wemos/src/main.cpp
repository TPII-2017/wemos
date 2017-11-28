#include <Arduino.h>
#include <WebServer.h>
#include <Letter.h>

#define LETTERS_COUNT		1

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