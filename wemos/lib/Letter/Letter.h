#pragma once

#include <Arduino.h>

#define LETTERS_COUNT		1

#define MAX_COLUMNS		8
#define MAX_LETTERS		32
#define MESSAGE_SIZE	6

class Letter {

public:

	enum type_t{noType = 0, message, map, predefined};
	enum predefined_t{noPredefined = 0, smile, pacman, newLife};

	static void init(uint8_t letterCount);

	// Setea el mensaje a mostrar. La variable 'stringLength' no incluye al 0.
	// Tampoco debe superar el máximo definido por MESSAGE_SIZE. Si eso sucede, 
	// el comportamiento es indefinido.
	static void setMessage(const char* message, uint8_t stringLength, int16_t slideRate);

	// La cantidad de columnas a setear debe ser inferior a MAX_COLUMNS * MAX_LETTERS. 
	// En caso de que columnsCount fuese inferior a mLetterCount * MAX_COLUMNS, se rellena, 
	// las columnas restantes con blanco. Si fuese superior, asegurarse que columnsCount sea 
	// un múltiplo de MAX_COLUMNS, en caso contrario, podría imprimirse basura en las columnas 
	// no seteadas.
	static void setMap(const uint8_t* columns, uint8_t columnsCount, int16_t slideRate);

	static void setPredefined(predefined_t pre, int16_t slideRate);

	static void tick();

private:

	static uint8_t mLetterCount;
	static uint8_t mCommandBuffer[MAX_COLUMNS * MAX_LETTERS];
	static type_t mType;
	static char mRaw[14];

	// El comando se envia a todos los MAX7219 conectados en el circuito. No se
	// debe utilizar para setear individualmente una columna de un MAX7219.
	static void sendCommand(uint8_t address, uint8_t value);

	// Limpia la pantalla de todos los MAX7219 conectados en el circuito.
	static void clearScreen();

	// Es el metodo que controla el cartel, cuando lo que se seteó fue un texto.
	static void messageTick();

	// Es el metodo que controla el cartel, cuando lo que se seteó fue un map.
	static void mapTick();

	static void predefinedTick();

};