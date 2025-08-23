#include "audio.h"

Mix_Chunk *fire = NULL;
Mix_Chunk *ice = NULL;
Mix_Chunk *sand = NULL;

void AudioInitialize()
{
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 8, 256 );
    
    fire = Mix_LoadWAV( "../media/fire.wav" );
    ice = Mix_LoadWAV( "../media/ice.wav" );
    sand = Mix_LoadWAV( "../media/sand.wav" );
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
    if(Mix_Playing(0) == 0)
    {
        printf("\nresetting audio");
        Mix_PlayChannel( 0, fire, 10 );
    }
    else printf("\naudio is playing");
}


void AudioIceSetVolume(int volume)
{
    Mix_VolumeChunk(ice, volume);
    printf("\tvolume:\t%i\n", volume);
    int loop_volume = volume;
    
    while(loop_volume > 0)
    {
        printf("|");
        loop_volume--;
    }
}

void AudioIcePlay()
{
    if(Mix_Playing(1) == 0)
    {
        printf("\nresetting audio");
        Mix_PlayChannel( 1, ice, 10 );
    }
    else printf("\naudio is playing");
}

void AudioSandSetVolume(int volume)
{
    Mix_VolumeChunk(sand, volume);
    printf("\tvolume:\t%i\n", volume);
    int loop_volume = volume;
    
    while(loop_volume > 0)
    {
        printf("|");
        loop_volume--;
    }
}

void AudioSandPlay()
{
    if(Mix_Playing(2) == 0)
    {
        printf("\nresetting audio");
        Mix_PlayChannel( 2, sand, 10 );
    }
    else printf("\naudio is playing");
}