#include <stdint.h>

#ifndef AUDIO


void InitSounds();
void HandleAudioCall(int idx, uint8_t value);
void HandleAudioCallLoop(int idx, uint8_t value);
void StopAudio();
void StopLoop(uint8_t channel);
#endif