#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <Windows.h>

#include "struct.hpp"
#include "functions.hpp"

/*
De agaugat:
	- obiecte
	- cand se spawneaza obiectele sa nu se intersecteze cu pixelii
	- namespace functions
*/

int main() {
	srand((unsigned)time(NULL));

	functions::misc::SetGameConsoleTitle("Snake");
	functions::misc::SetWindowSize(std::make_pair(120, 30));
	functions::misc::GetWindowSize();
	functions::terrain::SetData(functions::GlobalScreenData.size.first, functions::GlobalScreenData.size.second, functions::GlobalTerrainData.chr);
	functions::player::GenerateStartPos();
	functions::terrain::GenerateMatrice();

#ifdef DEBUG
	functions::terrain::GenerateEmptySpace(std::make_pair(30, 0));
#endif // DEBUG

	functions::player::Spawn(functions::GlobalPlayerData.StartPos);

	int A = 0;
	int D = 0;
	int W = 0;
	int S = 0;
	functions::debug::Display();
#ifdef DEBUG
	int SPACE = 0;
#endif // DEBUG


	while (true) {
		if (GetKeyState(VK_ESCAPE) & 0x8000) // when esc is pressed, the game closes
			break;						

#ifdef DEBUG
		if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && SPACE == 0) {
			SPACE++;
			functions::debug::Display();
		}
		else if (GetAsyncKeyState(VK_SPACE) == 0) SPACE = 0;
#endif // DEBUG


		if ((GetAsyncKeyState(0x41) & 0x8000) && (A <= (functions::GlobalPlayerData.speed / 100) - 1)) { // When A is pressed
			A++;
			if (functions::terrain::ReadMatricePixel(std::make_pair(functions::GlobalPlayerData.pos.first - 1, functions::GlobalPlayerData.pos.second)) == '-') {
				functions::player::MoveLeft();
			}
		}
		else if (GetAsyncKeyState(0x41) == 0) A = 0;
		if ((GetAsyncKeyState(0x44) & 0x8000) && (D <= (functions::GlobalPlayerData.speed / 100) - 1)) { // When D is pressed
			D++;
			if (functions::terrain::ReadMatricePixel(std::make_pair(functions::GlobalPlayerData.pos.first + 1, functions::GlobalPlayerData.pos.second)) == '-') {
				functions::player::MoveRight();
			}
		}
		else if (GetAsyncKeyState(0x44) == 0) D = 0;
		if ((GetAsyncKeyState(0x57) & 0x8000) && (W <= (functions::GlobalPlayerData.speed / 100) - 1)) { // When W is pressed
			W++;
			if (functions::terrain::ReadMatricePixel(std::make_pair(functions::GlobalPlayerData.pos.first, functions::GlobalPlayerData.pos.second - 1)) == '-') {
				functions::player::MoveUp();
			}
		}
		else if (GetAsyncKeyState(0x57) == 0) W = 0;
		if ((GetAsyncKeyState(0x53) & 0x8000) && (S <= (functions::GlobalPlayerData.speed / 100) - 1)) { // When S is pressed
			S++;
			if (functions::terrain::ReadMatricePixel(std::make_pair(functions::GlobalPlayerData.pos.first, functions::GlobalPlayerData.pos.second + 1)) == '-') {
				functions::player::MoveDown();
			}
		}
		else if (GetAsyncKeyState(0x53) == 0) S = 0;
	}
	return 0;
}