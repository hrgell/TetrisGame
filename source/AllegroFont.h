#pragma once
#include "allegro5/allegro_color.h"
#include "allegro5/allegro_font.h"

class AllegroFont
{
public:
	ALLEGRO_FONT *font;
	AllegroFont(const char *fontname, int size = 10, int flags = 0);
	~AllegroFont();
	operator ALLEGRO_FONT *() const { return font; }
	void Draw(ALLEGRO_COLOR color, float x, float y, int flags, const char *txt);
};

