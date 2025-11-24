#ifndef _SFX_H_
#define _SFX_H_

#include "../global.h"

void BackGroundMusic(RenderWindow &window);
void PlaySoundClick();
void PlaySoundWin();
void PlaySoundDraw();
void PlaySoundLoading();

void SoundMute();
bool GetSoundMute();          // Get current mute state
void SetSoundMute(bool mute); // Set mute state directly
void SetMusicVolume(float volume);
float GetMusicVolume();
void SetEffectVolume(float volume);
float GetEffectVolume();

#endif // _SFX_H_
