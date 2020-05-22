#include <stdint.h>
#include <input.h>
#include <SDL2/SDL.h>


InputMap CreateInputMap() 
{
  // set up start staet from 
  // https://www.computerarcheology.com/Arcade/SpaceInvaders/Hardware.html
  InputMap inp;
  inp.in_port0 = 0b00001110;
  inp.in_port1 = 0b00001000;
  inp.in_port2 = 0b00000000;
  return inp;
}

void HandleKeyDown(InputMap *inp, SDL_Event event) 
{
 switch (event.key.keysym.scancode)
    {
    case SDL_SCANCODE_A:
      inp->in_port1 |= 0x20; //0010 0000
      break;
    case SDL_SCANCODE_D:
      inp->in_port1 |= 0x40; //0100 0000
      break;
    case SDL_SCANCODE_W:
      inp->in_port1 |= 0x10; //0001 0000
      break;
    case SDL_SCANCODE_1:
      inp->in_port1 |= 0x4; //0000 0100
    case SDL_SCANCODE_2:
      inp->in_port1 |= 0x2; //0000 0010
    case SDL_SCANCODE_C:
      inp->in_port1 |= 0x1; //0000 0001
    default:
      break;
    }
}


void HandleKeyUp(InputMap *inp, SDL_Event event) 
{
 switch (event.key.keysym.scancode)
    {
    case SDL_SCANCODE_A:
      inp->in_port1 &= ~0x20; //0010 0000
      break;
    case SDL_SCANCODE_D:
      inp->in_port1 &= ~0x40; //0100 0000
      break;
    case SDL_SCANCODE_W:
      inp->in_port1 &= ~0x10; //0001 0000
      break;
    case SDL_SCANCODE_1:
      inp->in_port1 &= ~0x4; //0000 0100
    case SDL_SCANCODE_2:
      inp->in_port1 &= ~0x2; //0000 0010
    case SDL_SCANCODE_C:
      inp->in_port1 &= ~0x1; //0000 0001
    default:
      break;
    }
}

void GetInput(InputMap *inp, SDL_Event event) {
  switch (event.type)
  {
  case SDL_KEYDOWN:
    HandleKeyDown(inp, event);
    /* code */
    break;
  case SDL_KEYUP:
    HandleKeyUp(inp, event);
    /* code */
    break;
  default:
    break;
  }
}