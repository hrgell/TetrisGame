#include "stdafx.h"
#include "AllegroResources.h"
#include "TetrisPlayer.h"
#include "Util.h"

#include <stack>

// Todo Work on how keys are pressed and the movement of tetris pieces, how they move and snap to the grid.

TetrisPlayer::TetrisPlayer(AllegroResources& gb)
	:
	right(0), bottom(0), unitx(0), unity(0), width(0), height(0), indentx(0), indenty(0), kbd(),
	resources(BasicResources::Instance()),
	gb(gb),
	glidingmove(true),
	numcols(0), numrows(0), siz(0), debristimer(-1), grid(), shapes(), debris(), explosions(), playing(false), score(0), left(0), top(0), opponent(NULL),
	numdumps(0), toprows(8), bottomrows(5), replay_text1("PRESS F5"), replay_text2("GAME OVER"), replay_text(replay_text1)
{
}


TetrisPlayer::~TetrisPlayer() {
}

void TetrisPlayer::SetKeyboard(TetrisKeyboard* kbd) {
	this->kbd = kbd;
}

void TetrisPlayer::SetPosition(float left, float top)
{
	this->left = left;
	this->top = top;
}

void TetrisPlayer::SetSize(size_t numcols, size_t numrows, long unitx, long unity)
{
	score = 0;
	if (debris.size() > 0)
		debris.clear();
	if (explosions.size() > 0)
		explosions.clear();
	if (grid.size() > 0)
		grid.clear();
	debristimer = -1;
	this->unitx = unitx;
	this->unity = unity;
	this->numcols = numcols;
	this->numrows = numrows;
	siz = numcols * numrows;
	indentx = unitx / 2;
	indenty = unity / 2;
	width = unitx * numcols + indentx * 2;
	height = unity * numrows + indenty * 2;
	right = left + width;
	bottom = top + height;
	if (siz == 0)
		return;
	for (size_t idx = 0; idx < siz; ++idx)
		grid.push_back(resources.color_default);
}

ALLEGRO_COLOR TetrisPlayer::Get(size_t row, size_t col) //xxx
{
	if (row >= numrows || col >= numcols)
		return resources.color_default;
	size_t pos = row * numcols + col;
	return grid[pos];
}

void TetrisPlayer::Set(size_t row, size_t col, ALLEGRO_COLOR val)
{
	if (row >= numrows || col >= numcols)
		return;
	size_t pos = row * numcols + col;
	grid[pos] = val;
}

bool TetrisPlayer::IsDefault(ALLEGRO_COLOR color) {
	return (color.r == resources.color_default.r && color.g == resources.color_default.g && color.b == resources.color_default.b);
}

bool TetrisPlayer::RowIsFull(size_t row) {
	for (size_t col = 0; col < numcols; ++col) {
		ALLEGRO_COLOR color = Get(row, col);
		if (IsDefault(color))
			return false;
	}
	return true;
}

bool TetrisPlayer::RowIsEmpty(size_t row) {
	for (size_t col = 0; col < numcols; ++col) {
		ALLEGRO_COLOR color = Get(row, col);
		if (!IsDefault(color))
			return false;
	}
	return true;
}

// @return Returns true if the player completed one or more rows.
bool TetrisPlayer::Collapse(TetrisPlayer* grid) {
	if (numrows == 0)
		return false;
	if (numcols == 0)
		return false;
	long dst = static_cast<long>(numrows - 1);
	long row;
	size_t completed_rows = 0;

	// count full rows and redraw only the non-full rows.
	for (row = dst; row >= 0; --row) {
		if (RowIsFull(row)) {
			++completed_rows;
			continue;
		}
		if (row != dst) {
			for (size_t col = 0; col < numcols; ++col) {
				ALLEGRO_COLOR color = (row == 0) ? resources.color_default : Get(row, col);
				Set(dst, col, color);
			}
		}
		--dst;
	}

	// reward the player
	if (completed_rows > 0) {
		if (completed_rows == 1)
			Earn(600);
		else if (completed_rows == 2)
			Earn(1800);
		else if (completed_rows == 3)
			Earn(6300);
		else // if (completed_rows == 4)
			Earn(25200);
	}

	// draw the remaining empty rows
	for (row = dst; row >= 0; --row)
		for (size_t col = 0; col < numcols; ++col)
			Set(row, col, resources.color_default);

	// punish the other player
	if (opponent != NULL && completed_rows > 1) {
		opponent->Transfer(completed_rows - 1);
	}

	// the player completed one or more rows ?
	return (completed_rows > 0);
}

