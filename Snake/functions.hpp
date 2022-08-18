#pragma once

#include "struct.hpp"

#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <string>

//#define DEBUG

namespace functions {
	PlayerData GlobalPlayerData = PlayerData();
	ObjectData GlobalObjectData = ObjectData();
	TerrainData GlobalTerrainData = TerrainData();
	ScreenData GlobalScreenData = ScreenData();
	DebugData GlobalDebugData = DebugData();

	namespace misc {
		void GameOver() {
			system("CLS");
			srand((unsigned)time(NULL));
			std::cout << R"(
			 _____    ___    __  __   _____     _____   _   _   _____   _____     _  
			|  __ \  / _ \  |  \/  | |  ___|   |  _  | | | | | |  ___| | ___ \   | | 
			| |  \/ / /_\ \ | .  . | | |__     | | | | | | | | | |__   | |_/ /   | | 
			| | __  |  _  | | |\/| | |  __|    | | | | | | | | |  __|  |    /    | | 
			| |_\ \ | | | | | |  | | | |___    \ \_/ / \ \_/ / | |___  | |\ \    |_| 
			 \____/ \_| |_/ \_|  |_/ \____/     \___/   \___/  \____/  \_| \_|   (_)
			 Press 'ESC' to exit the game or 'SPACE' to restart, thanks for playing !)" << 
						 R"(
							Total size: )" << GlobalPlayerData.size << '\n';
		}
		void ClearGameData() {
			GlobalPlayerData.backupPixels.clear();
			GlobalPlayerData.pixels.clear();
			GlobalPlayerData.size = 5;
			GlobalObjectData.spawned = false;
		}
		void SetGameConsoleTitle(std::string name) {
			::SetConsoleTitleA(name.c_str());
			GlobalScreenData.consoleName = name;
		}
		void SetWindowSize(std::pair<int, int>size) {
			COORD sizeW{};
			HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

			if (hStdOut == INVALID_HANDLE_VALUE) {
				printf("Invalid handle");
			}

			sizeW.X = size.first;
			sizeW.Y = size.second;

			SetConsoleDisplayMode(hStdOut, 1, &sizeW);
			
			sizeW.X = size.first - 1;
			sizeW.Y = size.second - 1;

			SetConsoleScreenBufferSize(hStdOut, sizeW);
		}
		void GetWindowSize() {
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			int w, h;
			HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

			if (hStdOut == INVALID_HANDLE_VALUE) {
				printf("Invalid handle");
			}

			GetConsoleScreenBufferInfo(hStdOut, &csbi);

			w = csbi.srWindow.Right - csbi.srWindow.Left + 1;
			h = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

			auto size = std::make_pair(w, h);
			GlobalScreenData.size.swap(size);
		}
		std::pair<int, int> GetWindowSizePair() {
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			int w, h;
			HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

			if (hStdOut == INVALID_HANDLE_VALUE) {
				printf("Invalid handle");
			}

			GetConsoleScreenBufferInfo(hStdOut, &csbi);

			w = csbi.srWindow.Right - csbi.srWindow.Left + 1;
			h = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

			auto size = std::make_pair(w, h);

			return size;
		}
		int GenerateRandomNumber(int min, int max, std::vector<std::pair<int, int>>exclude = {}) {
			int nr = 0;
			nr = min + rand() / (RAND_MAX / (max - min));

			for (auto& i : exclude) {
				if (i.first == nr or i.second == nr)
					return GenerateRandomNumber(min, max, exclude);
			}

			return nr;
		};
	}
	namespace terrain {
		void ClearMatrice() {
			system("CLS");
		}
		char ReadMatricePixel(std::pair<int, int> pixel) {
			DWORD dw;
			COORD pixelLocation{};
			HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

			if (hStdOut == INVALID_HANDLE_VALUE) {
				printf("Invalid handle");
			}

			pixelLocation.X = pixel.first - 1;
			pixelLocation.Y = pixel.second - 1;

			char str = '-';

			ReadConsoleOutputCharacterA(hStdOut, &str, 1, pixelLocation, &dw);
			return str;
		}
		void SetData(int w, int h, char chr) {
			auto sz = std::make_pair(w, h);
			GlobalTerrainData.size.swap(sz);
			GlobalTerrainData.chr = chr;
		}
		void GenerateMatrice() {
			for (int i = 1; i <= GlobalTerrainData.size.second; ++i) {
#ifdef DEBUG
				if (i > 1) std::cout << '\n';
#endif // DEBUG
				for (int j = 1; j <= GlobalTerrainData.size.first; ++j) {
					std::cout << GlobalTerrainData.chr;
				}
			}
		}
		void UpdateMatrice(std::pair<LPCWSTR, std::pair<int, int>> pixel) {
			DWORD dw;
			COORD pixelLocation{};
			HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

			if (hStdOut == INVALID_HANDLE_VALUE) {
				printf("Invalid handle");
			}

			pixelLocation.X = pixel.second.first - 1;
			pixelLocation.Y = pixel.second.second - 1;

			WriteConsoleOutputCharacterW(hStdOut, pixel.first, 1, pixelLocation, &dw);
		}
		void GenerateEmptySpace(std::pair<int, int> size) {
			auto OldSize = misc::GetWindowSizePair();
			
			ClearMatrice();
			functions::terrain::SetData(GlobalScreenData.size.first - size.first, GlobalScreenData.size.second - size.second, GlobalTerrainData.chr);
			misc::SetWindowSize(std::make_pair(OldSize.first + size.first, OldSize.second + size.second));
			GenerateMatrice();

			auto sz = std::make_pair(GlobalScreenData.size.first - size.first, GlobalScreenData.size.second - size.second);

			GlobalDebugData.size.swap(sz);
		}
	}
	namespace player {
		void UpdatePixels(std::pair<int, int> NewPos) {
			for (int i = 0; i <= GlobalPlayerData.size - 1; ++i) {
				GlobalPlayerData.backupPixels.at(i) = GlobalPlayerData.pixels.at(i);
			}

			for (int i = 0; i <= GlobalPlayerData.size - 1; ++i) {
				if (i == 0)
					GlobalPlayerData.pixels.at(i) = NewPos;
				else
					GlobalPlayerData.pixels.at(i) = GlobalPlayerData.backupPixels.at(static_cast<std::vector<std::pair<int, int>, std::allocator<std::pair<int, int>>>::size_type>(i) - 1);
			}
		}
		void Grow() {
			GlobalPlayerData.size += 1;
			if (terrain::ReadMatricePixel(std::make_pair(GlobalPlayerData.pos2.first + 1, GlobalPlayerData.pos2.second)) == '-') {
				std::pair<int, int> NewPos2 = std::make_pair(GlobalPlayerData.pos2.first + 1, GlobalPlayerData.pos2.second);
				GlobalPlayerData.pos2.swap(NewPos2);
				GlobalPlayerData.pixels.push_back(GlobalPlayerData.pos2);
				GlobalPlayerData.backupPixels.push_back(GlobalPlayerData.pos2);
			}
			else if (terrain::ReadMatricePixel(std::make_pair(GlobalPlayerData.pos2.first - 1, GlobalPlayerData.pos2.second)) == '-') {
				std::pair<int, int> NewPos2 = std::make_pair(GlobalPlayerData.pos2.first - 1, GlobalPlayerData.pos2.second);
				GlobalPlayerData.pos2.swap(NewPos2);
				GlobalPlayerData.pixels.push_back(GlobalPlayerData.pos2);
				GlobalPlayerData.backupPixels.push_back(GlobalPlayerData.pos2);
			}
			else if (terrain::ReadMatricePixel(std::make_pair(GlobalPlayerData.pos2.first, GlobalPlayerData.pos2.second + 1)) == '-') {
				std::pair<int, int> NewPos2 = std::make_pair(GlobalPlayerData.pos2.first, GlobalPlayerData.pos2.second + 1);
				GlobalPlayerData.pos2.swap(NewPos2);
				GlobalPlayerData.pixels.push_back(GlobalPlayerData.pos2);
				GlobalPlayerData.backupPixels.push_back(GlobalPlayerData.pos2);
			}
			else if (terrain::ReadMatricePixel(std::make_pair(GlobalPlayerData.pos2.first, GlobalPlayerData.pos2.second - 1)) == '-') {
				std::pair<int, int> NewPos2 = std::make_pair(GlobalPlayerData.pos2.first, GlobalPlayerData.pos2.second - 1);
				GlobalPlayerData.pos2.swap(NewPos2);
				GlobalPlayerData.pixels.push_back(GlobalPlayerData.pos2);
				GlobalPlayerData.backupPixels.push_back(GlobalPlayerData.pos2);
			}
		}
		void MoveLeft() {
			std::pair<int, int> NewPos = std::make_pair(GlobalPlayerData.pos.first - 1, GlobalPlayerData.pos.second);
			std::pair<int, int> NewPos2 = GlobalPlayerData.pixels.at(static_cast<std::vector<std::pair<int, int>, std::allocator<std::pair<int, int>>>::size_type>(GlobalPlayerData.size) - 2);

			UpdatePixels(NewPos);

			terrain::UpdateMatrice(std::make_pair(L"+", GlobalPlayerData.pos));
			terrain::UpdateMatrice(std::make_pair(L"-", GlobalPlayerData.pos2));
			terrain::UpdateMatrice(std::make_pair(L"<", NewPos));

			GlobalPlayerData.pos.swap(NewPos);
			GlobalPlayerData.pos2.swap(NewPos2);
		}
		void MoveRight() {
			std::pair<int, int> NewPos = std::make_pair(GlobalPlayerData.pos.first + 1, GlobalPlayerData.pos.second);
			std::pair<int, int> NewPos2 = GlobalPlayerData.pixels.at(static_cast<std::vector<std::pair<int, int>, std::allocator<std::pair<int, int>>>::size_type>(GlobalPlayerData.size) - 2);

			UpdatePixels(NewPos);

			terrain::UpdateMatrice(std::make_pair(L"+", GlobalPlayerData.pos));
			terrain::UpdateMatrice(std::make_pair(L"-", GlobalPlayerData.pos2));
			terrain::UpdateMatrice(std::make_pair(L">", NewPos));
			
			GlobalPlayerData.pos.swap(NewPos);
			GlobalPlayerData.pos2.swap(NewPos2);
		}
		void MoveUp() {
			std::pair<int, int> NewPos = std::make_pair(GlobalPlayerData.pos.first, GlobalPlayerData.pos.second - 1);
			std::pair<int, int> NewPos2 = GlobalPlayerData.pixels.at(static_cast<std::vector<std::pair<int, int>, std::allocator<std::pair<int, int>>>::size_type>(GlobalPlayerData.size) - 2);

			UpdatePixels(NewPos);

			terrain::UpdateMatrice(std::make_pair(L"+", GlobalPlayerData.pos));
			terrain::UpdateMatrice(std::make_pair(L"-", GlobalPlayerData.pos2));
			terrain::UpdateMatrice(std::make_pair(L"^", NewPos));
	

			GlobalPlayerData.pos.swap(NewPos);
			GlobalPlayerData.pos2.swap(NewPos2);
		}
		void MoveDown() {
			std::pair<int, int> NewPos = std::make_pair(GlobalPlayerData.pos.first, GlobalPlayerData.pos.second + 1);
			std::pair<int, int> NewPos2 = GlobalPlayerData.pixels.at(static_cast<std::vector<std::pair<int, int>, std::allocator<std::pair<int, int>>>::size_type>(GlobalPlayerData.size) - 2);

			UpdatePixels(NewPos);

			terrain::UpdateMatrice(std::make_pair(L"+", GlobalPlayerData.pos));
			terrain::UpdateMatrice(std::make_pair(L"-", GlobalPlayerData.pos2));
			terrain::UpdateMatrice(std::make_pair(L"^", NewPos));
			 
			GlobalPlayerData.pos.swap(NewPos);
			GlobalPlayerData.pos2.swap(NewPos2);
		}
		void Spawn(std::pair<int,int>pos) {
			for (int i = 1; i <= GlobalPlayerData.size - 1; ++i) {
				terrain::UpdateMatrice(std::make_pair(L"+", std::make_pair((pos.first + i) - GlobalPlayerData.size, pos.second)));
			}
			terrain::UpdateMatrice(std::make_pair(L">", std::make_pair(pos.first, pos.second)));
		}
		void GenerateStartPos() {
			int x, y;
			x = misc::GenerateRandomNumber(1, GlobalScreenData.size.first - 10) + GlobalPlayerData.size;
			y = misc::GenerateRandomNumber(1, GlobalScreenData.size.second);

			while (x == y) { x = misc::GenerateRandomNumber(0, 90); }
			
			auto pos0 = std::make_pair(x, y);
			auto pos1 = std::make_pair(x, y);
			auto pos2 = std::make_pair(x - GlobalPlayerData.size + 1, y);

			GlobalPlayerData.StartPos.swap(pos0);
			GlobalPlayerData.pos.swap(pos1);
			GlobalPlayerData.pos2.swap(pos2);

			GlobalPlayerData.pixels.push_back(std::make_pair(x, y));
			GlobalPlayerData.backupPixels.push_back(std::make_pair(x, y));

			for (int i = 1; i <= GlobalPlayerData.size - 1; ++i) {
				GlobalPlayerData.pixels.push_back(std::make_pair(x - i, y));
				GlobalPlayerData.backupPixels.push_back(std::make_pair(x - i, y));
			}
		};
		std::pair<int, int> GetPos() {
			return GlobalPlayerData.pos;
		};
	}
	namespace object {
		void Spawn() {
			if (!GlobalObjectData.spawned) {
				std::pair<int, int> pos = { misc::GenerateRandomNumber(1, GlobalScreenData.size.first, { GlobalPlayerData.pixels }), misc::GenerateRandomNumber(1, GlobalScreenData.size.second, { GlobalPlayerData.pixels }) };
				terrain::UpdateMatrice(std::make_pair(L"o", std::make_pair(pos.first, pos.second)));
				GlobalObjectData.pos.swap(pos);
				GlobalObjectData.spawned = true;
			}
		};
	}
	namespace debug {
		void DisplayPos2(LPCWSTR chr, bool f = false) {
			if (f) {
				terrain::UpdateMatrice(std::make_pair(chr, GlobalPlayerData.pos2));
				auto x = GlobalPlayerData.pos2;
				GlobalDebugData.lastPos2.swap(x);
			}
			else if (GlobalDebugData.lastPos2 != GlobalPlayerData.pos2) {
				terrain::UpdateMatrice(std::make_pair(chr, GlobalPlayerData.pos2));
				auto x = GlobalPlayerData.pos2;
				GlobalDebugData.lastPos2.swap(x);
			}
		}
	}
}