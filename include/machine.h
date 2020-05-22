// File to handle space invaders specific architecutres

#include <i8080.h>
#include <input.h>
#ifndef MACHINE
#define MACHINE


typedef struct IOPorts { 
  uint8_t r1;
  uint8_t r2;
  uint8_t r3;
  uint8_t w2;
  uint8_t w4;
} IOPorts;

int Shift();

int NextInstruction(State8080 *state, InputMap *inp);



#endif