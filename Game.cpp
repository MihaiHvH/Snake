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
	//x = GenerateRandomNumber(150, GlobalScreenData.size.first - 150) + GlobalPlayerData.size;
	//y = GenerateRandomNumber(1, GlobalScreenData.size.second - 20, {{x, x}});

	int pos = GenerateRandomNumber(0, GlobalObjectData.objectMap.size() - 1);	

	while (GlobalObjectData.objectMap[pos].first <= 150)
		pos = GenerateRandomNumber(0, GlobalObjectData.objectMap.size() - 1);

	GlobalPlayerData.StartPos = pos;
	GlobalPlayerData.pos.first = pos;
	GlobalPlayerData.pos.second = pos - GlobalPlayerData.size + 1;
	
#ifdef _DEBUG
	std::cout << "Start pos: " << GlobalPlayerData.StartPos << '\n';
	std::cout << "First pos: " << GlobalPlayerData.pos.first << '\n';
	std::cout << "Last pos: " << GlobalPlayerData.pos.second << '\n';
#endif // _DEBUG

	for (int i = 0; i < GlobalPlayerData.size; ++i) {
		GlobalPlayerData.pixels.push_back(pos - i);
		GlobalPlayerData.backupPixels.push_back(pos - i);

#ifdef _DEBUG
		std::cout << "Pixel No " << i << " pos: " << GlobalPlayerData.pixels[i] << '\n';
		std::cout << "Backup Pixel No " << i << " pos: " << GlobalPlayerData.backupPixels[i] << '\n';
#endif // _DEBUG
	}
#ifdef _DEBUG
	std::cout << "Start pos generated successfully" << '\n';
#endif // _DEBUG

}

void pGame::UpdatePixels(int NewPos) {
	for (int i = 0; i <= GlobalPlayerData.size - 1; ++i)
		GlobalPlayerData.backupPixels.at(i) = GlobalPlayerData.pixels.at(i);

	for (int i = 0; i < GlobalPlayerData.size; ++i)
		if (i == 0)
			GlobalPlayerData.pixels.at(i) = NewPos;
		else
			GlobalPlayerData.pixels.at(i) = GlobalPlayerData.backupPixels.at(static_cast<std::vector<std::pair<int, int>, std::allocator<std::pair<int, int>>>::size_type>(i) - 1);
}

void pGame::MoveLeft() {
	std::pair<int, int> NewPos = 
	{
		GlobalPlayerData.pos.first - 1, 
		GlobalPlayerData.pixels.at(static_cast<std::vector<std::pair<int, int>, std::allocator<std::pair<int, int>>>::size_type>(GlobalPlayerData.size) - 2)
	};

	UpdatePixels(NewPos.first);

	GlobalPlayerData.pos = NewPos;
}

void pGame::MoveRight() {
	std::pair<int, int> NewPos =
	{
		GlobalPlayerData.pos.first + 1,
		GlobalPlayerData.pixels.at(static_cast<std::vector<std::pair<int, int>, std::allocator<std::pair<int, int>>>::size_type>(GlobalPlayerData.size) - 2)
	};

	UpdatePixels(NewPos.first);

	GlobalPlayerData.pos = NewPos;
}

void pGame::MoveUp() {
	std::pair<int, int> NewPos =
	{
		GlobalPlayerData.pos.first - GlobalScreenData.size.first / 30,
		GlobalPlayerData.pixels.at(static_cast<std::vector<std::pair<int, int>, std::allocator<std::pair<int, int>>>::size_type>(GlobalPlayerData.size) - 2)
	};

	UpdatePixels(NewPos.first);

	GlobalPlayerData.pos = NewPos;
}

void pGame::MoveDown() {
	std::pair<int, int> NewPos =
	{
		GlobalPlayerData.pos.first + GlobalScreenData.size.first / 30,
		GlobalPlayerData.pixels.at(static_cast<std::vector<std::pair<int, int>, std::allocator<std::pair<int, int>>>::size_type>(GlobalPlayerData.size) - 2)
	};

	UpdatePixels(NewPos.first);

	GlobalPlayerData.pos = NewPos;
}

bool pGame::BinarySearch(int n, std::vector<int> v) {
	std::sort(v.begin(), v.end());

	int Sol = -1, Left = 0, Right = v.size() - 1;
	while (Left <= Right) {
		int Mid = (Left + Right) / 2;
		if (v[Mid] == n) {
			Sol = Mid;
			break;
		}
		if (v[Mid] > n)
			Right = Mid - 1;
		if (v[Mid] < n)
			Left = Mid + 1;
	}
	if (Sol == -1) return false;

	return true;
}

