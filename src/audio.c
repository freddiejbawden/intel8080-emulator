#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <audio.h>

static Mix_Chunk* sounds[256] = {NULL};
uint8_t playing[256] = {0};
int audio_rate = 44100; 
uint16_t audio_format = AUDIO_S16SYS; 
int audio_channels = 2; 
int audio_buffers = 512; 
int looping_channel;


int LoadSounds(int channel, char* filename) {
  Mix_Chunk* snd = Mix_LoadWAV(filename);
  if (snd == NULL) {
      printf("Unable to load file '%s'\n", filename);
      return -1;
  }
  sounds[channel] = snd;
  return channel;
}



void InitSounds() {
  if( Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,
        MIX_DEFAULT_FORMAT,
        MIX_DEFAULT_CHANNELS,
         1024) != 0 ) {
		printf("Unable to initialize audio: %s\n", Mix_GetError()); 
    exit(1); 
  }

  LoadSounds(0,"./sounds/0.wav");
  LoadSounds(1,"./sounds/1.wav");
  LoadSounds(2,"./sounds/2.wav");
  LoadSounds(3,"./sounds/3.wav");
  LoadSounds(4,"./sounds/4.wav");
  LoadSounds(5,"./sounds/5.wav");
  LoadSounds(6,"./sounds/6.wav");
  LoadSounds(7,"./sounds/7.wav");
  LoadSounds(8,"./sounds/8.wav");
}

// frees buffered sounds and quits SDL_mixer
void StopAudio() {
    for (int i = 0; i < 256; i++) {
        if (sounds[i] != NULL) {
          Mix_FreeChunk(sounds[i]);
          sounds[i] = NULL;
        }
    }
    Mix_Quit();
}

void PlaySound(uint8_t value) {
  if(Mix_PlayChannel(-1, sounds[value], 0)==-1) {
    printf("Unable to play WAV file: %s\n", Mix_GetError());    // may be critical error, or maybe just no channels were free.
    // you could allocated another channel in that case...
  }
}

void LoopSound(uint8_t value) {
  looping_channel = Mix_PlayChannel(-1, sounds[value], -1);
  if (looping_channel == -1) {
    printf("Unable to play WAV file: %s\n", Mix_GetError());    // may be critical error, or maybe just no channels were free.
    // you could allocated another channel in that case...
  }
}

void StopSound(uint8_t channel) {
  Mix_HaltChannel(channel);
}
void HandleAudioCall(int idx, uint8_t value) {
  if (playing[idx] == 0 && value == 1) {
    printf("Playing %d\n", idx);
    PlaySound(idx);
  }
  playing[idx] = value;
}
void HandleAudioCallLoop(int idx, uint8_t value) {
  if (playing[idx] == 0 && value == 1) {
    LoopSound(idx);
  } else if (playing[idx] == 1 && value == 0) {
    StopSound(looping_channel);

  }
  playing[idx] = value;
}