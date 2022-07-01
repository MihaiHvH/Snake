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

	- coord ultimul pixel in struct PlayerData
	- move up, down, (*left*)
	- obiecte
	- cand se spawneaza obiectele sa nu se intersecteze cu pixelii
	- (poate) la generarea unui nr random sa se poata excluda unele (pentru obiecte) (vector)

*/
int main() {
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
		
		if ((GetAsyncKeyState(0x41) & 0x8000) && (A <= (functions::GlobalPlayerData.speed / 100) - 1)) { // When A is pressed
			A++;
			functions::player::MoveLeft();
			functions::terrain::UpdateMatrice(std::make_pair(L"A", functions::GlobalPlayerData.pos2));
		}
		else if (GetAsyncKeyState(0x41) == 0) A = 0;
		if ((GetAsyncKeyState(0x44) & 0x8000) && (D <= (functions::GlobalPlayerData.speed / 100) - 1)) { // When D is pressed
			D++;
			functions::player::MoveRight();
			functions::terrain::UpdateMatrice(std::make_pair(L"A", functions::GlobalPlayerData.pos2));
		}
		else if (GetAsyncKeyState(0x44) == 0) D = 0;
		if ((GetAsyncKeyState(0x57) & 0x8000) && (W <= (functions::GlobalPlayerData.speed / 100) - 1)) { // When W is pressed
			W++;
			functions::player::MoveUp();
			functions::terrain::UpdateMatrice(std::make_pair(L"A", functions::GlobalPlayerData.pos2));
		}
		else if (GetAsyncKeyState(0x57) == 0) W = 0;
		if ((GetAsyncKeyState(0x53) & 0x8000) && (S <= (functions::GlobalPlayerData.speed / 100) - 1)) { // When S is pressed
			S++;
			functions::player::MoveDown();
			functions::terrain::UpdateMatrice(std::make_pair(L"A", functions::GlobalPlayerData.pos2));
		}
		else if (GetAsyncKeyState(0x53) == 0) S = 0;
	}
	return 0;
}