#include "stdafx.h"
#include "TetrisKey.h"
TetrisKey::TetrisKey() : down(0), pressed(0), timestamp(0) { }
TetrisKey::~TetrisKey() { }
bool TetrisKey::IsPressed() { return (pressed != 0); }
bool TetrisKey::IsDown() { return (down != 0); }
void TetrisKey::KeyDown() {	down = 1; pressed = 1; }
void TetrisKey::KeyUp() { down = 0; }
void TetrisKey::Clear() { down = 0; pressed = 0; }
void TetrisKey::UnPress() { pressed = 0; }
void TetrisKey::Press() { pressed = 1; }
bool TetrisKey::PressIfDown() {	if (!IsDown()) return false; Press(); return true; }
