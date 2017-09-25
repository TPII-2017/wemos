#include "ledmatrix.h"
#include <string.h>
#include <Arduino.h>

char matrix [ROWS][COLUMNS];

void ledmatrix_set_char(char, char);
void ledmatrix_clean(void);
void ledmatrix_set_space(char);

void ledmatrix_set_str(char * str){
	ledmatrix_clean();
	for(char i = 0; i < strlen(str); i++){
		ledmatrix_set_char(i*CHAR_WIDTH+i,str[i]);
		ledmatrix_set_space(i*CHAR_WIDTH+1);
	}
}

void ledmatrix_set_space(char pos_x){
	char row;
	for(row = 0; row < ROWS; row++){
		matrix[row][pos_x];
	}
}

void ledmatrix_set_char(char pos_x, char c){
	const char* char_map = alphabet_get_char_map(c);
	char map_col = 0;
	char column;
	char row;
	for(column = pos_x; column < pos_x+CHAR_WIDTH; column++){
		char byte_col = char_map[map_col];
		// byte_col= 0x7D = 01111101
		//   (byte_col << 0)   01111101
		// &            128  & 10000000
		// ============================
		//                     00000000

		//   (byte_col << 1)   11111010
		// &            128  & 10000000
		// ============================
		//                     10000000
		// etc
		for(row = 0; row < CHAR_HEIGHT; row++){		
			if (byte_col / 128 == 1){
				matrix[row][column] = LED_ON;
			}else{
				matrix[row][column] = LED_OFF;
			}
			byte_col = byte_col << 1;			 
		}
		map_col++;
	}	
}

void ledmatrix_clean(void){
	for(int row = 0; row < ROWS; row++){
        for(int column = 0; column < COLUMNS; column++){
            matrix[row][column] = LED_OFF;
        }
    }
}

void ledmatrix_print_serial(){
    char row, column;
    for(row = 0; row < ROWS; row++){
        for(column = 0; column < COLUMNS; column++){
            Serial.print(matrix[row][column]);
        }
        Serial.println("");
    }
}
