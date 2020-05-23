#include <i8080.h>
#include <SDL2/SDL.h>

#ifndef EMU_GRAPHICS
#define EMU_GRAPHICS

#define VRAM_ADDR 0x2400
#define WINDOW_WIDTH 224
#define WINDOW_HEIGHT 256
typedef struct Graphics {
  SDL_Window* window;
  SDL_Renderer* rend;
  SDL_Texture* texture;
  uint8_t *vram;
  uint32_t pitch;
} Graphics;


void DrawScreen(State8080 *state, Graphics *graphics, int middle);
int StartScreen();
int EndScreen(Graphics *graphics);
Graphics SetUpGraphics();

#endif