#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <Windows.h>

#include "struct.hpp"
#include "functions.hpp"

void main(void) {
	srand((unsigned)time(NULL));

	functions::player::GenerateStartPos();
	functions::terain::GenerateMatrice();
}