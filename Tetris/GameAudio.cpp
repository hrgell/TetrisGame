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
#ifdef LOOP_SOUND1
	sound_theme1(IDR_TETRIS_MUSIC1, resources.wavname_theme1, ALLEGRO_PLAYMODE_LOOP),
#else
	sound_theme1(IDR_TETRIS_MUSIC1, resources.wavname_theme1),
#endif
	sound_theme2(IDR_TETRIS_MUSIC2, resources.wavname_theme2),
	sound_theme3(IDR_TETRIS_MUSIC3, resources.wavname_theme3),
	sound_theme4(IDR_TETRIS_MUSIC4, resources.wavname_theme4),
	sound_theme5(IDR_TETRIS_MUSIC5, resources.wavname_theme5),
	sound_theme6(IDR_TETRIS_MUSIC6, resources.wavname_theme6),
	sound_theme7(IDR_TETRIS_MUSIC7, resources.wavname_theme7),
	sound_theme8(IDR_TETRIS_MUSIC8, resources.wavname_theme8),
	sound_theme9(IDR_TETRIS_MUSIC9, resources.wavname_theme9),
	sound_tada(0, resources.wavname_tada)
{
	sound_theme1.SetGain(0.4); // the volume of the first sample is too high compared to the rest of the samples.
}

GameAudio::~GameAudio()
{
}

void GameAudio::ToggleMusic() {
	if (enabled) {
		sound_theme1.Stop();
		sound_theme2.Stop();
		sound_theme3.Stop();
		sound_theme4.Stop();
		sound_theme5.Stop();
		sound_theme6.Stop();
		sound_theme7.Stop();
		sound_theme8.Stop();
		sound_theme9.Stop();
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

void GameAudio::PlayTheme1() { if (enabled) sound_theme1.Play(); }
void GameAudio::PlayTheme2() { if (enabled)	sound_theme2.Play(); }
void GameAudio::PlayTheme3() { if (enabled) sound_theme3.Play(); }
void GameAudio::PlayTheme4() { if (enabled) sound_theme4.Play(); }
void GameAudio::PlayTheme5() { if (enabled) sound_theme5.Play(); }
void GameAudio::PlayTheme6() { if (enabled) sound_theme6.Play(); }
void GameAudio::PlayTheme7() { if (enabled) sound_theme7.Play(); }
void GameAudio::PlayTheme8() { if (enabled) sound_theme8.Play(); }
void GameAudio::PlayTheme9() { if (enabled) sound_theme9.Play(); }

bool GameAudio::PlayTheme() {
	static unsigned long toggle = 0;
	if (!enabled)
		return false;
	if (sound_theme1.IsPlaying()
		|| sound_theme2.IsPlaying()
		|| sound_theme3.IsPlaying()
		|| sound_theme4.IsPlaying()
		|| sound_theme5.IsPlaying()
		|| sound_theme6.IsPlaying()
		|| sound_theme7.IsPlaying()
		|| sound_theme8.IsPlaying()
		|| sound_theme9.IsPlaying())
		return false;
	if (++toggle > 9)
		toggle = 1;
#ifdef LOOP_SOUND1
	toggle = 1;
#endif
	if (toggle == 1) PlayTheme1();
	else if (toggle == 2) PlayTheme2();
	else if (toggle == 3) PlayTheme3();
	else if (toggle == 4) PlayTheme4();
	else if (toggle == 5) PlayTheme5();
	else if (toggle == 6) PlayTheme6();
	else if (toggle == 7) PlayTheme7();
	else if (toggle == 8) PlayTheme8();
	else if (toggle == 9) PlayTheme9();
	return true;
}