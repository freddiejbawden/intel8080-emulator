#include <graphics.h>
#include <unistd.h>
#include <i8080.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#define WINDOW_WIDTH 224
#define WINDOW_HEIGHT 256

void InitialiseVRAM(Graphics *graphics) {
  // Note: SDL needs an 8 bit color so we have to have a larger vram
  size_t len = WINDOW_HEIGHT * WINDOW_WIDTH * 4;
  graphics->vram = (uint8_t*) malloc(len * sizeof(graphics->vram));;
  graphics->pitch = sizeof(uint8_t) * 4 * WINDOW_WIDTH;

}


void CreateRenderer(Graphics *graphics) {
  Uint32 render_flags = SDL_RENDERER_ACCELERATED  | SDL_RENDERER_PRESENTVSYNC;
  SDL_Renderer* rend = SDL_CreateRenderer(graphics->window, -1, render_flags);
  graphics->rend = rend;
}

void CreateTexture(Graphics *graphics) {
  SDL_Texture* texture = SDL_CreateTexture(
        graphics->rend,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        WINDOW_WIDTH,
        WINDOW_HEIGHT);
  graphics->texture = texture;
} 

void CreateWindow(Graphics *graphics) {
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
  graphics->window = win;
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

Graphics SetUpGraphics() {
  StartScreen();
  Graphics graphics;
  CreateWindow(&graphics);
  CreateRenderer(&graphics);
  CreateTexture(&graphics);
  InitialiseVRAM(&graphics);
  return graphics;
}

int EndScreen(Graphics *graphics) {
  SDL_DestroyRenderer(graphics->rend);
  SDL_DestroyWindow(graphics->window);
  SDL_Quit();
  return 0;
}



void DrawScreen(State8080 *state, Graphics *graphics, int middle) {  
  
  int limit = (WINDOW_HEIGHT * WINDOW_WIDTH) / 8
;
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
        int pix_pos = (py*WINDOW_WIDTH + px) * 4;
        uint8_t pixel_val; 
        if (pixel) pixel_val = 255; else pixel_val = 0;
        graphics->vram[pix_pos] = pixel_val;
        graphics->vram[pix_pos + 1] = pixel_val; 
        graphics->vram[pix_pos + 2] = pixel_val;


        // SDL_SetRenderDrawColor(graphics->rend, 255*pixel,255*pixel,255*pixel,255);
        // SDL_RenderDrawPoint(graphics->rend, px,py);
    }
  }
  SDL_UpdateTexture(graphics->texture, NULL, graphics->vram, graphics->pitch);
  SDL_RenderClear(graphics->rend);
  SDL_RenderCopy(graphics->rend, graphics->texture, NULL, NULL);
  SDL_RenderPresent(graphics->rend);
}
