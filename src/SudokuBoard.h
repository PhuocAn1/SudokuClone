#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include "Cell.h"
#include "LevelsManager.h"

class SudokuBoard
{
public:
	SudokuBoard();
	~SudokuBoard();

	void close();
	void renderBoard(SDL_Renderer* gRenderer);
	int getCellSize();
	void loadNumbers(TTF_Font *gFont, SDL_Renderer* gRenderer);
	void loadLevel();
	void saveLevel(std::string gFileName);
	void chosenCell(int x, int y); // Set the current editable cell
	void setNumber(int number); // Set the value of the chosen cell 
	bool checkBoard(); // Check if the board is correct

	int boardState[9][9]; //Save the initial state of the board ???
	SDL_Texture *numbers[9];
	Cell gCells[9][9];
	LevelsManager levelsManager;
	int currentCellX = -1, currentCellY = -1;

private:
	const int CELL_SIZE = 50;
	const int TEXT_WIDTH = 12;
	const int TEXT_HEIGHT = 47;
};