size_t TetrisPlayer::Earn(size_t num) {
	score += num;
	return score;
}

void TetrisPlayer::DrawText()
{
	//gb.fonts.font1.Draw(color_9, left + indentx + 1, top + indenty + 1, ALLEGRO_ALIGN_LEFT, "Press F5 to restart");
	//al_draw_rectangle(left, top, left + 8, top + 8, color, 0);
	std::string txt = string_format("%ld", score);
	gb.fonts.header1.Draw(resources.color_white, left + indentx + unitx * numcols / 2, top - unity * (toprows - 2), ALLEGRO_ALIGN_CENTER, "TETRIS");
	gb.fonts.header1.Draw(resources.color_white, left + indentx + unitx * numcols / 2, bottom + unity / 1.5, ALLEGRO_ALIGN_CENTER, "SCORE");
	gb.fonts.fixed.Draw(resources.color_white, left + indentx + unitx * numcols / 2, bottom + unity * 2, ALLEGRO_ALIGN_CENTER, txt.c_str());
	if (!playing)
		gb.fonts.header1.Draw(resources.color_white, left + indentx + unitx * numcols / 2, top + unity * numrows / 2, ALLEGRO_ALIGN_CENTER, replay_text);
}

bool TetrisPlayer::DrawGrid() {
	bool isempty = true;
	if (numrows == 0)
		return isempty;
	if (numcols == 0)
		return isempty;
	bool collapsed_something = Collapse();
	if (collapsed_something)
		gb.audio.PlayNice();
	for (size_t row = 0; row < numrows; ++row) {
		for (size_t col = 0; col < numcols; ++col) {
			ALLEGRO_COLOR color = Get(row, col);
			if (IsDefault(color))
				continue;
			isempty = false;
			float x = left + indentx + col * unitx;
			float y = top + indenty + row * unity;
			//debug(string_format("--- row %u col %u row %.2f col %.2f", row, column, x , y1));
			DrawSquare(x, y, color);
		}
	}
	return isempty;
}

bool TetrisPlayer::RemoveSquare() {
	if (playing)
		return false;
	if (numrows == 0)
		return false;
	if (numcols == 0)
		return false;
	for (size_t row = 0; row < numrows; ++row) {
		for (size_t col = 0; col < numcols; ++col) {
			ALLEGRO_COLOR color = Get(row, col);
			if (color.r == 0 && color.g == 0 && color.b == 0)
				continue;
			Set(row, col, resources.color_default);
			return true;
		}
	}
	return false;
}

void TetrisPlayer::SwapMoveMethod()
{
	glidingmove = !glidingmove;
}

void TetrisPlayer::DrawSquare(float x, float y, ALLEGRO_COLOR color) {
	al_draw_filled_rectangle(x, y, x + unitx - 1, y + unity - 1, color);
	//float direction = 3, dy = 3, d = unitx / 3 - 1;
	//al_draw_line(x + direction, y1 + dy + d / 2 - 1, x + direction + d, y1 + dy + d / 2 - 1, resources.color_white, 1);
	//al_draw_line(x + direction + d / 2, y1 + dy, x + direction + d / 2, y1 + dy + d, resources.color_white, 1);
}

