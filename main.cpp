#include "main.h"

void AutoMove(int dir = 0) {
	while (1) {
		if (dir != 0)
			std::cout << "ASMOKUS";
	}
}

int main() {
#ifdef _DEBUG
	std::cout << "-------------------DEBUG-------------------" << '\n';
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
#else
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
#endif // _DEBUG

	pGraphics Graphics;

	Graphics.rGame.GlobalScreenData.windowName = "Snake by MihaiHvH";

	Graphics.Initialize();
	
	Graphics.rGame.GlobalScreenData.size = Graphics.getScreenSize();
	Graphics.rGame.GenerateObjectMap();
	Graphics.rGame.GenerateStartPos();
	Graphics.rGame.Spawn();
	Graphics.rGame.canRender = true;

#ifdef _DEBUG
	std::cout << "Graphics initialized successfully" << '\n';
	std::cout << "Screen size X: " << Graphics.rGame.GlobalScreenData.size.first << " Y: " << Graphics.rGame.GlobalScreenData.size.second << '\n';
#endif // DEBUG

	std::thread Render(&pGraphics::OnRender, &Graphics);
	Render.join();
	
	//Graphics.RunMessageLoop();

	std::thread MessageLoop(&pGraphics::RunMessageLoop, &Graphics);
	//MessageLoop.join();

	return 0;
}