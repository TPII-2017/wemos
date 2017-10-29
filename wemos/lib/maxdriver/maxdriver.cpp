#include "maxdriver.h"

inline void sendByte(const byte reg, const byte data){
	digitalWrite(SS, LOW);
	SPI.transfer(reg);
	SPI.transfer(data);
	digitalWrite(SS, HIGH); 
}

void letter(const byte c, const int time){
	for(byte col=0; col<8; col++)
	{
		sendByte(col + 1, pgm_read_byte(&cp437_font[c][col]));
		delay(time);
	}
}

void showAllCaracters(const unsigned long time){
	for(int c=67; c<TOTAL; c++)
	{
		letter(c, 50);
		delay(time);
	}
}

void movimientoSensual(){
	for(int i=-8;i<6;i++)
	{	
		letter(' ', 0);
		for(byte col=0; col<8; col++)
		{
			if ((col + 1 - i > 0) && (col + 1 - i <= 8))
				sendByte(col + 1 - i, pgm_read_byte(&cp437_font['2'][col]));
		}
		delay(300);
	}
}

void maxdriver_setup(){
	SPI.begin();
	pinMode(SS, OUTPUT);

	sendByte(MAX7219_REG_SCANLIMIT, 7);	// show all 8 digits
	sendByte(MAX7219_REG_DECODEMODE, 0);	// using an led matrix (not digits)
	sendByte(MAX7219_REG_DISPLAYTEST, 0);	// display test = 1 - no display test = 0
	
	// clear display
	for(byte col = 0; col < 8; col++)
		sendByte(col + 1, 0);

	sendByte(MAX7219_REG_INTENSITY, 15);  // character intensity: range: 0 to 15
	sendByte(MAX7219_REG_SHUTDOWN, 1);   // not in shutdown mode (ie. start it up)
}

void maxdriver_loop(){
	byte aux[11]={0x7F, 0x88, 0x88, 0x88, 0x7F, 0x00, 0xFF, 0x91, 0x91, 0x91, 0x6E};
	for(int i=-8;i<7;i++)
	{	
		letter(' ', 0);
		for(byte col=0; col<11; col++)
		{
			if ((col + 1 - i > 0) && (col + 1 - i <= 8))
				sendByte(col + 1 - i, aux[col]);
		}
		delay(300);
	}	
}