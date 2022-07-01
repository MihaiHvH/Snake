#pragma once
#include <vector>
#include <set>
#include <utility>

struct PlayerData {
	std::pair<int, int>StartPos;// StartPos(x,y)
	std::pair<int, int> pos; // pos(x,y)
	int size = 5;
	int speed = 100;
};

struct ObjectData {
	std::pair<int, int> pos; // pos(x,y)
	std::vector<std::pair<int, int>> posHistory;
	int saturation = 1;
};

struct TerrainData {
	std::pair<int, int> size; //size(w, h)
	std::set<std::pair<char, std::pair<int, int>>> pixel; //auto pixel = std::make_pair(L"M", std::make_pair(1, 1)); // pixel example
	char chr;
};

struct ScreenData {
	std::pair<int, int> size; //size(w, h)
};