#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

#include "func.h"

void AudioInitialize();

void AudioTerminate();

void AudioFireSetVolume(int volume);

void AudioFirePlay();

#endif