void pGame::Grow() {
	if (GlobalObjectData.GrowObjectMap[GlobalPlayerData.pos.second] > 0 && !BinarySearch(GlobalPlayerData.pos.second - 1, GlobalPlayerData.pixels)) {// Left
		--GlobalPlayerData.pos.second;
		GlobalPlayerData.pixels.push_back(GlobalPlayerData.pos.second);
		GlobalPlayerData.backupPixels.push_back(GlobalPlayerData.pos.second);
		++GlobalPlayerData.size;
	}
	else if (GlobalObjectData.GrowObjectMap[GlobalPlayerData.pos.second] < (GlobalScreenData.size.first / 30) - 1 && !BinarySearch(GlobalPlayerData.pos.second + 1, GlobalPlayerData.pixels)) { // right
		++GlobalPlayerData.pos.second;
		GlobalPlayerData.pixels.push_back(GlobalPlayerData.pos.second);
		GlobalPlayerData.backupPixels.push_back(GlobalPlayerData.pos.second);
		++GlobalPlayerData.size;
	}
	else if (GlobalPlayerData.pos.second < (GlobalScreenData.size.second / 30 * (GlobalScreenData.size.first / 30 - 1)) + 1 && GlobalPlayerData.pos.second < (GlobalScreenData.size.second / 30 * GlobalScreenData.size.first / 30) - 1 && !BinarySearch(GlobalPlayerData.pos.second + GlobalScreenData.size.first / 30, GlobalPlayerData.pixels)) { // down
		GlobalPlayerData.pos.second += GlobalScreenData.size.first / 30;
		GlobalPlayerData.pixels.push_back(GlobalPlayerData.pos.second);
		GlobalPlayerData.backupPixels.push_back(GlobalPlayerData.pos.second);
		++GlobalPlayerData.size;
	}
	else if (GlobalPlayerData.pos.second > 0 && GlobalPlayerData.pos.second > GlobalScreenData.size.first / 30 - 1 && !BinarySearch(GlobalPlayerData.pos.second - GlobalScreenData.size.first / 30, GlobalPlayerData.pixels)) { // up
		GlobalPlayerData.pos.second -= GlobalScreenData.size.first / 30;
		GlobalPlayerData.pixels.push_back(GlobalPlayerData.pos.second);
		GlobalPlayerData.backupPixels.push_back(GlobalPlayerData.pos.second);
		++GlobalPlayerData.size;
	}
}

void pGame::Spawn() {
	if (!GlobalObjectData.spawned) {
		GlobalObjectData.objectId = GenerateRandomNumber(0, GlobalObjectData.objectMap.size() - 1);

		for (int& i : GlobalPlayerData.pixels) {
			if (GlobalObjectData.objectId == i) {
				Spawn();
			}
		}

		GlobalObjectData.spawned = true;
	}
#ifdef _DEBUG
	std::cout << "Object spawned successfully at " << GlobalObjectData.objectId << '\n';
#endif // _DEBUG

}

void pGame::ClearAllData() {
	canRender = false;

	GlobalPlayerData.StartPos = 0;
	GlobalPlayerData.pos = {0, 0};
	GlobalPlayerData.pixels.clear();
	GlobalPlayerData.backupPixels.clear();
	GlobalPlayerData.size = GlobalPlayerData.defSize;

	GlobalObjectData.GrowObjectMap.clear();
	GlobalObjectData.objectMap.clear();
	GlobalObjectData.objectId = 0;
	GlobalObjectData.spawned = false;

	GlobalScreenData.cal = { 0, 0 };

	GenerateObjectMap();
	GenerateStartPos();
	Spawn();

	canRender = true;

#ifdef _DEBUG
	std::cout << "Cleared all data" << '\n';
#endif // _DEBUG

}

void pGame::GenerateObjectMap() {
	GlobalObjectData.GrowObjectMap.clear();
	GlobalObjectData.objectMap.clear();
	GlobalScreenData.cal = { ((GlobalScreenData.size.first - 10) % 30) / 2, 20 + ((GlobalScreenData.size.second - 10) % 30) / 2 };
	for (int i = 0; i < (GlobalScreenData.size.second - 10) / 30; ++i) {
		for (int j = 0; j < (GlobalScreenData.size.first - 10) / 30; ++j) {
			GlobalObjectData.objectMap.push_back({ j * 30 + GlobalScreenData.cal.first, i * 30 + GlobalScreenData.cal.second });
			GlobalObjectData.GrowObjectMap.push_back(j);
		}
	}
#ifdef _DEBUG
	std::cout << "Generated Object Map successfully" << '\n';
	std::cout << "Calibration X: " << GlobalScreenData.cal.first << " Y: " << GlobalScreenData.cal.second << '\n';
#endif // _DEBUG
}