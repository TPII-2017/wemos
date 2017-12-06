#include <Letter.h>
#include <SPI.h>
#include <Font.h>

// La cantidad de milisegundos que debe esperarse hasta ejecutarse un nuevo
// tick del sistema.
#define TICKS	10


// Registros del MAX7219.
static constexpr uint8_t MAX7219_REG_NOOP		= 0x0;
static constexpr uint8_t MAX7219_REG_DIGIT0		= 0x1;
static constexpr uint8_t MAX7219_REG_DIGIT1		= 0x2;
static constexpr uint8_t MAX7219_REG_DIGIT2		= 0x3;
static constexpr uint8_t MAX7219_REG_DIGIT3		= 0x4;
static constexpr uint8_t MAX7219_REG_DIGIT4 	= 0x5;
static constexpr uint8_t MAX7219_REG_DIGIT5		= 0x6;
static constexpr uint8_t MAX7219_REG_DIGIT6		= 0x7;
static constexpr uint8_t MAX7219_REG_DIGIT7		= 0x8;
static constexpr uint8_t MAX7219_REG_DECODEMODE	= 0x9;
static constexpr uint8_t MAX7219_REG_INTENSITY	= 0xA;
static constexpr uint8_t MAX7219_REG_SCANLIMIT	= 0xB;
static constexpr uint8_t MAX7219_REG_SHUTDOWN	= 0xC;
static constexpr uint8_t MAX7219_REG_TESTMODE	= 0xF;

// La estructura que se utiliza para almacenar la información de configuración
// cuando la petición corresponde a una frase.
struct text_t {
	// No incluye al caracter 0.
	uint8_t textLength;
	// El mensaje incluyendo al caracter 0.
	char message[MESSAGE_SIZE + 1];
	// Representa la última letra a dibujar. Va desde 0 hasta textLength 
	// exclusive.
	uint8_t letterIndex;
	// Representa la última columna de la letra a dibujar. Va desde 0 hasta 
	// MAX_COLUMNS exclusive.
	uint8_t columnIndex;
}__attribute__((packed));


// La estructura que se utiliza para almacenar la información de configuración
// cuando la petición corresponde a una mapeo de leds.
struct map_t {
	// La cantidad de columnas que tiene el mapeo.
	uint8_t columnsCount;
	// Representa la primera columna que se dibuja. Va desde 0 hasta columnsCount 
	// exclusive.
	uint8_t columnIndex;
}__attribute__((packed));

// La estructura que se utiliza para almacenar la información de configuración
// cuando la petición corresponde a un sprite predefinido.
struct predefined_t {
	// Determina cual animación se está representando.
	uint8_t sprite;
	// Determina cuál sprite de la animación se está representando.
	uint8_t spriteIndex;
	// Determina cuantos sprites tiene la animación seleccionada.
	uint8_t spritesCount;
	// Determina la cantidad de ms que debe pasar hasta cambiar el indice.
	int16_t fpms;
	// Contador de ticks, que determina cuantos ticks faltan para ejecutar un
	// refrescado.
	int16_t remainingRefreshTicks;
	// La cantidad de columnas que tiene la animación.
	uint8_t columnsCount;
	// Representa la primera columna que se dibuja. Va desde 0 hasta 
	// columnsCount exclusive.
	uint8_t columnIndex;
}__attribute__((packed));

struct party_t {
	// Flag que determina si esta encendido o apagado.
	uint8_t on;
	// La cantidad de ticks restantes para tooglear.
	int16_t remainingTicks;
	// El valor por defecto.
	int16_t ticks;
}__attribute__((packed));


// La estructura base, que contiene información de configuración general a
// todas las peticiones.
struct base_t {
	// El valor esta en ms. El signo determina hacia donde se mueve. Negativo
	// implica hacia la izquierda.
	int16_t srate;
	// Contador de ticks, que determina cuantos ticks faltan para ejecutar un
	// slide.
	int16_t remainingSlideTicks;
	union {
		text_t text;
		map_t map;
		predefined_t predefined;
		party_t party;
	};
}__attribute__((packed));

static_assert(sizeof(base_t) <= RAW_DATA_SIZE, "Error en la longitud de rawData");


