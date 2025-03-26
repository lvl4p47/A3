#include "audio.h"

Mix_Chunk *fire = NULL;

void AudioInitialize()
{
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 256 );
    
    fire = Mix_LoadWAV( "../media/fire.wav" );
}

void AudioTerminate()
{
    Mix_FreeChunk(fire);
}

void AudioFireSetVolume(int volume)
{
    Mix_VolumeChunk(fire, volume);
    printf("\tvolume:\t%i\n", volume);
    int loop_volume = volume;
    
    while(loop_volume > 0)
    {
        printf("|");
        loop_volume--;
    }
}

void AudioFirePlay()
{
    if(Mix_Playing(-1) == 0)
    {
        printf("\nresetting audio");
        Mix_PlayChannel( -1, fire, 0 );
    }
    else printf("\naudio is playing");
}