bool TetrisPlayer::DrawExplosion(TetrisElement& elem, float dx, float dy) {
	if (elem.explosion_age > 100)
		return false;
	TetrisSquares coordinates = elem.shape.GetRotation(elem.rotation);
	for (TetrisSquare& point : coordinates) {
		float x = elem.left + unitx * point.first;
		float y = elem.top + elem.shape.dimensions * unity - unity * (point.second + 1);
		//DrawSquare(x + dx, y + dy, elem.shape.color);
		auto siz = unitx / 2;
		auto radius = unitx / 2;	
		long radius2 = 5 - elem.explosion_age / 10;
		if (radius2 < 0)
			radius = 1;
		else if (radius2 < radius)
			radius = radius2;
		al_draw_filled_circle(x + dx + siz, y + dy + siz, radius, { 0.7, 0.9, 0.84 });
	}
	if (gb.developing)
		DrawBoundingBox(elem.left + dx, elem.top + dy, elem.shape);
}

void TetrisPlayer::DrawBoundingBox(float x, float y, TetrisShape shape) {
	al_draw_rectangle(x - 1, y - 1, x + unitx * shape.dimensions + 1, y + unity * shape.dimensions + 1, shape.color, 0);
}

TetrisPosition TetrisPlayer::GetInitialShapePosition() {
	float x = left + indentx + numcols / 2 * unitx - 1.8 * unitx;
	float y = top - unity * toprows + unity * 6;
	return { x , y };
}

TetrisPosition TetrisPlayer::GetInitialDebrisPosition() {
	float x = left + indentx + (DiceRoll(numcols) - 1) * unitx;
	float y = top + indenty;
	return { x , y };
}

TetrisElement TetrisPlayer::GetRandomTetrisElement(float x, float y, size_t id) {
	size_t siz = resources.shape_types.size();
	assert(siz > 0);
	size_t idx;
	if (siz == 1)
		idx = 0;
	else if (id == 0xffff)
		idx = DiceRoll(siz) - 1;
	else {
		idx = DiceRoll(siz - 1) - 1;
		if (resources.shape_types[idx].id == id)
			idx = siz - 1;
	}
	return TetrisElement(resources.shape_types[idx], x, y);
}

void TetrisPlayer::NextShape()
{
	size_t siz = shapes.size();
	size_t elemtype = 0xffff;
	if (siz > 0) {
		TetrisElement& elem = shapes.front();
		elemtype = elem.shape.id;
		shapes.pop();
	}
	TetrisPosition point = GetInitialShapePosition();
	shapes.push(GetRandomTetrisElement(point.first, point.second, elemtype));
	TetrisElement& elem = shapes.front();
	elemtype = elem.shape.id;
	if (siz < 2)
		shapes.push(GetRandomTetrisElement(point.first, point.second, elemtype));
	elem = shapes.front();
	elem.top = top + indenty - elem.EmptyTopRows() * unity;
	elem.left = left + indentx + 4 * unitx;
}

void TetrisPlayer::OddShape()
{
	if (!playing)
		return;
	size_t siz = shapes.size();
	TetrisElement elem = shapes.front();
	while (shapes.size() > 0)
		shapes.pop();
	shapes.push(elem);
	TetrisPosition point = GetInitialShapePosition();
	shapes.push(TetrisElement(resources.oddshape_types[0], point.first, point.second));
}

// Must only be called once per vertical tick.
bool TetrisPlayer::DropDebris()
{
	if (!playing)
		return true;
	if (debristimer < 0)
		return true;
	if (debristimer-- % 34 != 0)
		return true;
	TetrisPosition point = GetInitialDebrisPosition();
	debris.push_back(TetrisElement(resources.debris_types[0], point.first, point.second));
	return true;
	//TetrisElement& elem = debris.front();
	//return (TestCollision(elem) == COLLISION_NONE);
}

TetrisSquare TetrisPlayer::GetRelativePositionOfSquare(TetrisShape shape, TetrisSquare point) {
	float x = unitx * point.first;
	float y = unity * (shape.dimensions - point.second - 1);
	return { x , y };
}

void TetrisPlayer::DrawRotations(TetrisShape& shape, float x, float y) {
	for (TetrisSquares& coordinates : shape.rotations) {
		for (TetrisSquare& point : coordinates) {
			TetrisSquare coord = GetRelativePositionOfSquare(shape, point);
			DrawSquare(x + coord.first, y + coord.second, shape.color);
		}
		DrawBoundingBox(x, y, shape);
		x += 80;
	}
}

