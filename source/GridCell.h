#pragma once
#include <allegro5\color.h>
class GridCell
{
public:
	ALLEGRO_COLOR color;
	long explosion_age;
	GridCell();
	GridCell(ALLEGRO_COLOR color);
};

