// File to handle space invaders specific architecutres

#include <i8080.h>

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

void NextInstruction(State8080 *state);

void MachineOutPort(State8080 *state, uint8_t port, uint8_t);

uint8_t MachineInPort(State8080 *state, uint8_t port);

#endif