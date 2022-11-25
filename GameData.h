#pragma once

#include <vector>
#include <string>

struct PlayerData {
	std::pair<int, int>StartPos;
	std::pair<int, int> pos;
	std::pair<int, int> pos2;
	std::vector<std::pair<int, int>> pixels;
	std::vector<std::pair<int, int>> backupPixels;
	int size = 5;
};

struct ObjectData {
	std::vector<std::pair<int, int>> objectMap;
	int ObjectMapSize = 0;
	int objectId = 0;
	bool spawned = false;
};

struct ScreenData {
	std::pair<int, int> size;
	std::string windowName = "";
};