void TetrisPlayer::DrawAllRotations(float x, float y) {
	for (TetrisShape shape : resources.shape_types) {
		DrawRotations(shape, x, y);
		y += 80;
	}
}

void TetrisPlayer::DrawShape(TetrisElement& elem, float dx, float dy) {
	TetrisSquares coordinates = elem.shape.GetRotation(elem.rotation);
	for (TetrisSquare& point : coordinates) {
		float x = elem.left + unitx * point.first;
		float y = elem.top + elem.shape.dimensions * unity - unity * (point.second + 1);
		DrawSquare(x + dx, y + dy, elem.shape.color);
	}
	if (gb.developing)
		DrawBoundingBox(elem.left + dx, elem.top + dy, elem.shape);
}

bool TetrisPlayer::DrawFrame()
{
	DrawGameBoard();
	bool isempty = DrawGrid();
	// draw the falling piece controled by the player
	if (playing || !isempty)
		DrawShape(shapes.front());
	// draw the hint for the next piece
	// todo move calculation dx,dy - howto center the piece elsewhere
	auto& next = shapes.back();
	float dx = (4 - next.shape.dimensions) * unitx / 2;
	float dy = (4 - next.shape.dimensions) * unity / 2 - 2 * unity;
	DrawShape(next, dx, dy);
	// draw debris
	for (auto& elem : debris)
		DrawShape(elem);
	// draw explosions
	std::stack<size_t> positions;
	size_t pos = 0;
	for (auto& elem : explosions) {
		if (!DrawExplosion(elem, 0, 0))
		//if (!DrawExplosion(elem.top, elem.left, { 0.9, 0.9, 0.9 }, elem.explosion_age))
				positions.push(pos);
		elem.explosion_age++;
		++pos;
	}
	DeleteFromDeque(explosions, positions);
	// draw the info about the game state
	DrawText();
	return true;
}

void TetrisPlayer::DrawGameBoard() {
	// draw game board
	al_draw_filled_rectangle(left, top - unitx * toprows, left + indentx, bottom + unitx * bottomrows, resources.color_white);
	al_draw_filled_rectangle(right - indentx, top - unitx * toprows, right, bottom + unitx * bottomrows, resources.color_white);
	float myleft = left + indentx;
	float myright = right - indenty;
	al_draw_filled_rectangle(myleft, top, myright, top + indenty, resources.color_white);
	al_draw_filled_rectangle(myleft, bottom - indenty, myright, bottom, resources.color_white);
	al_draw_filled_rectangle(myleft, top - unitx * toprows, myright, top - unitx * toprows + indenty, resources.color_white);
	al_draw_filled_rectangle(myleft, bottom + unitx * bottomrows - indenty, myright, bottom + unitx * bottomrows, resources.color_white);
	// draw developer grid
	if (gb.developing) {
		float y1 = top + indenty;
		float y2 = bottom - indenty;
		for (float x = myleft + unitx; x < myright; x += unitx) {
			al_draw_line(x, y1, x, y2, resources.color_white, 1);
		}
		while ((y1 += unity) < y2) {
			al_draw_line(myleft, y1, myright, y1, resources.color_white, 1);
		}
	}
}


void TetrisPlayer::Rotate(bool clockwise)
{
	TetrisElement& elem = shapes.front();
	size_t siz = elem.shape.rotations.size();
	if (clockwise) {
		if (++elem.rotation >= siz)
			elem.rotation = 0;
	}
	else if (elem.rotation > 0)
		--elem.rotation;
	else
		elem.rotation = siz - 1;
}

TetrisPosition TetrisPlayer::GetCoordsOfShapepoint(TetrisElement& elem, TetrisSquare& point) {
	float x = elem.left - left - indentx + point.first;
	float y = elem.top - top - indenty + point.second;
	return { x, y };
}

