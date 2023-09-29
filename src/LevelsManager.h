#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <filesystem>
#include <vector>
#include "Cell.h"

class LevelsManager
{
public:
	LevelsManager();
	~LevelsManager();
	void setDifficulty(int difficultyLevel);
	void loadUserSavedLevel(Cell gCell[9][9], std::string gFileName);
	int setRandomLevel();
	std::vector<std::string> getFileName();
	void depopulateFileName();
	void loadLevel(Cell gCell[9][9]);
	void saveLevel(Cell gCell[9][9], std::string gFileName);

private:
	std::string difficulty[3] = {"easy", "medium", "hard"};
	int difficultyLevel = -1;
	std::vector<std::string> gFileName;
};

