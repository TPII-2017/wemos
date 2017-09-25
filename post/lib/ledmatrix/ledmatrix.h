#ifndef LEDMATRIX_H
#define LEDMATRIX_H

#include "alphabet.h"

#define ROWS 8
#define COLUMNS 24

extern char matrix [ROWS][COLUMNS];

void ledmatrix_set_str(char *);
void ledmatrix_test(void);

#endif