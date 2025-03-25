#include "audio.h"

Mix_Chunk *fire = NULL;

void AudioInitialize()
{
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 512 );
    
    fire = Mix_LoadWAV( "../media/fire.wav" );
}

void AudioTerminate()
{
    Mix_FreeChunk(fire);
}

void AudioFireSetVolume(int volume)
{
    Mix_VolumeChunk(fire, volume);
}

void AudioFirePlay()
{
    int play = 0;
    
    play = Mix_PlayChannel( -1, fire, -1 );
    // if(play == 1) printf("audio fire playing");
    // if(play == -1) printf("no free channel");
}