float TetrisPlayer::CopyPieceToGrid(TetrisElement& elem) {
	float highest_row = 0;
	TetrisSquares coordinates = elem.shape.GetRotation(elem.rotation);
	for (TetrisSquare& point : coordinates) {
		TetrisSquare square = GetRelativePositionOfSquare(elem.shape, point);
		TetrisPosition coord = GetCoordsOfShapepoint(elem, square);
#pragma warning( suppress: 26451 )
		float columns = round(coord.first / unitx);
#pragma warning( suppress: 26451 )
		float rows = floor(coord.second / unity);
		if (highest_row < rows)
			highest_row = rows;
		Set(rows, columns, elem.shape.color);
	}
	return highest_row;
}

bool TetrisPlayer::Dump(TetrisElement& elem) {
	//al_draw_filled_rectangle(elem.left, elem.top, elem.left + 8, elem.top + 8, tetris_color_white);
	float highest_row = CopyPieceToGrid(elem);
	if (highest_row > 0) {
		size_t offset = (numrows - highest_row) / 5;
		Earn(1 + offset);
	}
	NextShape();

	return (collide(shapes.front(), 0, 0) == COLLISION_NONE);
}

// TODO animate the insertion of rows
bool TetrisPlayer::Transfer(size_t rows) {

	if (rows == 1) {
		debristimer += 100; // DropDebris();
	}
	else if (rows == 2) {
		OddShape();
	}
	if (rows >= numrows || rows < 1)
		return 0; // ok, ignore
	size_t row;
	for (row = 0; row < numrows && RowIsEmpty(row); ++row)
		;
	size_t blankrows = row;
	ALLEGRO_COLOR color;
	size_t nextrow = (rows > blankrows) ? rows : blankrows;
	for (row = nextrow; row < numrows; ++row) {
		long dst = static_cast<long>(row - rows);
		for (size_t col = 0; col < numcols; ++col) {
			color = Get(row, col);
			Set(dst, col, color);
		}
	}
	size_t rst = DiceRoll(numcols) - 1;
	for (row = numrows - rows; row < numrows; ++row) {
		for (size_t col = 0; col < numcols; ++col) {
			Set(row, col, (rst == col) ? resources.color_default : resources.color_gray);
		}
	}
	// TODO Adjust the position of the falling piece
	TetrisElement& elem = shapes.front();
	while (collide(elem, 0, 0) != COLLISION_NONE) {
		elem.top--;
	}

	return (blankrows < rows);
}

long TetrisPlayer::SyncWithGrid(TetrisElement& elem) {
	SyncWithRow(elem);
	SyncWithCol(elem);
	return 0;
}

long TetrisPlayer::SyncWithRow(TetrisElement& elem) {
	float y = elem.top - top - indenty;
	float rows = floor(y / unity);
	elem.top = top + indenty + rows * unity;
	return 0;
}

long TetrisPlayer::SyncWithCol(TetrisElement& elem) {
	float x = elem.left - left - indentx;
	float cols = 0;
	if (kbd->horizontal_move < 0) {
		cols = floor(x / unitx);
	}
	else if (kbd->horizontal_move > 0) {
		cols = ceil(x / unitx);
	}
	else
		cols = round(x / unitx);
	elem.left = left + indentx + cols * unitx;
	return 0;
}

TetrisPosition TetrisPlayer::GetBoundingBoxCoordinates(TetrisElement& elem, float dx, float dy)
{
	float x = elem.left - left - indentx + dx;
	float y = elem.top - top - indenty + dy;
	return { x, y };
}

TetrisPosition TetrisPlayer::GetSquareCoordinates(TetrisElement& elem, TetrisPosition bbox, TetrisSquare point) {
	TetrisSquare coord = GetRelativePositionOfSquare(elem.shape, point);
	float x = bbox.first + coord.first;
	float y = bbox.second + coord.second;
	return { x, y };
}

