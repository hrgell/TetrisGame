#pragma once

#include <unordered_map>
#include "TetrisKey.h"

// TODO needs to be entirely rewritten
class TetrisKeyboard
{
public:
	bool fluentmove;
	long rotate_clockwise;
	long vertical;
	long horizontal;
	long horizontal_move;
	long horizontal_direction;
	unsigned long timestamp;
	std::unordered_map<long, TetrisKey *> keymap;
	TetrisKey k_up;
	TetrisKey k_down;
	TetrisKey k_left;
	TetrisKey k_right;
	TetrisKey k_space;
	TetrisKey k_F5;

	TetrisKeyboard();
	~TetrisKeyboard();
	void Add(long ident, TetrisKey *key);
	TetrisKey *Lookup(long ident);
	void KeyboardUp(long keycode);
	void KeyboardDown(long keycode);
	void KeyboardUnPressAll();
	void Setup1();
	void Setup2();
};

