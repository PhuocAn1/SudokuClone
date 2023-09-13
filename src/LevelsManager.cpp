#include "LevelsManager.h"

LevelsManager::LevelsManager() {

}

LevelsManager::~LevelsManager() {

}

void LevelsManager::setDifficulty(int difficultyLevel) {
	this->difficultyLevel = difficultyLevel;
}

int LevelsManager::setRandomLevel() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 5);

	int randomLevel = dis(gen);
	return randomLevel;
}

void LevelsManager::loadLevel(int sudokuBoard[9][9]) {
	int randomLevel = setRandomLevel();
	if (difficultyLevel != -1) {

		std::string path = "levels/" + difficulty[difficultyLevel] + "/" + "level" + std::to_string(randomLevel) + ".txt";
		std::ifstream LevelFile;

		LevelFile.open(path);

		int row = 0, col = 0;

		if (!LevelFile.is_open()) {
			std::cout << "Error opening file" << std::endl;
		}
		else {
			char inChar;

			while (LevelFile.get(inChar)) {
				if (inChar == '\n') {//While it is able to get a character from the text file
					col = 0;
					row++;
				}
				else {
					int value = inChar - '0';
					sudokuBoard[row][col] = value;
					col++;
				}
			}
		}
		std::cout << "Done loading level" << randomLevel << std::endl; //Debugging purposes
		LevelFile.close();
	}	
}