// TODO implement it or delete it
long TetrisPlayer::CheckDebrisCollision(std::pair<long, long>& point)
{
	float x = point.first / unitx;
	float y = point.second / unity;
	long cols = (long)round(x);
	long rows = (long)round(y);
	long collision = CheckCollisionWithGamebox(point, cols, rows);
	// abort if we already got a collision.
	if (collision != COLLISION_NONE)
		return collision;
	if (rows >= 0) {
		ALLEGRO_COLOR color = Get(rows, cols);
		if (IsDefault(color))
			return COLLISION_NONE;
		collision |= COLLISION_SQUARE;
	}
	return collision;
}

long TetrisPlayer::CheckCollision(std::pair<long, long>& point)
{
	float x = point.first / unitx;
	float y = point.second / unity;
	long cols = (long)round(x);
	long rows = (long)round(y);
	long collision = CheckCollisionWithGamebox(point, cols, rows);
	// abort if we already got a collision.
	if (collision != COLLISION_NONE)
		return collision;
	if (rows >= 0) {
		ALLEGRO_COLOR color = Get(rows, cols);
		if (IsDefault(color))
			return COLLISION_NONE;
		collision |= COLLISION_SQUARE;
	}
	return collision;
}

long TetrisPlayer::CheckCollisionWithGamebox(std::pair<long, long>& point, long cols, long rows)
{
	if (point.first < 0)
		return COLLISION_LEFT;
	if (cols >= (long)numcols)
		return COLLISION_RIGHT;
	//if (point.second < 0)
	//	return COLLISION_UP;
	if (rows >= (long)numrows)
		return COLLISION_DOWN;
	return COLLISION_NONE;
}

long TetrisPlayer::collide(TetrisElement& elem, float dx, float dy) {
	//al_draw_filled_rectangle(elem.left, elem.top, elem.left + 8, elem.top + 8, tetris_color_white);
	long rst = COLLISION_NONE;
	TetrisPosition bbox = GetBoundingBoxCoordinates(elem, dx, dy);
	for (TetrisSquare& point : elem.shape.rotations[elem.rotation]) {
		std::pair<long, long> square = GetSquareCoordinates(elem, bbox, point);
		// Determine the position in the grid
		rst |= CheckCollision(square);
		square.first += unitx - 1;
		rst |= CheckCollision(square);
		square.second += unity - 1;
		rst |= CheckCollision(square);
		square.first -= unitx - 1;
		rst |= CheckCollision(square);
		square.second -= unity - 1;
	}
	return rst;
}

// Returns COLLISION_SQUARE if the two pieces intersect.
long TetrisPlayer::collide_wit_debris(TetrisElement& elem, TetrisElement& elem2, float dx, float dy) {
	// For each square in the the first element
	TetrisPosition bbox = GetBoundingBoxCoordinates(elem, dx, dy);
	for (TetrisSquare& point : elem.shape.rotations[elem.rotation]) {
		std::pair<long, long> square = GetSquareCoordinates(elem, bbox, point);
		// For each square in the next element
		TetrisPosition bbox2 = GetBoundingBoxCoordinates(elem2, dx, dy);
		for (TetrisSquare& point2 : elem2.shape.rotations[elem2.rotation]) {
			std::pair<long, long> square2 = GetSquareCoordinates(elem2, bbox2, point2);
			// Test if the squares overlap.
			if (!((square2.first > square.first + unitx - 1) // right of
				|| (square2.first + unitx - 1 < square.first) // left of
				|| (square2.second + unity - 1 < square.second) // above
				|| (square2.second > square.second + unity - 1))) // below
				return COLLISION_SQUARE;
		}
	}
	return COLLISION_NONE;
}


