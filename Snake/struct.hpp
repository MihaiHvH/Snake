#pragma once
#include <vector>

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

struct TerainData {
	std::pair<int, int> size; //size(w, h)
	char chr;
};