uint8_t 		Letter::mLetterCount;
uint8_t			Letter::mCommandBuffer[MAX_COLUMNS * MAX_LETTERS];
char			Letter::mRaw[RAW_DATA_SIZE];
Letter::type_t	Letter::mType;
bool			Letter::mEnabled;
uint8_t			Letter::mIntensity;

// Realiza la copia desde el puntero src, hasta dst, indicando cuantos bytes se
// quieren copiar. Mientras se realiza la copia, se busca en src un terminador 
// 0. En caso de que no se encuentre se coloca en dst un terminador 0 al inicio.
static int strcpy_s(char *dest, unsigned int dmax, const char *src);

// Hace la cuenta del valor que se pasa por referencia. Si excede el máximo
// inicia en 0 nuevamente. En cambio si restando se llega por debajo de 0,
// la cuenta arranca en maxValue - 1.
static void countWithModule1(uint8_t &value, uint8_t maxValue, bool ascnd);

// Realiza dos cuentas con módulo recursivamente.
static void countWithModule2(uint8_t &s, uint8_t mxs, uint8_t &mn, uint8_t mxmn, bool ascnd);


void Letter::init()
{
	mLetterCount = LETTERS_COUNT;
	pinMode(SS, OUTPUT);
	SPI.begin();
	sendCommand(MAX7219_REG_TESTMODE, 0x00);	// No test mode
	sendCommand(MAX7219_REG_SHUTDOWN, 0x00); 	// Shutdown
	sendCommand(MAX7219_REG_DECODEMODE, 0x00); 	// No decode
	setIntensity(0x0F);							// High intensity
	sendCommand(MAX7219_REG_SCANLIMIT, 0x07); 	// Scan all columns
	clearScreen();
	sendCommand(MAX7219_REG_SHUTDOWN, 0x01);	// Turn on
	randomSeed(analogRead(0));
	mType = noType;
	mEnabled = true;
}

void Letter::sendCommand(uint8_t address, uint8_t value)
{
	digitalWrite(SS, LOW);
	for (uint8_t i = 0; i < mLetterCount; i++) {
		SPI.transfer(address);
		SPI.transfer(value);
	}
	digitalWrite(SS, HIGH); 
}

void Letter::clearScreen()
{
	for(uint8_t i = 0; i < MAX_COLUMNS; i++) {
		sendCommand(MAX7219_REG_DIGIT0 + i, 0x00);
	}
	mType = noType;
}

void Letter::setMessage(const char* message, uint8_t strLen, int16_t srate)
{
	if (!mEnabled)
		return;

	base_t* base = reinterpret_cast<base_t*>(mRaw);
	strcpy_s(base->text.message, strLen + 1, message);

	uint8_t maxSize = (strLen > mLetterCount) ? strLen : mLetterCount;

	for(int8_t j = strLen; j < maxSize; j++)
		base->text.message[j] = ' ';
	base->text.message[maxSize] = 0;
	
	base->srate = srate;
	base->remainingSlideTicks = 0;
	base->text.textLength = maxSize;
	base->text.letterIndex = mLetterCount - 1;
	base->text.columnIndex = MAX_COLUMNS - 1;

	mType = type_t::message;
}

void Letter::setMap(const uint8_t* cols, uint8_t colCnt, int16_t srate)
{
	if (!mEnabled)
		return;

	for(uint8_t i = 0; i < colCnt; i++) {
		mCommandBuffer[i] = cols[i];
	}
	uint8_t maxColumns = mLetterCount * MAX_COLUMNS;
	for(uint8_t i = colCnt; i < maxColumns; i++) {
		mCommandBuffer[i] = 0x00;
	}
	
	base_t* base = reinterpret_cast<base_t*>(mRaw);
	base->srate = srate;
	base->remainingSlideTicks = 0;
	base->map.columnsCount = (colCnt < maxColumns) ? maxColumns : colCnt;
	base->map.columnIndex = 0;

	mType = type_t::map;
}

