#include "stdafx.h"
#include "windows.h"
#include "TetrisGame.h"
#include "Util.h"
#include "BasicResources.h"

using namespace std;

TetrisGame::TetrisGame(AllegroResources &gb) :
	resources(BasicResources::Instance()),
	gb(gb),
	is_tetris(true),
	tick(0),
	numcols(10),
	numrows(25),
	started(true),
	kbd1(),
	kbd2(),
	player1(gb),
	player2(gb)

{
	// Set fps counters
	fps_frame.Initialize(gb.fps, 85);
	fps_horz.Initialize(gb.fps, 180);
	fps_vert.Initialize(gb.fps, 80);
	fps_keyb.Initialize(gb.fps, 20);
	fps_second.Initialize(gb.fps, 1);
	fps_grid.Initialize(gb.fps, 18);

	//recalc();
	indent = gb.monitor.unitx;
	// Calculate the position and size of the display.
	width = gb.monitor.unitx * numcols + indent * 2;
	height = gb.monitor.unity * numrows + indent * 2;
	// centered
	left = gb.monitor.unitx;
	top = gb.monitor.unitx + gb.monitor.unity * 11;
	right = left + width;
	bottom = top + height;

	player1.SetKeyboard(&kbd1);
	player2.SetKeyboard(&kbd2);
	player1.SetPosition(left + 14 * gb.monitor.unitx, top);
	player2.SetPosition(left, top);

	player1.SetSize(numcols, numrows, gb.monitor.unitx, gb.monitor.unity);
	player2.SetSize(numcols, numrows, gb.monitor.unitx, gb.monitor.unity);
	player1.opponent = &player2;
	player2.opponent = &player1;
	//player1.playing = true;
	//player2.playing = true;
	kbd1.Setup1();
	kbd2.Setup2();
	player1.NextShape();
	player2.NextShape();
} // method

long TetrisGame::timer_event(ALLEGRO_EVENT event)
{
	bool do_grid = false;
	bool do_frame = false;

	// increase the tick count
	++tick;
	if (tick < 0) {
		tick = 0;
		fps_frame.position = fps_frame.fraction;
		fps_horz.position = fps_horz.fraction;
		fps_vert.position = fps_vert.fraction;
		fps_keyb.position = fps_keyb.fraction;
		fps_second.position = fps_second.fraction;
		fps_grid.position = fps_grid.fraction;
	}
	bool do_horz = fps_horz.Tick(tick);
	bool do_vert = fps_vert.Tick(tick);
	bool do_keyb = fps_keyb.Tick(tick);
	bool do_second = fps_second.Tick(tick);
	do_grid = fps_grid.Tick(tick);
	do_frame = fps_frame.Tick(tick);

	if (!gb.paused) {
		// first debris
		if (do_frame) {
			player1.DropDebris();
			player2.DropDebris();
		}

		// then animation
		player1.ProcessMovement(gb, fps_vert, do_horz, do_vert);
		player2.ProcessMovement(gb, fps_vert, do_horz, do_vert);
	}
	if (do_keyb) {
	}

	if (do_second) {
	}
	if (do_grid) {
		//player1.RemoveSquareWhenGameIsOver();
		//player2.RemoveSquareWhenGameIsOver();
		gb.audio.PlayTheme();
	}

	if (do_frame) {
		if (is_tetris) {
			player1.DrawFrame();
			player2.DrawFrame();
			//player1.DrawAllRotations(570, 70);
			auto xpos = left + (numcols + 2) * gb.monitor.unitx + player1.indentx;
			auto ypos = top - gb.monitor.unity * (player1.toprows + 3);
			gb.fonts.header2.Draw(resources.color_white, xpos, ypos, ALLEGRO_ALIGN_CENTER, "Press F5 to Restart");
			gb.fonts.small.Draw(resources.color_white, xpos, ypos + 22, ALLEGRO_ALIGN_CENTER, "Press M to toggle Music");
			if (false) {
				std::string rst = player1.ToString();
				gb.fonts.header1.Draw(resources.color_white, gb.monitor.unitx * 30, gb.monitor.unity, ALLEGRO_ALIGN_LEFT, rst.c_str());
			}
		}
		if (false) {
			// Test drawing a sprite in the upper left corner of the window
			static int xxxx = 0;
			auto siz = gb.sprite1.sprites.size();
			DebugWriteMsg(string_format("Length of sprites %d", (int)siz));
			int idx = 0;
			if (xxxx < 100)
				idx = 0;
			else if (xxxx < 200)
				idx = 1;
			else if (xxxx < 300)
				idx = 2;
			else if (xxxx < 400)
				idx = 3;
			else {
				idx = 0;
				xxxx = 0;
			}
			if (idx < siz) {
				auto bmp = gb.sprite1.sprites[idx];
				if (bmp != NULL)
					al_draw_bitmap(bmp, 0, 0, 0);
			}
			++xxxx;
		}
		// Show the frame
		DisplayFrame();
	}
	
	return 0;
} // method

void TetrisGame::draw_unicode_char(char32_t ch, float dx) {
	char buf[5];
	size_t siz1 = al_utf8_encode(buf, ch);
	if (siz1 > 0) {
		buf[siz1] = 0;
		ALLEGRO_USTR *str = al_ustr_newf("%s", buf);
		if (str != nullptr) {
			al_draw_ustr(gb.fonts.big, al_map_rgb(255, 255, 255), dx, 0, 0, str);
			al_ustr_free(str);
		}
	}
} // method

