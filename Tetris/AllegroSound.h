#pragma once

#include <string>
#include <allegro5/allegro_audio.h>

class AllegroSound
{
public:
	size_t id;
	std::string filename;
	ALLEGRO_PLAYMODE flag;
	ALLEGRO_SAMPLE* sample;
	ALLEGRO_SAMPLE_INSTANCE* sample_instance;
	AllegroSound() = delete;
	AllegroSound(const AllegroSound &) = delete;
	AllegroSound &operator=(const AllegroSound &) = delete;
	AllegroSound(size_t id, std::string filename, ALLEGRO_PLAYMODE flag = ALLEGRO_PLAYMODE_ONCE);
	AllegroSound(std::string filename, ALLEGRO_PLAYMODE flag = ALLEGRO_PLAYMODE_ONCE);
	~AllegroSound();
	void Load();
	void Play();
	void Stop();
	bool IsPlaying();
};

