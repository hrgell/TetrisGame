#pragma once
#include "FpsCounter.h" // for convinience
#include "GameMonitor.h"
#include "GameAudio.h"
#include "GameFonts.h"
#include "Scope.h"
#include "AllegroSprite.h"

// We use RAII to manage the allegro resources.
class AllegroResources
{
public:
	bool developing;
	bool paused;
	GameMonitor monitor;
	int width;
	int height;
	double fps;
	ALLEGRO_DISPLAY *display;
	ALLEGRO_TIMER *timer;
	ALLEGRO_EVENT_QUEUE *event_queue;
private:
	Scope scope_base;
	Scope scope_display;
	Scope scope_font;
	Scope scope_ttffont;
public:
	GameFonts fonts;
private:
	Scope scope_primitives;
	Scope scope_keyboard;
	Scope scope_mouse;
	Scope scope_audio;
	Scope scope_acodec;
	Scope scope_image;
	Scope scope_timer;
	Scope scope_event_queue;
	Scope scope_event_kbd;
	Scope scope_event_mouse;
	Scope scope_event_timer;
	Scope scope_event_display;
public:
	GameAudio audio;
	AllegroSprite sprite1;
	AllegroResources() = delete;
	AllegroResources(int fps);
	~AllegroResources();
};
