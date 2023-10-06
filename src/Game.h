#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include "SudokuBoard.h"
#include "Button.h"
#include "PopUpWindow.h"

class Game
{
public:
	SDL_Window *gWindow;
	SDL_Renderer *gRenderer;
	SDL_Event e;
	TTF_Font *gFont;
	std::string gTextInputBuffer;

	Game();
	~Game();
	void run();
	bool init();
	void close();
	void makeButton();
	void renderTextInput();
	void renderFileName();

private:
	void processEvents();
	void update();
	void render();

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	const int textInputWidth = 300;
	const int textInputHeight = 30;
	size_t gChosenLine = -1;
	bool quit = false, main_menu = true, gameRunning = false, 
		difficultyMenu = false, saveMenu = false, loadMenu = false, fromMainMenu = false, fromGameRunning = false;

	size_t warpHead = 0, warpTail = 0; //To display the file names in the load menu

	SDL_Rect textInputRect, textRect, displayFileName[5];
	SudokuBoard sudokuBoard;
	Button startButton, easyButton, mediumButton, hardButton, backButton, continueButton, saveButton, loadButton;
	Button checkBoardButton, solveBoardButton, newGameButton, resetBoardButton;

	std::string gChosenFileName;
	std::vector<std::string> fileNames;

	PopUpWindow gMessageWindow;
};

