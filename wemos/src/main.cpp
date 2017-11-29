#include <Arduino.h>
#include <WebServer.h>
#include <Letter.h>


// Punto de entrada, solo se ejecuta una vez. Se inicializan los componentes.
void setup()
{
	Letter::init();
	WebServer::init();
}

// Loop principal del programa, nunca retorna, las operaciones que acá se hagan
// deben ser no bloqueantes.
void loop()
{
	WebServer::tick();
	Letter::tick();
}