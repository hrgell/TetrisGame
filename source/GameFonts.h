#pragma once
#include "AllegroFont.h"
#include "BasicResources.h"

class GameFonts
{
public:
	BasicResources const& resources;
	AllegroFont big;
	AllegroFont header1;
	AllegroFont header2;
	AllegroFont normal;
	AllegroFont fixed;
	GameFonts();
	~GameFonts();
};

