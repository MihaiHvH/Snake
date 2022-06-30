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
	TerainData GlobalTerainData = TerainData();

	namespace misc {
		bool GeneratedStartPos = false;
		int GenerateRandomNumber(int min, int max) {
			int nr = 0;
			nr = min + rand() / (RAND_MAX / (max - min));
			
			return nr;
		};
	}
	namespace terain {
		void SetData(int w, int h, char chr) {
			auto sz = std::make_pair(w, h);
			GlobalTerainData.size.swap(sz);
			GlobalTerainData.chr = chr;
		}
		void GenerateMatrice() {
			if (!misc::GeneratedStartPos)
				return;

			for (int i = 1; i <= GlobalTerainData.size.second; ++i) {
				if (i > 1) std::cout << '\n';
				for (int j = 1; j <= GlobalTerainData.size.first; ++j) {
					std::cout << GlobalTerainData.chr;
				}
			}
		}
	}
	namespace player {
		void Spawn(std::pair<int,int>pos) {

		}
		void GenerateStartPos() {
			if (misc::GeneratedStartPos == true)
				return;
			int x, y;
			x = misc::GenerateRandomNumber(0, 90);
			y = misc::GenerateRandomNumber(0, 90);

			while (x == y) { x = misc::GenerateRandomNumber(0, 90); }
			//std::cout << x << " " << y << '\n'; //DEGUB
			auto pos = std::make_pair(x, y);
			GlobalPlayerData.StartPos.swap(pos);
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