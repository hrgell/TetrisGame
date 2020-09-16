#include "stdafx.h"
#include "AllegroResources.h"
#include "TetrisElement.h"
#include "Util.h"


TetrisElement::TetrisElement()
	: shape(), left(0), top(0), rotation(0), waiting(0), age(0)
{
}

TetrisElement::TetrisElement(TetrisShape shape, float left, float top, size_t rotation)
	: shape(shape), left(left), top(top), rotation(rotation), waiting(0), age(0)
{
}

TetrisElement::TetrisElement(TetrisShape shape, float left, float top)
	: shape(shape), left(left), top(top), rotation(0), waiting(0), age(0)
{
	//rotation = dice_roll(shape.rotations.size()) - 1;
}

TetrisElement::~TetrisElement()
{
}

bool TetrisElement::Waiting()
{
	return (waiting > 0);
}

size_t TetrisElement::Wait()
{
	if(waiting == 0)
		return 0;
	return --waiting;
}

size_t TetrisElement::Age()
{
	return ++age;
}

size_t TetrisElement::EmptyTopRows() {
	TetrisSquares squres = shape.GetRotation(rotation);
	size_t val = 0;
	for (TetrisSquare squre : squres) {
		if (squre.second > static_cast<long>(val))
			val = squre.second;
	}
	return shape.dimensions - val - 1;
}