#include "stdafx.h"
#include "AllegroSound.h"
#include "Util.h"

#pragma warning( suppress: 26812 )
AllegroSound::AllegroSound(size_t id, std::string filename, ALLEGRO_PLAYMODE flag) :
	id(id), filename(filename), flag(flag), sample(NULL), sample_instance(NULL)
{
	Load();
}

#pragma warning( suppress: 26812 )
AllegroSound::AllegroSound(std::string filename, ALLEGRO_PLAYMODE flag) :
	id(0), filename(filename), sample(NULL), sample_instance(NULL), flag(flag)
{
	Load();
}

AllegroSound::~AllegroSound()
{
	if (sample_instance != NULL) {
		al_destroy_sample_instance(sample_instance);
	}
	if (sample != NULL) {
		al_destroy_sample(sample);
	}
}

void AllegroSound::Load() {
	//if (id == 0) 
	//	return;
	Extract(id, filename, "WAVE");
	sample = al_load_sample(filename.c_str()); // al_destroy_sample
	if (sample == NULL)
		return; // throw std::runtime_error("Failed to load sound.");
	sample_instance = al_create_sample_instance(sample); // al_destroy_sample_instance
	if (sample_instance == NULL)
		return; // throw std::runtime_error("Failed to create sound.");
	al_set_sample_instance_playmode(sample_instance, flag);
	al_attach_sample_instance_to_mixer(sample_instance, al_get_default_mixer());
}

void AllegroSound::SetGain(float val) {
	if (sample_instance != NULL)
		al_set_sample_instance_gain(sample_instance, val);
}

void AllegroSound::Play() {
	if (sample_instance != NULL)
		al_play_sample_instance(sample_instance);
}

void AllegroSound::Stop() {
	if (sample_instance != NULL)
		al_stop_sample_instance(sample_instance);
}

bool AllegroSound::IsPlaying() {
	if (sample_instance == NULL)
		return false;
	return al_get_sample_instance_playing(sample_instance);
}
