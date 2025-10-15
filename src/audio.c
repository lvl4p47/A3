#include "audio.h"

Mix_Chunk *fire = NULL;
Mix_Chunk *ice = NULL;
Mix_Chunk *sand = NULL;
Mix_Chunk *water = NULL;
Mix_Chunk *magma = NULL;
Mix_Chunk *dirt = NULL;
Mix_Chunk *rock = NULL;
Mix_Chunk *fabric = NULL;

void AudioInitialize()
{
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 8, 256 );
    
    fire = Mix_LoadWAV( "../media/fire.wav" );
    ice = Mix_LoadWAV( "../media/ice.wav" );
    sand = Mix_LoadWAV( "../media/sand.wav" );
    water = Mix_LoadWAV( "../media/water.wav" );
    magma = Mix_LoadWAV( "../media/magma.wav" );
    dirt = Mix_LoadWAV( "../media/dirt.wav" );
    rock = Mix_LoadWAV( "../media/rock.wav" );
    fabric = Mix_LoadWAV( "../media/fabric.wav" );
}

void AudioTerminate()
{
    Mix_FreeChunk(fire);
}

void AudioFireSetVolume(int volume)
{
    Mix_VolumeChunk(fire, volume);
    // if(volume > 0) printf("\nFire: ", volume);
    int loop_volume = volume;
    
    while(loop_volume > 0)
    {
        // printf("|");
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
}


void AudioIceSetVolume(int volume)
{
    Mix_VolumeChunk(ice, volume);
    // if(volume > 0) printf("\nIce: ", volume);
    int loop_volume = volume;
    
    while(loop_volume > 0)
    {
        // printf("|");
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
}

void AudioSandSetVolume(int volume)
{
    Mix_VolumeChunk(sand, volume);
    // if(volume > 0) printf("\nSand: ", volume);
    int loop_volume = volume;
    
    while(loop_volume > 0)
    {
        // printf("|");
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
}

void AudioWaterSetVolume(int volume)
{
    Mix_VolumeChunk(water, volume);
    // if(volume > 0) printf("\nWater: ", volume);
    int loop_volume = volume;
    
    while(loop_volume > 0)
    {
        // printf("|");
        loop_volume--;
    }
}

void AudioWaterPlay()
{
    if(Mix_Playing(3) == 0)
    {
        printf("\nresetting audio");
        Mix_PlayChannel( 3, water, 10 );
    }
}

void AudioMagmaSetVolume(int volume)
{
    Mix_VolumeChunk(magma, volume);
    // if(volume > 0) printf("\nMagma: ", volume);
    int loop_volume = volume;
    
    while(loop_volume > 0)
    {
        // printf("|");
        loop_volume--;
    }
}

void AudioMagmaPlay()
{
    if(Mix_Playing(4) == 0)
    {
        printf("\nresetting audio");
        Mix_PlayChannel( 4, magma, 10 );
    }
}

void AudioDirtSetVolume(int volume)
{
    Mix_VolumeChunk(dirt, volume);
    // if(volume > 0) printf("\nDirt: ", volume);
    int loop_volume = volume;
    
    while(loop_volume > 0)
    {
        // printf("|");
        loop_volume--;
    }
}

void AudioDirtPlay()
{
    if(Mix_Playing(5) == 0)
    {
        printf("\nresetting audio");
        Mix_PlayChannel( 5, dirt, 10 );
    }
}

void AudioRockSetVolume(int volume)
{
    Mix_VolumeChunk(rock, volume);
    // if(volume > 0) printf("\nRock: ", volume);
    int loop_volume = volume;
    
    while(loop_volume > 0)
    {
        // printf("|");
        loop_volume--;
    }
}

void AudioRockPlay()
{
    if(Mix_Playing(6) == 0)
    {
        printf("\nresetting audio");
        Mix_PlayChannel( 6, rock, 10 );
    }
}

void AudioFabricSetVolume(int volume)
{
    Mix_VolumeChunk(fabric, volume);
    // if(volume > 0) printf("\nFabric: ", volume);
    int loop_volume = volume;
    
    while(loop_volume > 0)
    {
        // printf("|");
        loop_volume--;
    }
}

void AudioFabricPlay()
{
    if(Mix_Playing(7) == 0)
    {
        printf("\nresetting audio");
        Mix_PlayChannel( 7, fabric, 10 );
    }
}