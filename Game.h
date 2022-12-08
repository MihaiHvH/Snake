#pragma once

#include "GameData.h"

class pGame {
public:
		PlayerData GlobalPlayerData;
		ScreenData GlobalScreenData;
		ObjectData GlobalObjectData;

		bool canRender = false;

		void GenerateStartPos();

		void UpdatePixels(std::pair<int, int> NewPos);

		void Grow();

		void MoveLeft();
		void MoveRight();
		void MoveUp();
		void MoveDown();

		bool IsPixelPos(std::pair<int, int> pixel, std::pair<int, int> detect, bool exact = false);
		bool IsPixelPos(std::vector<std::pair<int, int>> pixel, std::pair<int, int> detect, bool exact = false);

		void GenerateObjectMap();
		void Spawn();
private:
	bool BinarySearch(int n, int max, std::vector<int> v);
	int GenerateRandomNumber(int min, int max, std::vector<std::pair<int, int>>exclude = {});
};
