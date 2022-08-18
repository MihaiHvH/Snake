#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <Windows.h>

#include "struct.hpp"
#include "functions.hpp"

int main() {
	srand((unsigned)time(NULL));

	functions::misc::ClearGameData();
	functions::misc::SetGameConsoleTitle("Snake by MihaiHvH");
	functions::misc::SetWindowSize(std::make_pair(120, 30));
	functions::misc::GetWindowSize();
	functions::terrain::SetData(functions::GlobalScreenData.size.first, functions::GlobalScreenData.size.second, functions::GlobalTerrainData.chr);
	functions::player::GenerateStartPos();
	functions::terrain::GenerateMatrice();
	functions::player::Spawn(functions::GlobalPlayerData.StartPos);

	int A = 0;
	int D = 0;
	int W = 0;
	int S = 0;
	int SPACE = 0;

	functions::object::Spawn();
	
	while (true) {
		if (GetKeyState(VK_ESCAPE) & 0x8000)
			break;

		if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && (SPACE == 0)) {
			++SPACE;
			functions::terrain::ClearMatrice();
			main();
		}
		else if (GetAsyncKeyState(0x44) == 0) SPACE = 0;

		if ((GetAsyncKeyState(0x41) & 0x8000) && (A == 0)) {
			A++;
			if (functions::terrain::ReadMatricePixel(std::make_pair(functions::GlobalPlayerData.pos.first - 1, functions::GlobalPlayerData.pos.second)) == '-') {
				functions::player::MoveLeft();
			}
			else if (functions::terrain::ReadMatricePixel(std::make_pair(functions::GlobalPlayerData.pos.first - 1, functions::GlobalPlayerData.pos.second)) == 'o') {
				functions::player::Grow();
				functions::GlobalObjectData.spawned = false;
				functions::player::MoveLeft();
				functions::object::Spawn();
			}
			else
				functions::misc::GameOver();
		}
		else if (GetAsyncKeyState(0x41) == 0) A = 0;
		if ((GetAsyncKeyState(0x44) & 0x8000) && (D == 0)) {
			D++;
			if (functions::terrain::ReadMatricePixel(std::make_pair(functions::GlobalPlayerData.pos.first + 1, functions::GlobalPlayerData.pos.second)) == '-') {
				functions::player::MoveRight();
			}
			else if (functions::terrain::ReadMatricePixel(std::make_pair(functions::GlobalPlayerData.pos.first + 1, functions::GlobalPlayerData.pos.second)) == 'o') {
				functions::player::Grow();
				functions::GlobalObjectData.spawned = false;
				functions::player::MoveRight();
				functions::object::Spawn();
			}
			else
				functions::misc::GameOver();
		}
		else if (GetAsyncKeyState(0x44) == 0) D = 0;
		if ((GetAsyncKeyState(0x57) & 0x8000) && (W == 0 )) {
			W++;
			if (functions::terrain::ReadMatricePixel(std::make_pair(functions::GlobalPlayerData.pos.first, functions::GlobalPlayerData.pos.second - 1)) == '-') {
				functions::player::MoveUp();
			}
			else if (functions::terrain::ReadMatricePixel(std::make_pair(functions::GlobalPlayerData.pos.first, functions::GlobalPlayerData.pos.second - 1)) == 'o') {
				functions::player::Grow();
				functions::GlobalObjectData.spawned = false;
				functions::player::MoveUp();
				functions::object::Spawn();
			}
			else
				functions::misc::GameOver();
		}
		else if (GetAsyncKeyState(0x57) == 0) W = 0;
		if ((GetAsyncKeyState(0x53) & 0x8000) && (S == 0)) {
			S++;
			if (functions::terrain::ReadMatricePixel(std::make_pair(functions::GlobalPlayerData.pos.first, functions::GlobalPlayerData.pos.second + 1)) == '-') {
				functions::player::MoveDown();
			}
			else if (functions::terrain::ReadMatricePixel(std::make_pair(functions::GlobalPlayerData.pos.first, functions::GlobalPlayerData.pos.second + 1)) == 'o') {
				functions::player::Grow();
				functions::GlobalObjectData.spawned = false;
				functions::player::MoveDown();
				functions::object::Spawn();
			}
			else
				functions::misc::GameOver();
		}
		else if (GetAsyncKeyState(0x53) == 0) S = 0;
	}
	return 0;
}

//Self made by MihaiHvH // last updated - 11:44 GMT+2