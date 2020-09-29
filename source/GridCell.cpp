#include "GridCell.h"

GridCell::GridCell()
	: color({ 0, 0, 0 }), explosion_age(0)
{
}

GridCell::GridCell(ALLEGRO_COLOR color)
	: color(color), explosion_age(0)
{
}
