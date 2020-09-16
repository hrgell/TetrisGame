#pragma once

#include <allegro5/color.h>

typedef std::pair<long, long> TetrisSquare;
typedef std::vector<TetrisSquare> TetrisSquares;
typedef std::vector<TetrisSquares> TetrisRotations;

class TetrisShape
{
public:
	size_t id;
	ALLEGRO_COLOR color;
	long dimensions; // the dimensions of the bounding box of the shape
	TetrisRotations rotations;

	TetrisShape();
	TetrisShape(size_t id, long dimensions, TetrisSquares coordinates, ALLEGRO_COLOR color);
	const TetrisSquares GetRotation(size_t rotation);
	TetrisSquares GetRamdom();
private:
	void Rotate(TetrisSquares& coordinates);
};

