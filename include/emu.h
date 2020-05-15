#ifndef EMU
#define EMU

void RunEmu(char*file);
State8080 SetUpEmu();
State8080* LoadROM(State8080 *state, char*file, uint16_t location);
#endif