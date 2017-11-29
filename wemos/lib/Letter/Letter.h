#pragma once

#include <Arduino.h>

// Constante que define la cantidad de matrices de 8x8 se conectan al sistema.
#define LETTERS_COUNT	1

// La cantidad de columnas que tiene cada matriz de LEDs.
#define MAX_COLUMNS		8
// La máxima cantidad de matrices que se pueden conectar en el sistema.
#define MAX_LETTERS		32
// El tamaño máximo del mensaje que se puede escribir. Este valor no incluye al
// caracter nulo.
#define MESSAGE_SIZE	6



// Esta clase es la encargada de controlar la matriz de LEDs, su principal
// función consiste en configurar cada MAX7219 que haya conectado. Luego se
// encarga de recibir los datos desde el servidor, procesarlos y armar un
// buffer de salida. Dicho arreglo de bytes son los que se envían vía SPI hacia
// los chips shifters.
class Letter {

public:

	// Tipo de enumerativo que se utiliza para indicar cuál sprite precargado
	// se quiere utilizar para representar.
	enum predefined_t{noPredefined = 0, smile, pacman, newLife};

	// Configura todos los MAX7219, y apaga todos los LEDs. Luego se queda a la
	// espera de las peticiones que vienen desde el servidor WEB.
	static void init();

	// Setea el mensaje a mostrar. La variable 'strlen' no incluye al 0.
	// Tampoco debe superar el máximo definido por MESSAGE_SIZE. Si eso sucede, 
	// el comportamiento es indefinido.
	static void setMessage(const char* message, uint8_t strlen, int16_t srate);

	// La cantidad de columnas a setear debe ser inferior a MAX_COLUMNS * 
	// MAX_LETTERS. En caso de que columnsCnt fuese inferior a mLetterCount *
	// MAX_COLUMNS, se rellena, las columnas restantes con blanco. Si fuese 
	// superior, debe asegurarse que columnsCnt sea un múltiplo de 
	// MAX_COLUMNS, en caso contrario, podría imprimirse basura en las columnas 
	// no seteadas.
	static void setMap(const uint8_t* cols, uint8_t columnsCnt, int16_t srate);

	// El predefinido que se establece debe ser del tipo predefined_t definido
	// en este archivo. En caso de que de alguna forma se enviara información
	// que no corresponda, este método no realiza ninguna función.
	static void setPredefined(predefined_t pre, int16_t slideRate);

	// Genera un tick al sistema, en base a la configuración de sus variables
	// privadas, determina si tiene que actualizar o no su buffer de salida. En
	// caso de que la respuesta fuese sí, entonces debe enviar nuevamente la
	// información vía SPI hacia los distintos MAX7219.
	static void tick();

private:

	// Este tipo de dato determina el tipo de petición que se ha recibido desde
	// el servidor WEB. En caso de no haber recibido ninguna, el valor por
	// defecto es noType.
	enum type_t{noType = 0, message, map, predefined};

	// La cantidad de matrices de LEDs de 8x8 conectadas en el sistema.
	static uint8_t mLetterCount;

	// El buffer de salida que se envía mediante SPI hacia los chips MAX7219.
	static uint8_t mCommandBuffer[MAX_COLUMNS * MAX_LETTERS];

	// El tipo de petición que se está procesando. En caso de no haber recibido
	// ninguna, el valor por defecto es noType.
	static type_t mType;

	// Un arreglo de bytes que posee toda la configuración necesearia que el 
	// controlador necesita.
	static char mRaw[14];

	// El comando se envia a todos los MAX7219 conectados en el circuito. No se
	// debe utilizar para setear individualmente una columna de un MAX7219.
	static void sendCommand(uint8_t address, uint8_t value);

	// Limpia la pantalla de todos los MAX7219 conectados en el circuito.
	static void clearScreen();

	// Es el método que controla el cartel, cuando lo que se seteó fue un
	// texto.
	static void messageTick();

	// Es el método que controla el cartel, cuando lo que se seteó fue un map.
	static void mapTick();

	// Es el método que controla el cartel, cuando lo que se seteó fue un 
	// sprite predefinido.
	static void predefinedTick();

};