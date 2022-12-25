#pragma once

#include <vector>
#include <string>

struct PlayerData {
	int StartPos;
	std::pair<int, int> pos; // first = pos cap ; second = pos coada
	std::vector<int> pixels;
	std::vector<int> backupPixels;
	int size = 5;
	int defSize = 5;
};

struct ObjectData {
	std::vector<int> GrowObjectMap;
	std::vector<std::pair<int, int>> objectMap;
	int objectId = 0;
	bool spawned = false;
};

struct ScreenData {
	std::pair<int, int> size;
	std::pair<int, int> cal;
	std::string windowName = "";
	bool gameOver = false;
};