void Letter::setPredefined(Letter::predefined_t pre, int16_t srate)
{
	if (!mEnabled)
		return;

	if (pre == noPredefined) {
		mType = type_t::noType;
		return;
	}

	base_t* base = reinterpret_cast<base_t*>(mRaw);
	base->predefined.sprite = predefined_config[pre - 1][2];
	base->predefined.spriteIndex = 0;
	base->predefined.spritesCount = predefined_config[pre - 1][0];
	base->predefined.fpms = predefined_config[pre - 1][1];
	base->predefined.remainingRefreshTicks = 0;
	base->remainingSlideTicks = (srate < 0) ? -srate : srate;
	base->predefined.columnsCount = mLetterCount * MAX_COLUMNS;
	base->predefined.columnIndex = 0;
	base->srate = srate;

	mType = type_t::predefined;
}

void Letter::setPartyOn()
{
	if (!mEnabled)
		return;
		
	if (mType == type_t::party)
		return;

	base_t* base = reinterpret_cast<base_t*>(mRaw);
	base->party.on = 0;
	base->party.remainingTicks = 0;
	base->party.ticks = 100;

	mType = type_t::party;
}

void Letter::setEnabled(bool enabled)
{
	mEnabled = enabled;
}

bool Letter::isEnabled()
{
	return mEnabled;
}

void Letter::setIntensity(uint8_t intensity)
{
	sendCommand(MAX7219_REG_INTENSITY, intensity & 0x0F);
	mIntensity = intensity & 0x0F;
}

uint8_t Letter::getIntensity()
{
	return mIntensity;
}

void Letter::tick()
{
	switch(mType) {
	case type_t::message:
		messageTick();
		break;
	
	case type_t::map:
		mapTick();
		break;

	case type_t::predefined:
		predefinedTick();
		break;

	case type_t::party:
		partyTick();
		break;

	default:
		break;
	}

	delay(TICKS);
}

void Letter::messageTick()
{
	base_t* base = reinterpret_cast<base_t*>(mRaw);

	base->remainingSlideTicks -= TICKS;
	if (base->remainingSlideTicks > 0)
		return;

	base->remainingSlideTicks = (base->srate < 0) ? -base->srate : base->srate;

	uint8_t i = base->text.columnIndex;
	uint8_t I = base->text.letterIndex;

	for (uint8_t j = 0; j < mLetterCount * MAX_COLUMNS; j++) {
		mCommandBuffer[j] = font[base->text.message[I] * MAX_COLUMNS + i]; 
		countWithModule2(i, MAX_COLUMNS, I, base->text.textLength, false);		
	}

	for (uint8_t j = 1; j <= MAX_COLUMNS; j++) {
		digitalWrite(SS, LOW);
		
		for (uint8_t k = 0; k < mLetterCount; k++) {
			uint16_t word = ((MAX_COLUMNS - (j - 1)) << 8);
			uint8_t value = mCommandBuffer[(j - 1) + k * MAX_COLUMNS];
			SPI.transfer16(word | value & 0x00FF);
		}
		
		digitalWrite(SS, HIGH);
	}

	if (base->srate) {
		countWithModule2(base->text.columnIndex, MAX_COLUMNS, 
						base->text.letterIndex, base->text.textLength, 
						(base->srate < 0));
	} else {
		mType = type_t::noType;
	}
	
}

void Letter::mapTick()
{
	base_t* base = reinterpret_cast<base_t*>(mRaw);

	base->remainingSlideTicks -= TICKS;
	if (base->remainingSlideTicks > 0)
		return;
	
	base->remainingSlideTicks = (base->srate < 0) ? -base->srate : base->srate;

	for (uint8_t j = 1; j <= MAX_COLUMNS; j++) {
		digitalWrite(SS, LOW);
		
		for (int8_t k = mLetterCount - 1; k >= 0; k--) {
			uint16_t word = (j << 8);
			uint8_t value = mCommandBuffer[((j - 1) + k * MAX_COLUMNS + 
					base->map.columnIndex ) % base->map.columnsCount];

			SPI.transfer16(word | value & 0x00FF);
		}
		
		digitalWrite(SS, HIGH);
	}

	if (base->srate)
		countWithModule1(base->map.columnIndex, base->map.columnsCount, 
						(base->srate < 0));
	else
		mType = type_t::noType;
}

