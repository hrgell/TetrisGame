#pragma once

class TetrisKey
{
private:
	// Is the key down ?
	volatile long down;
	// Has the key been down ?
	volatile long pressed;
public:
	long int id;
	// Keep track of order in which keys are pressed.
	unsigned long timestamp;
	TetrisKey();
	~TetrisKey();
	void KeyDown();
	void KeyUp();
	void Clear();
	void UnPress();
	void Press();
	bool IsPressed();
	bool IsDown();
	bool PressIfDown();
};
