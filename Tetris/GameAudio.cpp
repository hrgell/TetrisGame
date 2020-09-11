#include "stdafx.h"
#include "Util.h"
#include "GameAudio.h"
#include "Resource.h"

GameAudio::GameAudio()
	:
#ifdef HASMUSIC
	enabled(true),

#else
	enabled(false),
#endif
	resources(BasicResources::Instance()),
	scope_sound([]() { al_reserve_samples(3); }), // TODO unreserve samples
	sound_theme1(IDR_TETRIS_MUSIC1, resources.wavname_theme1),
	sound_theme2(IDR_TETRIS_MUSIC2, resources.wavname_theme2),
	sound_tada(0, resources.wavname_tada)
{
}

GameAudio::~GameAudio()
{
}

void GameAudio::ToggleMusic() {
	if (enabled) {
		sound_theme1.Stop();
		sound_theme2.Stop();
	}
	enabled = !enabled;
}

void GameAudio::PlayNice() {
	if (enabled)
		sound_tada.Play();
}

void GameAudio::PlayMove() {
	//if(enabled)
	//    move_sound.Play();
}

void GameAudio::PlayTheme1() {
	if (enabled)
		sound_theme1.Play();
}

void GameAudio::PlayTheme2() {
	if (enabled)
		sound_theme2.Play();
}

bool GameAudio::PlayTheme(bool playmaintheme) {
	if (!enabled)
		return false;
	if (sound_theme1.IsPlaying() || sound_theme2.IsPlaying())
		return false;
	if (playmaintheme)
		PlayTheme1();
	else
		PlayTheme2();
	return true;
}