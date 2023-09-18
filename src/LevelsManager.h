#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include "Cell.h"

class LevelsManager
{
public:
	LevelsManager();
	~LevelsManager();
	void setDifficulty(int difficultyLevel);
	void loadUserLevel(Cell sudokuBoard[9][9]);
	int setRandomLevel();
	void loadLevel(int sudokuBoard[9][9]);
private:
	std::string difficulty[3] = {"easy", "medium", "hard"};
	int difficultyLevel = -1;
};

