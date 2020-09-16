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
	AllegroSound sound_tada;
	//AllegroSound move_sound;
	GameAudio();
	~GameAudio();
	void ToggleMusic();
	void PlayNice();
	void PlayMove();
	void PlayTheme1();
	void PlayTheme2();
	bool PlayTheme(bool playmaintheme);
};

