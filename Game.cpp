#include "Game.h"
#include "main.h"
#include <random>

int pGame::GenerateRandomNumber(int min, int max, std::vector<std::pair<int, int>> exclude) {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(min, max);
	
	for (auto& i : exclude)
		if (dist(mt) == i.second or dist(mt) == i.first)
			return GenerateRandomNumber(min, max, exclude);

	return dist(mt);
}

void pGame::GenerateStartPos() {
	int x, y;
	x = GenerateRandomNumber(150, GlobalScreenData.size.first - 150) + GlobalPlayerData.size;
	y = GenerateRandomNumber(1, GlobalScreenData.size.second - 20, {{x, x}});

	auto pos0 = std::make_pair(x, y);
	auto pos1 = std::make_pair(x, y);
	auto pos2 = std::make_pair(x - GlobalPlayerData.size + 1, y);

	GlobalPlayerData.StartPos.swap(pos0);
	GlobalPlayerData.pos.swap(pos1);
	GlobalPlayerData.pos2.swap(pos2);

	GlobalPlayerData.pixels.push_back(std::make_pair(x, y));
	GlobalPlayerData.backupPixels.push_back(std::make_pair(x, y));

	for (int i = 1; i <= GlobalPlayerData.size - 1; ++i) {
		GlobalPlayerData.pixels.push_back(std::make_pair(x - (i * 10 + i * 20), y));
		GlobalPlayerData.backupPixels.push_back(std::make_pair(x - (i * 10 + i * 20), y));
	}
}

void pGame::UpdatePixels(std::pair<int, int> NewPos) {
	for (int i = 0; i <= GlobalPlayerData.size - 1; ++i)
		GlobalPlayerData.backupPixels.at(i) = GlobalPlayerData.pixels.at(i);

	for (int i = 0; i <= GlobalPlayerData.size - 1; ++i)
		if (i == 0)
			GlobalPlayerData.pixels.at(i) = NewPos;
		else
			GlobalPlayerData.pixels.at(i) = GlobalPlayerData.backupPixels.at(static_cast<std::vector<std::pair<int, int>, std::allocator<std::pair<int, int>>>::size_type>(i) - 1);
}

void pGame::MoveLeft() {
	std::pair<int, int> NewPos = std::make_pair(GlobalPlayerData.pos.first - 30, GlobalPlayerData.pos.second);
	std::pair<int, int> NewPos2 = GlobalPlayerData.pixels.at(static_cast<std::vector<std::pair<int, int>, std::allocator<std::pair<int, int>>>::size_type>(GlobalPlayerData.size) - 2);

	UpdatePixels(NewPos);

	GlobalPlayerData.pos.swap(NewPos);
	GlobalPlayerData.pos2.swap(NewPos2);
}

void pGame::MoveRight() {
	std::pair<int, int> NewPos = std::make_pair(GlobalPlayerData.pos.first + 30, GlobalPlayerData.pos.second);
	std::pair<int, int> NewPos2 = GlobalPlayerData.pixels.at(static_cast<std::vector<std::pair<int, int>, std::allocator<std::pair<int, int>>>::size_type>(GlobalPlayerData.size) - 2);

	UpdatePixels(NewPos);

	GlobalPlayerData.pos.swap(NewPos);
	GlobalPlayerData.pos2.swap(NewPos2);
}

void pGame::MoveUp() {
	std::pair<int, int> NewPos = std::make_pair(GlobalPlayerData.pos.first, GlobalPlayerData.pos.second - 30);
	std::pair<int, int> NewPos2 = GlobalPlayerData.pixels.at(static_cast<std::vector<std::pair<int, int>, std::allocator<std::pair<int, int>>>::size_type>(GlobalPlayerData.size) - 2);

	UpdatePixels(NewPos);

	GlobalPlayerData.pos.swap(NewPos);
	GlobalPlayerData.pos2.swap(NewPos2);
}

void pGame::MoveDown() {
	std::pair<int, int> NewPos = std::make_pair(GlobalPlayerData.pos.first, GlobalPlayerData.pos.second + 30);
	std::pair<int, int> NewPos2 = GlobalPlayerData.pixels.at(static_cast<std::vector<std::pair<int, int>, std::allocator<std::pair<int, int>>>::size_type>(GlobalPlayerData.size) - 2);

	UpdatePixels(NewPos);

	GlobalPlayerData.pos.swap(NewPos);
	GlobalPlayerData.pos2.swap(NewPos2);
}