void TetrisPlayer::ProcessImmediateKeyboardInput()
{
	// TODO rotate elsewhere and do it right
	while (kbd->rotate_clockwise > 0) {
		//--kbd->rotate_clockwise;
		kbd->rotate_clockwise = 0;
		Rotate(true);
		long collision = collide(shapes.front(), 0, 0);
		// snap to row
		if (collision != COLLISION_NONE) {
			// TODO if a collision occurs displace the bounding box. Let the new bounding
			// box be within the grid. This moves the center of the rotation for the shape,
			// ie. unrotate, set new center, rotate again, check again and reset the center
			// of rotation when finished testing and rotating.
			// TODO Dont sync with row, use a displaced bounding box instead.
			auto elem = shapes.front();
			SyncWithRow(elem);
			if (collide(elem, 0, 0) != COLLISION_NONE) {
				//SyncWithCol(shapes.front());
				//long collision = collide(shapes.front(), 0, 0);
				Rotate(false);
			}
		}
	}

	if (kbd->k_down.IsDown()) {
		bool downmethod = true;
		if (!downmethod) kbd->k_down.KeyUp();
		TetrisElement& elem = shapes.front();
		float y = 0;
		long steps = 0;
		while (collide(elem, 0, unity * steps) == COLLISION_NONE)
			++steps;
		if (steps > 0) {
			if (downmethod) {
				float newtop = elem.top + unity * 0.1;
				elem.top = newtop;
			}
			else {
				float newtop = elem.top + unity * (steps - 7);
				if (newtop > elem.top)
					elem.top = newtop;
				else
					elem.top = elem.top + unity * (steps - 1);
			}
		}
	}
}

bool TetrisPlayer::IsVerticalyAligned(TetrisElement& elem)
{
	float x = elem.left - left - indentx;
	float col = round(x / unitx);
	if (x == col * unitx)
		return true;
	return false;
}

long TetrisPlayer::HorizontalMove(TetrisElement& elem, long dx) {
	long success = (collide(elem, dx, 0) == COLLISION_NONE);
	if (success)
		elem.left += dx;
	return success;
}

bool TetrisPlayer::DoHorizontalMoveV1(TetrisElement& elem)
{
	long direction = 0;
	if (kbd->horizontal_direction < 0)
		direction = -1;
	else if (kbd->horizontal_direction > 0)
		direction = 1;
	else if (kbd->horizontal_move < 0)
		direction = -1;
	else if (kbd->horizontal_move > 0)
		direction = 1;

	bool success = true;
	if (direction != 0) {
		success = HorizontalMove(elem, direction);
		if (success)
			kbd->horizontal_move = direction;
	}

	if (IsVerticalyAligned(elem) || !success)
		kbd->horizontal_move = 0;

	return success;
}

bool TetrisPlayer::DoHorizontalMoveV2(TetrisElement& elem)
{
	if (kbd->horizontal == 0)
		return true;
	bool success = true;
	long direction = ((kbd->horizontal < 0) ? -1 : 1);
	// move twice as fast, call HorizontalMove twice.
	success = HorizontalMove(elem, direction);
	if (success)
		HorizontalMove(elem, direction);
	// stop moving the piece if it is on a boundary
	if (!success || IsVerticalyAligned(elem))
		kbd->horizontal = 0;
	// return true if the piece could move.
	return success;
}

long TetrisPlayer::MoveDown(TetrisElement& elem) {
	long collision = collide(elem, 0, 1);
	if (collision == COLLISION_NONE) {
		auto oldtop = elem.top;
		elem.top++;
		collision = collide(elem, 0, 0);
		if (collision != COLLISION_NONE)
			elem.top = oldtop;
	}
	if (collision == COLLISION_NONE) {
		elem.waiting = 0;
	}
	return collision;
}

bool TetrisPlayer::DoVerticalMove(TetrisElement& elem, float fps)
{
	long collision = MoveDown(elem);
	if (collision == COLLISION_NONE)
		return true;

	if (elem.Waiting()) {
		if (elem.Wait() > 0)
			return true;
		SyncWithGrid(elem);
		collision = MoveDown(elem);
		if (collision != COLLISION_NONE) {
			gb.audio.PlayMove();
			playing = Dump(elem);
			if (!playing)
				replay_text = replay_text2;
			return false;
		}
	}
	// todo if element has been in the same position two thirds of the waiting time, then dump it if its blocked.
	elem.waiting = fps / 2;
	return true;
}

