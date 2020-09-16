#include "stdafx.h"
#include "GameMonitor.h"
#include "Scope.h"

GameMonitor::GameMonitor()
{
	HDC monitor;
	Scope scope(
		[&]() { return ((monitor = GetDC(NULL)) != NULL); },
		[&]() { ReleaseDC(NULL, monitor); },
		"Failed to resolve the resolution of the monitor.");
	horz_res = GetDeviceCaps(monitor, HORZRES);
	vert_res = GetDeviceCaps(monitor, VERTRES);
	unity = floor(vert_res / 52);
	unitx = unity;
}


GameMonitor::~GameMonitor()
{
}
