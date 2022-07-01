#pragma once

#include "struct.hpp"

#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <string>

namespace functions {
	PlayerData GlobalPlayerData = PlayerData();
	ObjectData GlobalObjectData = ObjectData();
	TerrainData GlobalTerrainData = TerrainData();
	ScreenData GlobalScreenData = ScreenData();

	namespace misc {
		void GetWindowSize() {
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			int w, h;

			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
			w = csbi.srWindow.Right - csbi.srWindow.Left + 1;
			h = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
			//std::cout << "W: " << w << " H: " << h << std::endl; //DEBUG
			auto size = std::make_pair(w, h);
			GlobalScreenData.size.swap(size);
		}
		bool GeneratedStartPos = false;
		int GenerateRandomNumber(int min, int max) {
			int nr = 0;
			nr = min + rand() / (RAND_MAX / (max - min));
			
			return nr;
		};
	}
	namespace terrain {
		void SetData(int w, int h, char chr) {
			auto sz = std::make_pair(w, h);
			GlobalTerrainData.size.swap(sz);
			GlobalTerrainData.chr = chr;
		}
		void GenerateMatrice() {
			if (!misc::GeneratedStartPos)
				return;

			for (int i = 1; i <= GlobalTerrainData.size.second; ++i) {
				if (i > 1) std::cout << '\n';
				for (int j = 1; j <= GlobalTerrainData.size.first; ++j) {
					std::cout << GlobalTerrainData.chr;
				}
			}
		}
		void UpdateMatrice(std::pair<LPCWSTR, std::pair<int, int>> pixel) {
			DWORD dw;
			COORD pixelLocation;
			HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
			if (hStdOut == INVALID_HANDLE_VALUE)
			{
				printf("Invalid handle");
			}
			pixelLocation.X = pixel.second.first - 1;
			pixelLocation.Y = pixel.second.second - 1;

			WriteConsoleOutputCharacterW(hStdOut, pixel.first, 1, pixelLocation, &dw);
		}
	}
	namespace player {
		void MoveLeft() {
			terrain::UpdateMatrice(std::make_pair(L"+", std::make_pair(GlobalPlayerData.pos.first - 1, GlobalPlayerData.pos.second)));
			terrain::UpdateMatrice(std::make_pair(L"-", std::make_pair(GlobalPlayerData.pos.first + (GlobalPlayerData.size - 1), GlobalPlayerData.pos.second)));
			
			auto NewPos = std::make_pair(GlobalPlayerData.pos.first - 1, GlobalPlayerData.pos.second);
			GlobalPlayerData.pos.swap(NewPos);
		}
		void MoveRight() {
			terrain::UpdateMatrice(std::make_pair(L"+", std::make_pair(GlobalPlayerData.pos.first + GlobalPlayerData.size, GlobalPlayerData.pos.second)));
			terrain::UpdateMatrice(std::make_pair(L"-", std::make_pair(GlobalPlayerData.pos.first, GlobalPlayerData.pos.second)));

			auto NewPos = std::make_pair(GlobalPlayerData.pos.first + 1, GlobalPlayerData.pos.second);
			GlobalPlayerData.pos.swap(NewPos);
		}
		void MoveUp() {

		}
		void MoveDown() {
			terrain::UpdateMatrice(std::make_pair(L"+", std::make_pair(GlobalPlayerData.pos.first + (GlobalPlayerData.size - 1), GlobalPlayerData.pos.second - 1)));
			terrain::UpdateMatrice(std::make_pair(L"-", std::make_pair(GlobalPlayerData.pos.first, GlobalPlayerData.pos.second)));

			auto NewPos = std::make_pair(GlobalPlayerData.pos.first, GlobalPlayerData.pos.second - 1);
			GlobalPlayerData.pos.swap(NewPos);
		}
		void Spawn(std::pair<int,int>pos) {
			for (int i = 0; i <= GlobalPlayerData.size - 1; ++i) {
				terrain::UpdateMatrice(std::make_pair(L"<", std::make_pair(pos.first, pos.second)));
				terrain::UpdateMatrice(std::make_pair(L"+", std::make_pair(pos.first + i, pos.second)));
			}
		}
		void GenerateStartPos() {
			if (misc::GeneratedStartPos == true)
				return;
			int x, y;
			x = misc::GenerateRandomNumber(1, GlobalScreenData.size.first - 10);
			y = misc::GenerateRandomNumber(1, GlobalScreenData.size.second);

			while (x == y) { x = misc::GenerateRandomNumber(0, 90); }
			//std::cout << x << " " << y << '\n'; //DEGUB
			auto pos1 = std::make_pair(x, y);
			auto pos2 = std::make_pair(x, y);
			GlobalPlayerData.StartPos.swap(pos1);
			GlobalPlayerData.pos.swap(pos2);
			misc::GeneratedStartPos = true;
		};
		std::pair<int, int> GetPos() {
			return GlobalPlayerData.pos;
		};
	}
	namespace object {
		void GeneratePos() {
		
		};
		void GetPos() {
		
		};
	}
}