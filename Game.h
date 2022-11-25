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

		bool GetPixelPos(std::pair<int, int> pixel, std::pair<int, int> detect, bool exact = false);

		void GenerateObjectMap();
		void Spawn();
private:
	int GenerateRandomNumber(int min, int max, std::vector<std::pair<int, int>>exclude = {});
};
