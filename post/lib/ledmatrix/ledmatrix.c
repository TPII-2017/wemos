#include "ledmatrix.h"
#include <string.h>

char matrix [ROWS][COLUMNS];

void ledmatrix_set_char(char, char);
void ledmatrix_clean(void);

void ledmatrix_test(void){
	char i=3;
}

void ledmatrix_set_str(char * str){
	ledmatrix_clean();
	for(char i = 0; i < strlen(str); i++){
		ledmatrix_set_char(i*CHAR_WIDTH,str[i]);
	}
}

void ledmatrix_set_char(char pos_x, char c){
	const char* char_map = alphabet_get_char_map(c);
	char map_col = 0;
	for(char column = pos_x; column < pos_x+CHAR_WIDTH; column++){
		char byte_col = char_map[map_col];
		// byte_col= 0x7F = 01111101
		//   (byte_col >> 0)   01111101
		// &              1  & 00000001
		// ============================
		//                     00000001

		//   (byte_col >> 1)   00111110
		// &              1  & 00000001
		// ============================
		//                     00000000
		// etc
		for(char row = CHAR_HEIGHT-1; row >= 0; row--){
			if (byte_col & 1 == 1){
				matrix[row][column] = 'X';
			}else{
				matrix[row][column] = '.';
			}
			byte_col = byte_col >> 1;			 
		}
		map_col++;
	}	
}

void ledmatrix_clean(void){
	for(int row = 0; row < ROWS; row++){
        for(int column = 0; column < COLUMNS; column++){
            matrix[row][column] = 0;
        }
    }
}