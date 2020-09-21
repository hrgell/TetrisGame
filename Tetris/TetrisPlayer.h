#pragma once
#include "TetrisShape.h"
#include "TetrisElement.h"
#include "AllegroResources.h"
#include "TetrisKeyboard.h"
#include <stack>

typedef std::pair<float, float> TetrisPosition;
enum TetrisGameCollision { COLLISION_NONE = 0, COLLISION_LEFT = 1, COLLISION_RIGHT = 2, COLLISION_UP = 4, COLLISION_DOWN = 8, COLLISION_SQUARE = 16 };

class TetrisPlayer
{
public:
	BasicResources const& resources;
	AllegroResources &gb;
	bool glidingmove;
	float left;
	float top;
	float width;
	float height;
	float right;
	float bottom;
	long unitx;
	long unity;
	float indentx;
	float indenty;
	size_t numcols;
	size_t numrows;
	size_t siz;
	std::vector<ALLEGRO_COLOR> grid;
	std::queue<TetrisElement> shapes;
	std::deque<TetrisElement> debris;
	std::deque<TetrisElement> explosions;
	long debristimer;
	bool playing;
	size_t score;
	TetrisKeyboard *kbd;
	long numdumps;
	TetrisPlayer *opponent;
	size_t toprows;
	size_t bottomrows;
	const char* replay_text1;
	const char* replay_text2;
	const char* replay_text;
	//bool (TetrisPlayer::*horizontal_movment_handler2)(TetrisElement &) = NULL;
	typedef bool (TetrisPlayer::*movment_handler_t)(TetrisElement &);
public:
	TetrisPlayer() = delete;
	TetrisPlayer(AllegroResources &gb);
	~TetrisPlayer();
	void SetKeyboard(TetrisKeyboard *kbd);
	void SetSize(size_t numcols, size_t numrows, long sizex, long sizey);
	void SetPosition(float left, float top);
	ALLEGRO_COLOR Get(size_t row, size_t col);
	void Set(size_t row, size_t col, ALLEGRO_COLOR val);
	bool IsDefault(ALLEGRO_COLOR val);
	bool RowIsFull(size_t row);
	bool RowIsEmpty(size_t row);
	bool Collapse(TetrisPlayer *grid = NULL);
	size_t Earn(size_t num);
	void DrawText();
	void DrawSquare(float x, float y, ALLEGRO_COLOR color);
	bool DrawExplosion(TetrisElement& elem, float dx = 0, float dy = 0);
	void DrawBoundingBox(float x, float y, TetrisShape shape);
	bool DrawGrid();
	TetrisPosition GetInitialShapePosition();
	TetrisPosition GetInitialDebrisPosition();
	TetrisElement GetRandomTetrisElement(float x, float y, size_t elemtype);
	void NextShape();
	void OddShape();
	bool DropDebris();
	TetrisSquare GetRelativePositionOfSquare(TetrisShape shape, TetrisSquare point);
	void DrawRotations(TetrisShape& shape, float x, float y);
	void DrawAllRotations(float x, float y);
	void DrawShape(TetrisElement& elem, float dx = 0, float dy = 0);
	bool DrawFrame();
	void DrawGameBoard();
	void Rotate(bool clockwise);
	long HorizontalMove(TetrisElement &elem, long dx);
	void SwapMoveMethod();
	bool DoHorizontalMoveV2(TetrisElement & elem);
	bool DoHorizontalMoveV1(TetrisElement & elem);
	bool DoVerticalMove(TetrisElement & elem, float fps);
	void DeleteFromDeque(std::deque<TetrisElement>& deq, std::stack<size_t>& positions);
	bool DoVerticalDebrisMove();
	void ProcessImmediateKeyboardInput();
	float CopyPieceToGrid(TetrisElement& elem);
	bool Dump(TetrisElement& elem);
	bool Transfer(size_t rows);
	long SyncWithGrid(TetrisElement &elem);
	long SyncWithRow(TetrisElement &elem);
	long SyncWithCol(TetrisElement &elem);
	long collide(TetrisElement& elem, float dx, float dy);
	long collide_wit_debris(TetrisElement& elem, TetrisElement& debr, float dx, float dy);
	long CheckCollision(std::pair<long, long>& square);
	long CheckDebrisCollision(std::pair<long, long>& square);
	long CheckCollisionWithGamebox(std::pair<long, long> & point, long cols, long rows);
	TetrisPosition GetBoundingBoxCoordinates(TetrisElement &elem, float dx, float dy);
	TetrisPosition GetSquareCoordinates(TetrisElement &elem, TetrisPosition bbox, TetrisSquare point);
	TetrisPosition GetCoordsOfShapepoint(TetrisElement &elem, TetrisSquare &point);
	bool IsVerticalyAligned(TetrisElement &elem);
	void Restart();
	std::string ToString();
	bool ProcessMovement(AllegroResources &gb, FpsCounter &fps_vert, bool do_horz, bool do_vert);
	long TestCollisionCurent();
	long TestCollision(TetrisElement& elem);
	long MoveDown(TetrisElement &elem);
	bool RemoveSquareWhenGameIsOver();
	
};

