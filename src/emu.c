#include <stdio.h>
#include <i8080.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <graphics.h>
#include <emu.h>
#include <machine.h>
#include <i8080.h>
#include <input.h>



void GenerateInterrupt(State8080 *state, int interrupt_num)    
   {   
    //perform "PUSH PC"    
    Push(state, (state->pc & 0xFF00) >> 8, (state->pc & 0xff));
    if (interrupt_num == 8) {
      state->pc = 0x0008;    
    } else {
      state->pc = 0x0010;    
    }
    state->int_enable = 0;
   }    

State8080 SetUpEmu() {
  ConditionCodes cc;
  cc.z = 0;
  cc.s = 0;
  cc.p = 0;
  cc.cy = 0;
  cc.ac = 0;
  State8080 state;
  state.a = 0;
  state.b = 0;
  state.c = 0;
  state.d = 0;
  state.e = 0;
  state.h = 0;
  state.l = 0;

  state.sp = 0;
  state.pc = 0;
  state.int_enable = 0;
  size_t max_size = 1 << 15;
  state.memory = (uint8_t*) malloc(max_size * sizeof(*state.memory));
  state.cc = cc;
  return state;
}

int GetFileSize(char*file) {
  FILE *f = fopen(file, "rb");
  if (f == NULL) {
    printf("GetFileSize: File %s could not be found\n",file);
  }
  fseek(f, 0L, SEEK_END);
  int fsize = ftell(f); 
  fclose(f);
  return fsize;
}

State8080* LoadROM(State8080 *state, char*file, uint16_t location) {
  FILE *f = fopen(file, "rb");
  if (f == NULL) {
    printf("LoadROM: File %s could not be found\n",file);
  }
  fseek(f, 0L, SEEK_END);

  int fsize = ftell(f);
  fseek(f,0L,SEEK_SET);

  // read in file

  fread(state->memory + location, fsize, 1, f);
  fclose(f);
  printf("\n*********************************\n");
  printf("%s loaded\n", file);
  return state;
}

int64_t getCurrentTime() {
  struct timespec tms;

    /* The C11 way */
    /* if (! timespec_get(&tms, TIME_UTC)) { */

    /* POSIX.1-2008 way */
    if (clock_gettime(CLOCK_REALTIME,&tms)) {
        return -1;
    }
    /* seconds, multiplied with 1 million */
    int64_t micros = tms.tv_sec * 1000000;
    /* Add full microseconds */
    micros += tms.tv_nsec/1000;
    /* round up if necessary */
    if (tms.tv_nsec % 1000 >= 500) {
        ++micros;
    }
    return micros;
}

void RunEmu(char*file) {
  Shift();

  int middle = 1;

  State8080 state = SetUpEmu();
  InputMap inp = CreateInputMap();
  LoadROM(&state, file, 0x0);
  printf("Running %s", file);
  // Set up graphics
  Graphics g = SetUpGraphics();
  uint64_t startTime = getCurrentTime();
  SDL_Event event;
  int cpu_cycles = 0;
  int clock_speed = 2e6/120;
  int fsize = GetFileSize(file);
  while (state.pc < fsize) {
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT){
        break;
      } else {
        GetInput(&inp, event);
      }
    } 
    uint64_t cur_time = getCurrentTime();
      if (!state.int_enable) {
        startTime = cur_time;
      }
      if (state.int_enable) {
        if (cpu_cycles > clock_speed) {
          cpu_cycles = 0;
          if (middle == 1) {
            GenerateInterrupt(&state, 8);
            middle = 0;
          } else {
            GenerateInterrupt(&state, 10);
            middle = 1;
            DrawScreen(&state, &g, middle);
            cur_time = getCurrentTime();
            startTime = cur_time;
          }
      }
    }
    cpu_cycles += NextInstruction(&state, &inp);
  }
  print_state(&state);
  EndScreen(&g);
}