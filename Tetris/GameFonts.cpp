#include "stdafx.h"
#include "GameFonts.h"

GameFonts::GameFonts()
	:
	resources(BasicResources::Instance()),
	big(resources.fontname_arial, 30),
	header1(resources.fontname_arial, 20),
	header2(resources.fontname_verdana, 16),
	normal(resources.fontname_arial, 10),
	fixed(resources.fontname_fixed, 16)
{
	if (big.font != NULL) {
		DisplayFrame();
		big.Draw(resources.color_white, 18, 30, ALLEGRO_ALIGN_LEFT, "Loading ...");
		DisplayFrame();
	}
}

GameFonts::~GameFonts()
{
}