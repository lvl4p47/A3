#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

#include "func.h"

void AudioInitialize();

void AudioTerminate();

void AudioFireSetVolume(int volume);

void AudioFirePlay();

void AudioIceSetVolume(int volume);

void AudioIcePlay();

void AudioSandSetVolume(int volume);

void AudioSandPlay();

void AudioWaterSetVolume(int volume);

void AudioWaterPlay();

void AudioMagmaSetVolume(int volume);

void AudioMagmaPlay();

void AudioDirtSetVolume(int volume);

void AudioDirtPlay();

void AudioRockSetVolume(int volume);

void AudioRockPlay();

void AudioFabricSetVolume(int volume);

void AudioFabricPlay();


#endif