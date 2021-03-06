// Change Module Load Messages back on when not developing:
// Tools -> Options -> Debugging -> Output Window -> Module Load Messages -> On

#include "stdafx.h"
#include "TetrisGame.h"
#include "Main.h"
#include "Util.h"
#include "Scope.h"
//// format
//#include <iostream>
//#include <format>

using namespace std;

int doit() {
	AllegroResources gb(1200);
	TetrisGame tetrisgame(gb);
	tetrisgame.RunGame();
	return 0;
}

int main(int argc, char *argv[]) {
	//EnumerateResources();
	//DebugWriteMsg(std::format("Hello {}!\n", "world"));
	int status = 0;
	DebugWriteMsg("starting");
	try {
		status = doit();
	}
	catch (std::runtime_error& e)
	{
		DebugWriteMsg(e.what());
	}
	catch (...) {
		DebugWriteMsg("Error: An unknown exception occured.");
	}
	DebugWriteMsg("stopping");
	return status;
} // main()