#pragma once
#include <vector>

struct PlayerData {
	std::pair<int, int>StartPos;
	std::pair<int, int> pos;
	std::pair<int, int> pos2;
	std::vector<std::pair<int, int>> pixels;
	std::vector<std::pair<int, int>> backupPixels;
	int size = 5;
};

struct ObjectData {
	std::pair<int, int> pos;
	bool spawned = false;
};

struct TerrainData {
	std::pair<int, int> size;
	char chr = '-';
};

struct ScreenData {
	std::pair<int, int> size;
	std::string consoleName = "";
};

struct DebugData {
	std::pair<int, int> size;
	std::pair<int, int> lastPos2;
};