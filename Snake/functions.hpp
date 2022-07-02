#pragma once

#include "struct.hpp"

#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <string>

#define DEBUG

namespace functions {
	PlayerData GlobalPlayerData = PlayerData();
	ObjectData GlobalObjectData = ObjectData();
	TerrainData GlobalTerrainData = TerrainData();
	ScreenData GlobalScreenData = ScreenData();
	DebugData GlobalDebugData = DebugData();

	namespace misc {
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
		bool GeneratedStartPos = false;
		int GenerateRandomNumber(int min, int max, std::vector<int>exclude = {}) {
			int nr = 0;
			nr = min + rand() / (RAND_MAX / (max - min));

			for (auto i : exclude) {
				if (i == nr)
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
			COORD pixelLocation;
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
		std::string PixelOrientationToAnother(std::pair<int, int> pixelOri, std::pair<int, int> pixel) {
			if (pixelOri.first == pixel.first - 1)
				return "right";
			if (pixelOri.first - 1 == pixel.first)
				return "left";
			if (pixelOri.second == pixel.second - 1)
				return "up";
			if (pixelOri.second - 1 == pixel.second)
				return "down";

			return "An error has occured, check the pixels pos!";
		}
		std::string PixelOrientation(std::pair<int, int> pixel) {
			if (ReadMatricePixel(std::make_pair(pixel.first + 1, pixel.second)) == '+')
				return "right";
			if (ReadMatricePixel(std::make_pair(pixel.first - 1, pixel.second)) == '+')
				return "left";
			if (ReadMatricePixel(std::make_pair(pixel.first, pixel.second - 1)) == '+')
				return "up";
			if (ReadMatricePixel(std::make_pair(pixel.first, pixel.second + 1)) == '+')
				return "down";

			return "An error has occured, check pixel pos values !";
		}
		void SetData(int w, int h, char chr) {
			auto sz = std::make_pair(w, h);
			GlobalTerrainData.size.swap(sz);
			GlobalTerrainData.chr = chr;
		}
		void GenerateMatrice() {
			if (!misc::GeneratedStartPos)
				return;

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
			COORD pixelLocation;
			HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

			if (hStdOut == INVALID_HANDLE_VALUE) {
				printf("Invalid handle");
			}

			pixelLocation.X = pixel.second.first - 1;
			pixelLocation.Y = pixel.second.second - 1;

			WriteConsoleOutputCharacterW(hStdOut, pixel.first, 1, pixelLocation, &dw);
		}
		void UpdateMatriceString(std::pair<std::string, std::pair<int, int>> pixel) {
			std::wstring pixelW = std::wstring(pixel.first.begin(), pixel.first.end());
			LPCWSTR pixelChr = pixelW.c_str();

			UpdateMatrice(std::make_pair(pixelChr, std::make_pair(pixel.second.first, pixel.second.second)));
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
		void MoveLeft() {
			auto NewPos = std::make_pair(GlobalPlayerData.pos.first - 1, GlobalPlayerData.pos.second);
			auto NewPos2 = std::make_pair((GlobalPlayerData.pos.first + GlobalPlayerData.size) - 2, GlobalPlayerData.pos2.second);
			auto Orientation = terrain::PixelOrientation(GlobalPlayerData.pos2);

			if (Orientation == "right")
				NewPos2 = std::make_pair(GlobalPlayerData.pos2.first + 1, GlobalPlayerData.pos2.second);
			else if (Orientation == "left")
				NewPos2 = std::make_pair(GlobalPlayerData.pos2.first - 1, GlobalPlayerData.pos2.second);
			else if (Orientation == "up")
				NewPos2 = std::make_pair(GlobalPlayerData.pos2.first, GlobalPlayerData.pos2.second - 1);
			else if (Orientation == "down")
				NewPos2 = std::make_pair(GlobalPlayerData.pos2.first, GlobalPlayerData.pos2.second + 1);

			if (NewPos.first <= 0) {
				NewPos = std::make_pair(GlobalScreenData.size.first, GlobalPlayerData.pos.second);

				terrain::UpdateMatrice(std::make_pair(L"+", GlobalPlayerData.pos));
				terrain::UpdateMatrice(std::make_pair(L"-", GlobalPlayerData.pos2));
				terrain::UpdateMatrice(std::make_pair(L"<", std::make_pair(GlobalScreenData.size.first, GlobalPlayerData.pos.second)));
			}
			else if (NewPos.first <= GlobalScreenData.size.first) {
				terrain::UpdateMatrice(std::make_pair(L"+", GlobalPlayerData.pos));
				terrain::UpdateMatrice(std::make_pair(L"-", GlobalPlayerData.pos2));
				terrain::UpdateMatrice(std::make_pair(L"<", std::make_pair(GlobalPlayerData.pos.first - 1, GlobalPlayerData.pos.second)));
			}

			GlobalPlayerData.pos.swap(NewPos);
			GlobalPlayerData.pos2.swap(NewPos2);
		}
		void MoveRight() {
			auto NewPos = std::make_pair(GlobalPlayerData.pos.first + 1, GlobalPlayerData.pos.second);
			auto NewPos2 = std::make_pair((GlobalPlayerData.pos.first - GlobalPlayerData.size) + 2, GlobalPlayerData.pos2.second);
			auto Orientation = terrain::PixelOrientation(GlobalPlayerData.pos2);

			if (Orientation == "right")
				NewPos2 = std::make_pair(GlobalPlayerData.pos2.first + 1, GlobalPlayerData.pos2.second);
			else if (Orientation == "left")
				NewPos2 = std::make_pair(GlobalPlayerData.pos2.first - 1, GlobalPlayerData.pos2.second);
			else if (Orientation == "up")
				NewPos2 = std::make_pair(GlobalPlayerData.pos2.first, GlobalPlayerData.pos2.second - 1);
			else if (Orientation == "down")
				NewPos2 = std::make_pair(GlobalPlayerData.pos2.first, GlobalPlayerData.pos2.second + 1);

			if (NewPos.first > GlobalScreenData.size.first) {
				NewPos = std::make_pair(1, GlobalPlayerData.pos.second);

				terrain::UpdateMatrice(std::make_pair(L"+", GlobalPlayerData.pos));
				terrain::UpdateMatrice(std::make_pair(L"-", GlobalPlayerData.pos2));
				terrain::UpdateMatrice(std::make_pair(L">", std::make_pair(1, GlobalPlayerData.pos.second)));
			}
			else if (NewPos.first <= GlobalScreenData.size.first) {
				terrain::UpdateMatrice(std::make_pair(L"+", GlobalPlayerData.pos));
				terrain::UpdateMatrice(std::make_pair(L"-", GlobalPlayerData.pos2));
				terrain::UpdateMatrice(std::make_pair(L">", std::make_pair(GlobalPlayerData.pos.first + 1, GlobalPlayerData.pos.second)));
			}
			
			GlobalPlayerData.pos.swap(NewPos);
			GlobalPlayerData.pos2.swap(NewPos2);
		}
		void MoveUp() {
			auto NewPos = std::make_pair(GlobalPlayerData.pos.first, GlobalPlayerData.pos.second - 1);
			auto NewPos2 = std::make_pair(GlobalPlayerData.pos2.first, GlobalScreenData.size.second);
			auto Orientation = terrain::PixelOrientation(GlobalPlayerData.pos2);

			if (Orientation == "right")
				NewPos2 = std::make_pair(GlobalPlayerData.pos2.first + 1, GlobalPlayerData.pos2.second);
			else if (Orientation == "left")
				NewPos2 = std::make_pair(GlobalPlayerData.pos2.first - 1, GlobalPlayerData.pos2.second);
			else if (Orientation == "up")
				NewPos2 = std::make_pair(GlobalPlayerData.pos2.first, GlobalPlayerData.pos2.second - 1);
			else if (Orientation == "down")
				NewPos2 = std::make_pair(GlobalPlayerData.pos2.first, GlobalPlayerData.pos2.second + 1);

			if (NewPos.second <= 0) {
				NewPos = std::make_pair(GlobalPlayerData.pos.first, GlobalScreenData.size.second);

				terrain::UpdateMatrice(std::make_pair(L"+", GlobalPlayerData.pos));
				terrain::UpdateMatrice(std::make_pair(L"-", GlobalPlayerData.pos2));
				terrain::UpdateMatrice(std::make_pair(L"^", std::make_pair(GlobalPlayerData.pos.first, GlobalScreenData.size.second)));
			}
			else if (NewPos.second <= GlobalScreenData.size.second) {
				terrain::UpdateMatrice(std::make_pair(L"+", GlobalPlayerData.pos));
				terrain::UpdateMatrice(std::make_pair(L"-", GlobalPlayerData.pos2));
				terrain::UpdateMatrice(std::make_pair(L"^", std::make_pair(GlobalPlayerData.pos.first, GlobalPlayerData.pos.second - 1)));
			}

			GlobalPlayerData.pos.swap(NewPos);
			GlobalPlayerData.pos2.swap(NewPos2);
		}
		void MoveDown() {
			auto NewPos = std::make_pair(GlobalPlayerData.pos.first, GlobalPlayerData.pos.second + 1);
			auto NewPos2 = std::make_pair(GlobalPlayerData.pos.first, 1);
			auto Orientation = terrain::PixelOrientation(GlobalPlayerData.pos2);

			if (Orientation == "right")
				NewPos2 = std::make_pair(GlobalPlayerData.pos2.first + 1, GlobalPlayerData.pos2.second);
			else if (Orientation == "left")
				NewPos2 = std::make_pair(GlobalPlayerData.pos2.first - 1, GlobalPlayerData.pos2.second);
			else if (Orientation == "up")
				NewPos2 = std::make_pair(GlobalPlayerData.pos2.first, GlobalPlayerData.pos2.second - 1);
			else if (Orientation == "down")
				NewPos2 = std::make_pair(GlobalPlayerData.pos2.first, GlobalPlayerData.pos2.second + 1);

			if (NewPos.second > GlobalScreenData.size.second) {
				NewPos = std::make_pair(GlobalPlayerData.pos.first, 1);

				terrain::UpdateMatrice(std::make_pair(L"+", GlobalPlayerData.pos));
				terrain::UpdateMatrice(std::make_pair(L"-", GlobalPlayerData.pos2));
				terrain::UpdateMatrice(std::make_pair(L"^", std::make_pair(GlobalPlayerData.pos.first, 1)));
			}
			else if (NewPos.second <= GlobalScreenData.size.second) {
				terrain::UpdateMatrice(std::make_pair(L"+", GlobalPlayerData.pos));
				terrain::UpdateMatrice(std::make_pair(L"-", GlobalPlayerData.pos2));
				terrain::UpdateMatrice(std::make_pair(L"^", std::make_pair(GlobalPlayerData.pos.first, GlobalPlayerData.pos.second + 1)));
			}
			 
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
			if (misc::GeneratedStartPos == true)
				return;
			int x, y;
			x = misc::GenerateRandomNumber(1, GlobalScreenData.size.first - 10) + GlobalPlayerData.size;
			y = misc::GenerateRandomNumber(1, GlobalScreenData.size.second);

			while (x == y) { x = misc::GenerateRandomNumber(0, 90); }
			
			//std::cout << x << " " << y << '\n'; //DEGUB
			
			auto pos0 = std::make_pair(x, y);
			auto pos1 = std::make_pair(x, y);
			auto pos2 = std::make_pair(x - GlobalPlayerData.size + 1, y);

			GlobalPlayerData.StartPos.swap(pos0);
			GlobalPlayerData.pos.swap(pos1);
			GlobalPlayerData.pos2.swap(pos2);

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
	namespace debug {
		void Display() {
			std::string posXY = "Pos: X: " + GlobalPlayerData.pos.first;
			posXY += " Y: " + GlobalPlayerData.pos.second;
			std::string pos2XY = "Pos2: X: " + GlobalPlayerData.pos2.first;
			pos2XY += " Y: " + GlobalPlayerData.pos2.second;

			terrain::UpdateMatriceString(std::make_pair("LMAO", std::make_pair(100, 10)));
		}
	}
}