#include <stdint.h>

#ifndef EMU_UTILS
#define EMU_UTILS

int GetFirstPart(unsigned char code);
int GetLastPart(unsigned char code);
char GetDest(unsigned char code);
char GetSource(unsigned char code);
uint16_t MakeWord(uint8_t x, uint8_t y); 
uint32_t Add16Bit(uint8_t *left_ptr, uint8_t *right_ptr, uint16_t to_add);


#endif