void Letter::predefinedTick()
{
	base_t* base = reinterpret_cast<base_t*>(mRaw);
	bool hasToRefresh = false;
	bool hasToSlide = false;

	base->predefined.remainingRefreshTicks -= TICKS;
	if (base->predefined.remainingRefreshTicks <= 0) {
		base->predefined.remainingRefreshTicks = base->predefined.fpms;
		hasToRefresh = true;
	}

	base->remainingSlideTicks -= TICKS;
	if (base->srate && base->remainingSlideTicks <= 0) {
		base->remainingSlideTicks = (base->srate < 0) ? -base->srate : 
									base->srate;
		hasToSlide = true;
	}

	if (hasToRefresh | hasToSlide) {
		uint8_t i = base->predefined.columnIndex;

		for(uint8_t j = 0; j < mLetterCount * MAX_COLUMNS; j++)
			mCommandBuffer[j] = 0;

		for (uint8_t j = 0; j < MAX_COLUMNS; j++) {
			mCommandBuffer[i] = predefined_values[base->predefined.sprite + base->predefined.spriteIndex][j];
			countWithModule1(i, base->predefined.columnsCount, true);
		}

		for(uint8_t j = 0; j < MAX_COLUMNS; j++) {

			digitalWrite(SS, LOW);
			for(int8_t k = mLetterCount - 1; k >= 0; k--) {

				uint8_t address = j + 1;
				uint8_t value = mCommandBuffer[k * MAX_COLUMNS + j];

				SPI.transfer16(address << 8 | (value & 0x00FF));
			}
			digitalWrite(SS, HIGH);

		}

		if (hasToRefresh)
			countWithModule1(base->predefined.spriteIndex, base->predefined.spritesCount, true);

		if(hasToSlide)
			countWithModule1(base->predefined.columnIndex, base->predefined.columnsCount, (base->srate > 0));
	}
}

void Letter::partyTick()
{
	base_t* base = reinterpret_cast<base_t*>(mRaw);
	base->party.remainingTicks -= TICKS;
	if (base->party.remainingTicks > 0)
		return;

	base->party.remainingTicks = base->party.ticks;

	if (base->party.on) {
		for(uint8_t i = 0; i < MAX_COLUMNS; i++)
			sendCommand(MAX7219_REG_DIGIT0 + i, 0x00);

		base->party.on = 0;
	} else {
		for (uint8_t j = 1; j <= MAX_COLUMNS; j++) {

			digitalWrite(SS, LOW);
			for(int8_t k = mLetterCount - 1; k >= 0; k--) {
				SPI.transfer16(j << 8 | (random(256) & 0x00FF));
			}
			digitalWrite(SS, HIGH);

		}
		base->party.on = 1;
	}
}

static void countWithModule1(uint8_t &value, uint8_t maxValue, bool ascnd)
{
	if (ascnd) {
		if (value < maxValue - 1) {
			value++;
		} else {
			value = 0;
		}
	} else {
		if (value > 0) {
			value--;
		} else {
			value = maxValue - 1;
		}
	}
}

static void countWithModule2(uint8_t &seconds, uint8_t maxSeconds, uint8_t &minutes, uint8_t maxMinutes, bool ascnd)
{
	if (ascnd) {
		if (seconds < maxSeconds - 1) {
			seconds++;
		} else {
			seconds = 0;
			if (minutes < maxMinutes - 1)
				minutes++;
			else
				minutes = 0;
		}
	} else {
		if (seconds > 0) {
			seconds--;
		} else {
			seconds = maxSeconds - 1;
			if (minutes > 0)
				minutes--;
			else
				minutes = maxMinutes - 1;
		}
	}
}

int strcpy_s(char *dest, unsigned int dmax, const char *src)
{
	char *dst = dest;

	if (dmax)
		dmax--;
	else
		return 1;

	while (*src && dmax) {
		*(dest++) = *(src++);
		dmax--;
	}

	*dest = '\0';

	if (*src) {
		reinterpret_cast<char*>(dst)[0] = '\0';
		return 1;
	}

	return 0;
}