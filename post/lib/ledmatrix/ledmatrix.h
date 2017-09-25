#ifndef LEDMATRIX_H
#define LEDMATRIX_H

#include "alphabet.h"

#define ROWS 8
#define COLUMNS 24

//TODO: por ahora son caracteres para debug
#define LED_ON 'X'
#define LED_OFF '.'

extern char matrix [ROWS][COLUMNS];

void ledmatrix_set_str(char *);
void ledmatrix_print_serial(void);

#endif