void pGame::Grow() {
	GlobalPlayerData.size += 1;
	if (GlobalPlayerData.pos2.first + 30 <= GlobalScreenData.size.first) {
		std::pair<int, int> NewPos2 = std::make_pair(GlobalPlayerData.pos2.first + 30, GlobalPlayerData.pos2.second);
		GlobalPlayerData.pos2.swap(NewPos2);
		GlobalPlayerData.pixels.push_back(GlobalPlayerData.pos2);
		GlobalPlayerData.backupPixels.push_back(GlobalPlayerData.pos2);
	}
	else if (GlobalPlayerData.pos2.first - 30 >= 1) {
		std::pair<int, int> NewPos2 = std::make_pair(GlobalPlayerData.pos2.first - 30, GlobalPlayerData.pos2.second);
		GlobalPlayerData.pos2.swap(NewPos2);
		GlobalPlayerData.pixels.push_back(GlobalPlayerData.pos2);
		GlobalPlayerData.backupPixels.push_back(GlobalPlayerData.pos2);
	}
	else if (GlobalPlayerData.pos2.second + 30 <= GlobalScreenData.size.second) {
		std::pair<int, int> NewPos2 = std::make_pair(GlobalPlayerData.pos2.first, GlobalPlayerData.pos2.second + 30);
		GlobalPlayerData.pos2.swap(NewPos2);
		GlobalPlayerData.pixels.push_back(GlobalPlayerData.pos2);
		GlobalPlayerData.backupPixels.push_back(GlobalPlayerData.pos2);
	}
	else if (GlobalPlayerData.pos2.first - 30 >= 1) {
		std::pair<int, int> NewPos2 = std::make_pair(GlobalPlayerData.pos2.first, GlobalPlayerData.pos2.second - 30);
		GlobalPlayerData.pos2.swap(NewPos2);
		GlobalPlayerData.pixels.push_back(GlobalPlayerData.pos2);
		GlobalPlayerData.backupPixels.push_back(GlobalPlayerData.pos2);
	}
}

void pGame::Spawn() {
	if (!GlobalObjectData.spawned) {
		GlobalObjectData.objectId = GenerateRandomNumber(1, GlobalObjectData.ObjectMapSize);
		GlobalObjectData.spawned = true;
	}
}

bool pGame::GetPixelPos(std::pair<int, int> pixel, std::pair<int, int> detect, bool exact) {
	if (std::abs(pixel.first - detect.first) <= 20 && std::abs(pixel.second - detect.second) <= 20 && !exact)
		return true;
	if ((detect.first >= pixel.first && detect.first + 20 <= pixel.first + 20) && (detect.second >= pixel.second && detect.second + 20 <= pixel.second + 20))
		return true;
	return false;
}

void pGame::GenerateObjectMap() {
	auto pos = GlobalPlayerData.StartPos;

	int i = 0;
	int j = 0;
	int k = 0;
	int z = 0;

	while (pos.first <= GlobalScreenData.size.first - 30) {
		GlobalObjectData.objectMap.push_back({GlobalPlayerData.StartPos.first + 10 * i + 20 * i, GlobalPlayerData.StartPos.second});
		GlobalObjectData.ObjectMapSize += 1;
		++i;
		pos = { GlobalPlayerData.StartPos.first + 10 * i + 20 * i, GlobalPlayerData.StartPos.second };
	}
	pos = GlobalPlayerData.StartPos;
	while (pos.first >= 1) {
		GlobalObjectData.objectMap.push_back({GlobalPlayerData.StartPos.first - 10 * j - 20 * j, GlobalPlayerData.StartPos.second });
		GlobalObjectData.ObjectMapSize += 1;
		++j;
		pos = { GlobalPlayerData.StartPos.first - 10 * j - 20 * j, GlobalPlayerData.StartPos.second };
	}
	pos = GlobalPlayerData.StartPos;
	while (pos.second >= 1) {
		GlobalObjectData.objectMap.push_back({ GlobalPlayerData.StartPos.first, GlobalPlayerData.StartPos.second - 10 * k - 20 * k });
		GlobalObjectData.ObjectMapSize += 1;
		++k;
		pos = { GlobalPlayerData.StartPos.first, GlobalPlayerData.StartPos.second - 10 * k - 20 * k };
	}
	pos = GlobalPlayerData.StartPos;
	while (pos.second <= GlobalScreenData.size.second - 30) {
		GlobalObjectData.objectMap.push_back({ GlobalPlayerData.StartPos.first, GlobalPlayerData.StartPos.second + 10 * z + 20 * z });
		GlobalObjectData.ObjectMapSize += 1;
		++z;
		pos = { GlobalPlayerData.StartPos.first, GlobalPlayerData.StartPos.second + 10 * z + 20 * z };
	}
	pos = GlobalPlayerData.StartPos;
	for (int hh = 0; hh < k; ++hh) {
		for (int h = 0; h < i; ++h) {
			GlobalObjectData.objectMap.push_back({ GlobalPlayerData.StartPos.first + h * 10 + h * 20, GlobalPlayerData.StartPos.second - 10 * hh - 20 * hh });
			GlobalObjectData.ObjectMapSize += 1;
		}
		for (int h = 0; h < j; ++h) {
			GlobalObjectData.objectMap.push_back({ GlobalPlayerData.StartPos.first - h * 10 - h * 20, GlobalPlayerData.StartPos.second - 10 * hh - 20 * hh });
			GlobalObjectData.ObjectMapSize += 1;
		}
	}
	for (int hh = 0; hh < z; ++hh) {
		for (int h = 0; h < i; ++h) {
			GlobalObjectData.objectMap.push_back({ GlobalPlayerData.StartPos.first + h * 10 + h * 20, GlobalPlayerData.StartPos.second + 10 * hh + 20 * hh });
			GlobalObjectData.ObjectMapSize += 1;
		}
		for (int h = 0; h < j; ++h) {
			GlobalObjectData.objectMap.push_back({ GlobalPlayerData.StartPos.first - h * 10 - h * 20, GlobalPlayerData.StartPos.second + 10 * hh + 20 * hh });
			GlobalObjectData.ObjectMapSize += 1;
		}
	}
}