#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include "SudokuBoard.h"
#include "Button.h"

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

private:
	void processEvents();
	void update();
	void render();

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 451;
	const int textInputWidth = 300;
	const int textInputHeight = 30;
	bool quit = false, main_menu = true, gameRunning = false, 
		difficultyMenu = false, saveMenu = false;

	SDL_Rect textInputRect, textRect;
	SudokuBoard sudokuBoard;
	Button startButton, easyButton, mediumButton, hardButton, backButton, continueButton, saveButton, loadButton, solutionButton;
	Button checkBoardButton;
};

