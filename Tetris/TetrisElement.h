#pragma once

#include "TetrisShape.h"

class TetrisElement
{
public:
	// the top left on the screen
	float top;
	float left;
	size_t rotation;
	TetrisShape shape;
	// How long we must wait before the element MUST move down or be dumped.
	size_t waiting;
	size_t age;
	size_t explosion_age;
public:
	TetrisElement();
	TetrisElement(TetrisShape shape, float left, float top, size_t rotation);
	TetrisElement(TetrisShape shape, float left, float top);
	~TetrisElement();
	bool Waiting();
	size_t Wait();
	size_t Age();
	size_t EmptyTopRows();
};

