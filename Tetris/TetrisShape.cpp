#include "stdafx.h"
#include "AllegroResources.h"
#include "TetrisShape.h"
#include "Util.h"

using namespace std;

TetrisShape::TetrisShape()
{
}

TetrisShape::TetrisShape(size_t id, long dimensions, TetrisSquares default_shape, ALLEGRO_COLOR color) :
	id(id), color(color), dimensions(dimensions), rotations()
{
	TetrisSquares coordinates = default_shape;
	rotations.push_back(coordinates);
	Rotate(coordinates);
	rotations.push_back(coordinates);
	Rotate(coordinates);
	rotations.push_back(coordinates);
	Rotate(coordinates);
	rotations.push_back(coordinates);
}

const TetrisSquares TetrisShape::GetRotation(size_t index)
{
	if (index >= 0 && index < rotations.size())
		return rotations[index];
	return TetrisSquares();
}

TetrisSquares TetrisShape::GetRamdom()
{
	size_t siz = rotations.size();
	if (siz > 0) {
		size_t index = DiceRoll(siz) - 1;
		return rotations[index];
	}
	return TetrisSquares();
}

void TetrisShape::Rotate(TetrisSquares& coordinates)
{
	for (TetrisSquare& point : coordinates) {
		float tmp = point.second;
		point.second = -point.first + dimensions - 1;
		point.first = tmp;
	}
}
