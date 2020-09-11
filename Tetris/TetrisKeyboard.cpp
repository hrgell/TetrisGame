#include "stdafx.h"
#include "TetrisKeyboard.h"

// TODO needs to be entirely rewritten
TetrisKeyboard::TetrisKeyboard()
	: fluentmove(true), rotate_clockwise(0), horizontal_move(0), vertical(0), horizontal(0), timestamp(0), horizontal_direction(0)
{
} // method

void TetrisKeyboard::Setup1()
{
	keymap[ALLEGRO_KEY_UP] = &k_up;
	k_up.id = ALLEGRO_KEY_UP;
	keymap[ALLEGRO_KEY_LEFT] = &k_left;
	k_left.id = ALLEGRO_KEY_LEFT;
	keymap[ALLEGRO_KEY_DOWN] = &k_down;
	k_down.id = ALLEGRO_KEY_DOWN;
	keymap[ALLEGRO_KEY_RIGHT] = &k_right;
	k_right.id = ALLEGRO_KEY_RIGHT;
	keymap[ALLEGRO_KEY_SPACE] = &k_space;
	k_space.id = ALLEGRO_KEY_SPACE;
	keymap[ALLEGRO_KEY_F5] = &k_F5;
	k_F5.id = ALLEGRO_KEY_F5;
} // method

void TetrisKeyboard::Setup2()
{
	keymap[ALLEGRO_KEY_W] = &k_up;
	k_up.id = ALLEGRO_KEY_W;
	keymap[ALLEGRO_KEY_A] = &k_left;
	k_left.id = ALLEGRO_KEY_A;
	keymap[ALLEGRO_KEY_S] = &k_down;
	k_down.id = ALLEGRO_KEY_S;
	keymap[ALLEGRO_KEY_D] = &k_right;
	k_right.id = ALLEGRO_KEY_D;
	keymap[ALLEGRO_KEY_SPACE] = &k_space;
	k_space.id = ALLEGRO_KEY_SPACE;
	keymap[ALLEGRO_KEY_F5] = &k_F5;
	k_F5.id = ALLEGRO_KEY_F5;
} // method

TetrisKeyboard::~TetrisKeyboard()
{
}

void TetrisKeyboard::Add(long ident, TetrisKey *key)
{
	keymap[ident] = key;
}

TetrisKey *TetrisKeyboard::Lookup(long ident)
{
	return keymap[ident];
}

long IncreaseOrToggle(long value) {
	return (value < 0) ? 1 : value + 1;
}

long DecreaseOrToggle(long value) {
	return (value > 0) ? -1 : value - 1;
}

void TetrisKeyboard::KeyboardDown(long keycode)
{
	++timestamp;
	auto itr = keymap.find((long)keycode);
	if (itr == keymap.end())
		return;
	TetrisKey *key = itr->second;
	key->timestamp = timestamp;
	key->KeyDown();
	if (key == &k_up) {
		++rotate_clockwise;
		vertical = DecreaseOrToggle(vertical);
	}
	else if (itr->second == &k_down) {
		vertical = IncreaseOrToggle(vertical);
	}
	else if (itr->second == &k_left) {
		horizontal_direction = -1;
		horizontal = DecreaseOrToggle(horizontal);
	}
	else if (itr->second == &k_right) {
		horizontal_direction = 1;
		horizontal = IncreaseOrToggle(horizontal);
	}
} // method

void TetrisKeyboard::KeyboardUp(long keycode)
{
	auto itr = keymap.find(keycode);
	if (itr == keymap.end())
		return;
	TetrisKey *key = itr->second;
	key->KeyUp();
	horizontal_direction = 0;
	if (key == &k_left) {
		if (k_right.IsDown()) {
			horizontal_direction = 1;
			horizontal_move = 1;
		}
	}
	else if (key == &k_right) {
		if (k_left.IsDown()) {
			horizontal_direction = -1;
			horizontal_move = -1;
		}
	}
} // method

void TetrisKeyboard::KeyboardUnPressAll() {
	for (auto itr : keymap)
		itr.second->UnPress();
} // method