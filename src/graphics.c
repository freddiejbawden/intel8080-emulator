#include <graphics.h>
#include <unistd.h>
#include <i8080.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#define WINDOW_WIDTH 224
#define WINDOW_HEIGHT 256



SDL_Renderer* CreateRenderer(SDL_Window* win) {
  Uint32 render_flags = SDL_RENDERER_ACCELERATED  | SDL_RENDERER_PRESENTVSYNC;
  SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
  return rend;
}
 
SDL_Window* CreateWindow() {
  SDL_Window* win = SDL_CreateWindow("Intel8080",
                                      SDL_WINDOWPOS_CENTERED, 
                                      SDL_WINDOWPOS_CENTERED, 
                                      WINDOW_WIDTH,
                                      WINDOW_HEIGHT, 0
                                      );
    if (!win) {
      printf("error creating window: %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
  }
  return win;
}

int StartScreen() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    printf("error initializing SDL: %s\n", SDL_GetError());
    return 1;
  }
  printf("Init Successful!\n");
  return 0;
}


int EndScreen(SDL_Renderer* rend, SDL_Window* win) {
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}

void DrawScreen(State8080 *state, SDL_Renderer* rend, int middle) {  

  int limit = (WINDOW_HEIGHT * WINDOW_WIDTH) / 8;
  for (int i = 0; i < limit; i++) {
    int y = i * 8 / WINDOW_HEIGHT;
    int base_x = (i * 8) % 256;
    uint8_t b = state->memory[VRAM_ADDR + i];
    
    for (int bit = 0; bit < 8; bit++)
    {
        int x = base_x + bit;
        //read 1 bit from the original byte
        int pixel = (b >> bit) & 1;
        const int temp_x = x;
        int px = y;
        int py = -temp_x + WINDOW_HEIGHT - 1;
        SDL_SetRenderDrawColor(rend, 255*pixel,255*pixel,255*pixel,255);
        SDL_RenderDrawPoint(rend, px,py);
    }
  }
  SDL_RenderPresent(rend);

}
