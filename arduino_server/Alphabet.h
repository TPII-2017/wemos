#ifndef ALPHABET_H
#define ALPHABET_H

#define CHAR_HEIGHT 8
#define CHAR_WIDTH 5
#define ALPHABET_SIZE 26

extern const char uppercase_alphabet[ALPHABET_SIZE][CHAR_WIDTH];
extern const char lowercase_alphabet[ALPHABET_SIZE][CHAR_WIDTH];
extern const char numbers[10][CHAR_WIDTH];

const char* alphabet_get_char_map(char c);

#endif