// delete debris we don't need.
// we assume the values on the stack gets lower each time we pop a value, otherwise using debris.erase() will not work.
void TetrisPlayer::DeleteFromDeque(std::deque<TetrisElement>& deq, std::stack<size_t>& positions)
{
	while (!positions.empty()) {
		auto idx = positions.top();
		//DebugWriteMsg(string_format("\t\tTOP: %u / %u\r\n", (USHORT)idx, (USHORT)deq.size()));
		deq.erase(deq.begin() + idx);
		positions.pop();
	}
}

bool TetrisPlayer::DoVerticalDebrisMove()
{
	std::stack<size_t> positions;
	size_t pos = 0;
	for (auto& debr : debris) {
		++pos;
		bool touches = false;
		long collision = MoveDown(debr);
		if (collision != COLLISION_NONE) {
			SyncWithGrid(debr);
			collision = MoveDown(debr);
			if (collision != COLLISION_NONE) {
				gb.audio.PlayMove();
				// todo we may fail if dump fails.
				CopyPieceToGrid(debr);
				touches = true;
			}
		}
		if (collide_wit_debris(shapes.front(), debr, 0, 0) != COLLISION_NONE) {
			explosions.push_back(debr);
			touches = true;
		}
		if (touches) {
			positions.push(pos - 1);
		}
	}

	DeleteFromDeque(debris, positions);
	return true;
}

void TetrisPlayer::Restart()
{
	numdumps = 0;
	SetSize(numcols, numrows, unitx, unity);
	NextShape();
	NextShape();
	playing = true;
}

std::string  TetrisPlayer::ToString() {
	TetrisElement& elem = shapes.front();
	return
		string_format("%.2f %.2f %.2f %.2f (%ld %ld) %ldx%ld %ld %ld",
			left, top, right, bottom, unitx, unity, static_cast<long>(numrows), static_cast<long>(numcols), static_cast<long>(elem.left), static_cast<long>(elem.top));
	//std::vector<TetrisShape> shape_types;
	//std::vector<ALLEGRO_COLOR> grid;
	//size_t score;
	//TetrisKeyboard *kbd;
}

long TetrisPlayer::TestCollisionCurent()
{
	return TestCollision(shapes.front());
	////std::string deb_colide;
	//long colides = collide(shapes.front(), 0, 0);
	//if (colides != COLLISION_NONE) {
	//	//deb_colide = "colide";
	//	SyncWithGrid(shapes.front());
	//	colides = collide(shapes.front(), 0, 0);
	//	if (colides != COLLISION_NONE) {
	//		//deb_colide += " failed";
	//	}
	//}
	//return colides;
}


long TetrisPlayer::TestCollision(TetrisElement& elem)
{
	//std::string deb_colide;
	TetrisElement& xxxx = elem;
	long colides = collide(elem, 0, 0);
	if (colides != COLLISION_NONE) {
		//deb_colide = "colide";
		SyncWithGrid(elem);
		colides = collide(elem, 0, 0);
		if (colides != COLLISION_NONE) {
			//deb_colide += " failed";
		}
	}
	return colides;
}

// @return True if movement could be made.
bool TetrisPlayer::ProcessMovement(AllegroResources& gb, FpsCounter& fps_vert, bool do_horz, bool do_vert)
{
	if (!playing)
		return false;

	ProcessImmediateKeyboardInput();
	// Unpress all keys
	kbd->KeyboardUnPressAll();
	// Press movement keys if they are down.
	if (!kbd->k_right.PressIfDown())
		kbd->k_left.PressIfDown();
	if (!kbd->k_up.PressIfDown())
		kbd->k_down.PressIfDown();
	// Test if the current shape can fall
	if (TestCollisionCurent() != COLLISION_NONE)
		return false;
	// process horizontal movement
	if (do_horz) {
		if (glidingmove)
			DoHorizontalMoveV1(shapes.front());
		else
			DoHorizontalMoveV2(shapes.front());
	}
	// process vertical movement
	if (do_vert) {
		++numdumps;
		DoVerticalDebrisMove();
		if (!DoVerticalMove(shapes.front(), fps_vert.fps)) {
			if (numdumps % 10 == 0) {
				Earn(50);
				if (numdumps % 40 == 0)
					fps_vert.speedup();
			}
		}
	}
	return true;
}