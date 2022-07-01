#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <Windows.h>

#include "struct.hpp"
#include "functions.hpp"

void main(void) {
	srand((unsigned)time(NULL));
	
	functions::misc::GetWindowSize(); // get stdout window size
	functions::terrain::SetData(functions::GlobalScreenData.size.first, functions::GlobalScreenData.size.second, '-'); // set Terrain data
	functions::player::GenerateStartPos(); // gen snake spawn pos
	functions::terrain::GenerateMatrice(); // gen screen
	functions::player::Spawn(functions::GlobalPlayerData.StartPos);

	int A = 0;
	int D = 0;
	int W = 0;
	int S = 0;

	while (true) {
		if (GetKeyState(VK_ESCAPE) & 0x8000) // when esc is pressed, the game closes
			break;						
		
		if ((GetAsyncKeyState(0x41) & 0x8000) && (A == 0)) { // When A is pressed
			A = 1;
			functions::player::MoveLeft();
		}
		else if (GetAsyncKeyState(0x41) == 0) A = 0;
		if ((GetAsyncKeyState(0x44) & 0x8000) && (D == 0)) { // When D is pressed
			D = 1;
			functions::player::MoveRight();
		}
		else if (GetAsyncKeyState(0x44) == 0) D = 0;
		if ((GetAsyncKeyState(0x57) & 0x8000) && (W == 0)) { // When W is pressed
			W = 1;
			functions::player::MoveUp();
		}
		else if (GetAsyncKeyState(0x57) == 0) W = 0;
		if ((GetAsyncKeyState(0x53) & 0x8000) && (S == 0)) { // When S is pressed
			S = 1;
			functions::player::MoveDown();
		}
		else if (GetAsyncKeyState(0x53) == 0) S = 0;
	}
}