#include <SDL2/SDL.h>

#ifndef EMU_INPUT
#define EMU_INPUT


typedef struct InputMap
{
  uint8_t in_port0;
  uint8_t in_port1;
  uint8_t in_port2;
} InputMap;

InputMap CreateInputMap();

void GetInput(InputMap *inp, SDL_Event event);

#endif