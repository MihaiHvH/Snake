#pragma once

#include "GameData.h"

class pGame {
public:
		PlayerData GlobalPlayerData;
		ScreenData GlobalScreenData;
		ObjectData GlobalObjectData;

		bool canRender = false;

		void GenerateStartPos();

		void UpdatePixels(int NewPos);

		void Grow();

		void MoveLeft();
		void MoveRight();
		void MoveUp();
		void MoveDown();

		void GenerateObjectMap();
		void Spawn();
		
		bool BinarySearch(int n, std::vector<int> v);

		void ClearAllData();

private:
	int GenerateRandomNumber(int min, int max, std::vector<std::pair<int, int>>exclude = {});
};
