#include "main.h"

int main() {
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);

	pGraphics Graphics;

	Graphics.rGame.GlobalScreenData.windowName = "Snake by MihaiHvH";

	Graphics.Initialize();
	
	Graphics.rGame.GlobalScreenData.size = Graphics.getScreenSize();
	Graphics.rGame.GenerateStartPos();
	Graphics.rGame.GenerateObjectMap();
	Graphics.rGame.Spawn();
	Graphics.rGame.canRender = true;

	Graphics.OnRender();
	Graphics.RunMessageLoop();

	return 0;
}