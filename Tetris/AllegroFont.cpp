#include "stdafx.h"
#include "AllegroFont.h"


AllegroFont::AllegroFont(const char *fontname, int size, int flags)
	: font(al_load_ttf_font(fontname, size, flags))
{
	if (font == NULL)
		throw std::runtime_error("Failed to load font ");
}

AllegroFont::~AllegroFont()
{
	if (font != NULL)
		al_destroy_font(font);
}

void AllegroFont::Draw(ALLEGRO_COLOR color, float x, float y, int flags, const char *txt)
{
	if(font != NULL)
		al_draw_text(font, color, x, y, flags, txt);
}