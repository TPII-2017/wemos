#ifndef MAXDRIVER_H
#define MAXDRIVER_H

#include <Arduino.h>
#include <SPI.h>

// Registers
#define MAX7219_REG_NOOP		0x0
#define MAX7219_REG_DIGIT0		0x1
#define MAX7219_REG_DIGIT1		0x2
#define MAX7219_REG_DIGIT2		0x3
#define MAX7219_REG_DIGIT3		0x4
#define MAX7219_REG_DIGIT4		0x5
#define MAX7219_REG_DIGIT5		0x6
#define MAX7219_REG_DIGIT6		0x7
#define MAX7219_REG_DIGIT7		0x8
#define MAX7219_REG_DECODEMODE	0x9
#define MAX7219_REG_INTENSITY	0xA
#define MAX7219_REG_SCANLIMIT	0xB
#define MAX7219_REG_SHUTDOWN	0xC
#define MAX7219_REG_DISPLAYTEST	0xF

void maxdriver_setup(void);
void maxdriver_loop(void);

#endif