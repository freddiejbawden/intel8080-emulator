#include <stdint.h>

#ifndef i8080
#define i8080

typedef struct ConditionCodes { 
  uint8_t   z:1;
  uint8_t   s:1;
  uint8_t   p:1;
  uint8_t   cy:1;
  uint8_t   ac:1;
  uint8_t   pad:1;

} ConditionCodes;

typedef struct State8080 {
  uint8_t   a;
  uint8_t   b;
  uint8_t   c;
  uint8_t   d;
  uint8_t   e;
  uint8_t   h;
  uint8_t   l;
  uint16_t  sp;
  uint16_t  pc;
  uint8_t   *memory;
  struct    ConditionCodes  cc;
  uint8_t   int_enable;
} State8080;

int Emulate8080Op(State8080* state, int debug);
void Push(State8080 *state, uint8_t hi, uint8_t lo);
void print_state(State8080* state);
#endif