#pragma once

#include <string>
#include <vector>
#include <allegro5/color.h>
#include "Util.h"
#include "TetrisShape.h"
#include "BasicResources.h"

// An instance defines the colors and the names of the resource files (fonts,
// music, etc). And it extracts the embeded resources to the appdata folder.
class BasicResources
{
public:
	std::string appdata;
	ALLEGRO_COLOR color_default;
	ALLEGRO_COLOR color_clear_to;
	ALLEGRO_COLOR color_bg;
	ALLEGRO_COLOR color_white;
	ALLEGRO_COLOR color_gray;
	ALLEGRO_COLOR color_1;
	ALLEGRO_COLOR color_2;
	ALLEGRO_COLOR color_3;
	ALLEGRO_COLOR color_4;
	ALLEGRO_COLOR color_5;
	ALLEGRO_COLOR color_6;
	ALLEGRO_COLOR color_7;
	ALLEGRO_COLOR color_8;
	ALLEGRO_COLOR color_9;
	// fonts
	const char *fontname_arial;
	const char *fontname_verdana;
	const char *fontname_fixed;
	// music
	std::string wavname_theme1;
	std::string wavname_theme2;
	std::string wavname_tada;
	std::vector<TetrisShape> shape_types;
	std::vector<TetrisShape> oddshape_types;
	std::vector<TetrisShape> debris_types;

	BasicResources();
	~BasicResources();

	BasicResources(BasicResources const&) = delete;
	void operator=(BasicResources const&) = delete;
	static BasicResources& Instance()
	{
		static BasicResources instance;
		return instance;
	}

	// Returns the path to the folder we extract resources into.
	static std::string GetAppdata();
};