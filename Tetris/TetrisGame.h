#pragma once

#include "AllegroResources.h"
#include "TetrisPlayer.h"
#include "TetrisShape.h"
#include "TetrisElement.h"
#include "TetrisKey.h"
#include "TetrisKeyboard.h"
#include "TetrisPlayer.h"

class TetrisGame
{
public:
	BasicResources const& resources;
	AllegroResources &gb;
	bool is_tetris;
	long tick;
	size_t numcols;
	size_t numrows;
	bool started;
	TetrisKeyboard kbd1;
	TetrisKeyboard kbd2;
	TetrisPlayer player1;
	TetrisPlayer player2;
	float indent;
	float width;
	float height;
	float left;
	float right;
	float top;
	float bottom;
	FpsCounter fps_frame;
	FpsCounter fps_vert;
	FpsCounter fps_horz;
	FpsCounter fps_keyb;
	FpsCounter fps_second;
	FpsCounter fps_grid;

	TetrisGame(AllegroResources &gb);
	long timer_event(ALLEGRO_EVENT event);
	//void recalc();
	void RunGame();
	inline bool RunEvent(ALLEGRO_EVENT& event);
	void draw_unicode_char(char32_t ch, float dx = 0);
	bool ProcessGlobalKey(ALLEGRO_EVENT &event);
	bool SetWindowSize();
}; // class TetrisGame