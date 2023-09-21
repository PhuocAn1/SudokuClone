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

void LevelsManager::saveLevel(Cell gCell[9][9], std::string gFileName) {
	std::string path = "saves/" + gFileName + ".bin";
	std::fstream gSaveFile;
	int value = 0;

	gSaveFile.open(path,std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);

	if (!gSaveFile.is_open()) {
		std::cout << "Error opening file" << std::endl;
	}
	else {
		for (int row = 0; row < 9; row++) {
			for (int col = 0; col < 9; col++) {

				if (gCell[row][col].isEditable()) {
					value = gCell[row][col].getCellValue();
				}
				else {
					value = -gCell[row][col].getCellValue(); //If the cell is not editable, save the value as negative
				}
				gSaveFile.write(reinterpret_cast<const char*>(&value), sizeof(value));
			}
		}
	}

	gSaveFile.seekg(0, std::ios::beg);

	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {

			gSaveFile.read(reinterpret_cast<char*>(&value), sizeof(value));
			printf("%d ", value);
		}

		printf("\n");
	}

	gSaveFile.close();
}