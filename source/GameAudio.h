#pragma once
#include "BasicResources.h"
#include "Scope.h"
#include "AllegroSound.h"

class GameAudio
{
public:
	bool enabled;
	BasicResources const& resources;
	Scope scope_sound;
	AllegroSound sound_theme1;
	AllegroSound sound_theme2;
	AllegroSound sound_theme3;
	AllegroSound sound_theme4;
	AllegroSound sound_theme5;
	AllegroSound sound_theme6;
	AllegroSound sound_theme7;
	AllegroSound sound_theme8;
	AllegroSound sound_theme9;
	AllegroSound sound_tada;
	//AllegroSound move_sound;
	GameAudio();
	~GameAudio();
	void ToggleMusic();
	void PlayNice();
	void PlayMove();
	void PlayTheme1();
	void PlayTheme2();
	void PlayTheme3();
	void PlayTheme4();
	void PlayTheme5();
	void PlayTheme6();
	void PlayTheme7();
	void PlayTheme8();
	void PlayTheme9();
	bool PlayTheme();
};

