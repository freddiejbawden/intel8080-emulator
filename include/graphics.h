#include <i8080.h>
#include <SDL2/SDL.h>

#ifndef EMU_GRAPHICS
#define EMU_GRAPHICS

#define VRAM_ADDR 0x2400

void DrawScreen(State8080 *state, SDL_Renderer* rend, int middle);
int StartScreen();
int EndScreen(SDL_Renderer* rend, SDL_Window* win);
SDL_Window* CreateWindow();
SDL_Renderer* CreateRenderer(SDL_Window* win);
 
#endif