bool TetrisGame::ProcessGlobalKey(ALLEGRO_EVENT &event) {
	if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		return false;
	}
	else if (event.keyboard.keycode == ALLEGRO_KEY_F10) {
		player1.SwapMoveMethod();
		player2.SwapMoveMethod();
	}
	else if (event.keyboard.keycode == ALLEGRO_KEY_F11) {
		player2.debristimer += 100; // DropDebris();
		//is_tetris = !is_tetris;
		//SetWindowSize();
	}
	else if (event.keyboard.keycode == ALLEGRO_KEY_M) {
		gb.audio.ToggleMusic();
	}
	else if (event.keyboard.keycode == ALLEGRO_KEY_F5) {
		gb.paused = false;
		player1.Restart();
		player2.Restart();
		fps_vert.ReInitialize(80);
	}
	else if (event.keyboard.keycode == ALLEGRO_KEY_F6) {
		gb.paused = !gb.paused;
	}
	else if (event.keyboard.keycode == ALLEGRO_KEY_F3) {
		gb.developing = !gb.developing;
	}
	return true;
} // method

bool TetrisGame::SetWindowSize() {
	bool resized = false;
	float w, h;
	if (is_tetris) {
		float minwidth = gb.monitor.unitx * 27;
		float minheight = gb.monitor.unity * 45;
		bool newwidth = (gb.width != minwidth);
		bool newheight = (gb.height != minheight);
		if (newwidth || newheight) {
			w = (newwidth ? minwidth : gb.width);
			h = (newheight ? minheight : gb.height);
			resized = al_resize_display(gb.display, w, h);
		}
	}
	else {
		w = 1000;
		h = 700;
		resized = al_resize_display(gb.display, w, h);
	}
	if (resized) {
		gb.width = w;
		gb.height = h;
		float xx = (gb.monitor.horz_res - w) / 2;
		float yy = (gb.monitor.vert_res - h) / 4;
		al_set_window_position(gb.display, xx, yy);
	}
	return resized;
} // method

inline bool TetrisGame::RunEvent(ALLEGRO_EVENT& event)
{
	//al_get_keyboard_state(&keyboard_state);
	if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		return false;
	if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
		if (!ProcessGlobalKey(event))
			return false;
	}

	if (!gb.paused) {
		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			//char buf[100];
			//snprintf(buf, sizeof(buf), "mouse %u", event.mouse.button);
			//const char *msg = buf;
			//al_show_native_message_box(nullptr, "MOUSE", "MOUSE", msg, nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			auto val = event.mouse.button;
			if (val == 1)
				kbd1.KeyboardDown(kbd1.k_left.id);
			else if (val == 2)
				kbd1.KeyboardDown(kbd1.k_right.id);
			else if (val == 4)
				kbd1.KeyboardDown(kbd1.k_down.id);
			else if (val == 5)
				kbd1.KeyboardDown(kbd1.k_up.id);
			return true;
		}
		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			auto val = event.mouse.button;
			if (val == 1)
				kbd1.KeyboardUp(kbd1.k_left.id);
			else if (val == 2)
				kbd1.KeyboardUp(kbd1.k_right.id);
			else if (val == 4)
				kbd1.KeyboardUp(kbd1.k_down.id);
			else if (val == 5)
				kbd1.KeyboardUp(kbd1.k_up.id);
			return true;
		}
		if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			auto b = event.mouse.button;
			auto x = event.mouse.dx;
			auto y = event.mouse.dy;
			auto z = event.mouse.dz;
			if (z != 0) {
				if (z == 1) {
					kbd1.KeyboardDown(kbd1.k_up.id);
					kbd1.KeyboardUp(kbd1.k_up.id);
				}
				else { // if (z == -1) {
					kbd1.KeyboardDown(kbd1.k_up.id);
					kbd1.KeyboardUp(kbd1.k_up.id);
					// TODO instead:
					//  id = kbd1.k_down.id;
					//  kbd1.KeyboardDown(id);
					//  setTimeout(function delayed() { kbd1.KeyboardUp(id); }, 200);
				}
			}
			return true;
		}
		if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			kbd1.KeyboardDown(event.keyboard.keycode);
			kbd2.KeyboardDown(event.keyboard.keycode);
			return true;
		}
		if (event.type == ALLEGRO_EVENT_KEY_UP) {
			kbd1.KeyboardUp(event.keyboard.keycode);
			kbd2.KeyboardUp(event.keyboard.keycode);
			return true;
		}
	}
	if (event.type == ALLEGRO_EVENT_TIMER) {
		if (al_is_event_queue_empty(gb.event_queue)) {
			if (timer_event(event) != 0)
				return false;
		}
		return true;
	}
	return true;
}

void TetrisGame::RunGame()
{
	bool running = true;
	const float timeout = 0.1;
	SetWindowSize();
	al_start_timer(gb.timer);
	while (running) {
		ALLEGRO_EVENT event;
		al_wait_for_event(gb.event_queue, &event);
		double ts = al_current_time();
		do {
			running = RunEvent(event);
		} while (running && (al_current_time() - ts <= timeout) && al_get_next_event(gb.event_queue, &event));
	}
	started = false;
} // method