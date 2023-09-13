#include "Game.h"

Game::Game() {
	gWindow = NULL;
	gRenderer = NULL;
	gFont = NULL;
}

Game::~Game() {
	close();
}

bool Game::init() {
	bool success = true;
	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		success = false;
	}
	else {
		//Initialize SDL_image
		int imgFlags = IMG_INIT_PNG;

		if (IMG_Init(imgFlags) != imgFlags) {
			std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
			success = false;
		}
		else {
			//Set texture filtering to linear
			if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
				std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
			}

			//Initialize SDL_ttf
			if (TTF_Init() == -1) {
				std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
				success = false;
			}
			else {
				//Load font
				gFont = TTF_OpenFont("res/font/THSpatial.ttf", 24);
				if (gFont == NULL) {
					std::cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
					success = false;
				}
				else {
					//Create window
					gWindow = SDL_CreateWindow("Sudoku_ver2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
						SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

					if (gWindow == NULL) {
						std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
						success = false;
					}
					else {
						//Create renderer for window
						gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
						if (gRenderer == NULL) {
							std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
							success = false;
						}
						else {
							//Initialize renderer color
							SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
							//Load numbers
							sudokuBoard.loadNumbers(gFont, gRenderer);
						}
					}
				}
			}
		}
	}
	return success;
}

void Game::processEvents() {
	//Process events for main menu
	if (main_menu == true) {
		while (SDL_PollEvent(&e) > 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				if (startButton.isClicked(x, y)) {
					main_menu = false;
					difficultyMenu = true;
				}

				if (continueButton.getClickAble() == true) {
					if (continueButton.isClicked(x, y)) {
						main_menu = false;
						gameRunning = true;
					}
				}
			}
		}
	}

	//Process events for difficulty menu
	if (difficultyMenu == true) {
		while (SDL_PollEvent(&e) > 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				if (easyButton.isClicked(x, y)) {
					difficultyMenu = false;
					gameRunning = true;
					sudokuBoard.levelsManager.setDifficulty(0);
					sudokuBoard.loadLevel();
				}
				else if (mediumButton.isClicked(x, y)) {
					difficultyMenu = false;
					gameRunning = true;
					sudokuBoard.levelsManager.setDifficulty(1);
					sudokuBoard.loadLevel();
				}
				else if (hardButton.isClicked(x, y)) {
					difficultyMenu = false;
					gameRunning = true;
					sudokuBoard.levelsManager.setDifficulty(2);
					sudokuBoard.loadLevel();
				}
				else if (backButton.isClicked(x, y)) {
					difficultyMenu = false;
					main_menu = true;
				}
			}
		}
	}

	//Process events for game
	if (gameRunning == true) {
		while (SDL_PollEvent(&e) > 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				int x, y;
				SDL_GetMouseState(&x, &y);

				if (x >= 0 && x <= 450 && y >= 0 && y <= 450) {
					sudokuBoard.chosenCell(y / 50, x / 50);
				}
				else {
					if (saveButton.isClicked(x, y)) {
						printf("Save button clicked\n");
					}

					if (loadButton.isClicked(x, y)) {
						printf("Load button clicked\n");
					}

					if (checkBoardButton.isClicked(x, y)) {
						if (sudokuBoard.checkBoard() == true) {
							printf("Board is correct\n");
						}
						else {
							printf("Board is incorrect\n");
						}
					}
				}
			}
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym)
				{
				case SDLK_1:
					sudokuBoard.setNumber(1);
					break;
				case SDLK_2:
					sudokuBoard.setNumber(2);
					break;
				case SDLK_3:
					sudokuBoard.setNumber(3);
					break;
				case SDLK_4:
					sudokuBoard.setNumber(4);
					break;
				case SDLK_5:
					sudokuBoard.setNumber(5);
					break;
				case SDLK_6:
					sudokuBoard.setNumber(6);
					break;
				case SDLK_7:
					sudokuBoard.setNumber(7);
					break;
				case SDLK_8:
					sudokuBoard.setNumber(8);
					break;
				case SDLK_9:
					sudokuBoard.setNumber(9);
					break;
				case SDLK_BACKSPACE:
					sudokuBoard.setNumber(0);
					break;
				case SDLK_ESCAPE:
					gameRunning = false;
					main_menu = true;
					continueButton.setClickAble(true);
					break;
				default:
					break;
				}
			}
		}
	}
}

void Game::makeButton() {
	SDL_Color buttonColor = { 0, 0, 0, 0 };
	
	//Make the start button
	startButton.setButton(SCREEN_WIDTH / 2 - 150 / 2, SCREEN_HEIGHT / 2 - 25, 150, 50, buttonColor, "New Game", gRenderer, 18);

	//Make the continue button
	continueButton.setButton(SCREEN_WIDTH / 2 - 150 / 2, SCREEN_HEIGHT / 2 - 100, 150, 50, buttonColor, "Continue", gRenderer, 18);
	continueButton.setClickAble(false);

	//Make the difficulty buttons
	easyButton.setButton(SCREEN_WIDTH / 2 - 100 / 2, SCREEN_HEIGHT / 2 - 125, 100, 50, buttonColor, "Easy", gRenderer, 18);

	mediumButton.setButton(SCREEN_WIDTH / 2 - 100 / 2, SCREEN_HEIGHT / 2 - 50, 100, 50, buttonColor, "Medium", gRenderer, 18);

	hardButton.setButton(SCREEN_WIDTH / 2 - 100 / 2, SCREEN_HEIGHT / 2 + 25, 100, 50, buttonColor, "Hard", gRenderer, 18);

	//Make the back button
	backButton.setButton(SCREEN_WIDTH / 2 - 100 / 2, SCREEN_HEIGHT / 2 + 100, 100, 50, buttonColor, "Back", gRenderer, 18);

	//Make the save button
	saveButton.setButton(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 25,
			100, 50, buttonColor, "Save", gRenderer, 18);

	//Make the load button
	loadButton.setButton(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 100,
					100, 50, buttonColor, "Load", gRenderer, 18);

	//Make the check button
	checkBoardButton.setButton(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 175,
							100, 50, buttonColor, "Check", gRenderer, 18);
}

void Game::close() {
	//Free loaded images
	//gSpriteSheetTexture.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	sudokuBoard.close();

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Game::update() {

}

void Game::render() {
	//Clear screen
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderClear(gRenderer);

	if (main_menu == true) {
		startButton.renderButton(gRenderer);
		continueButton.renderButton(gRenderer);
	}

	if (difficultyMenu == true) {
		easyButton.renderButton(gRenderer);
		mediumButton.renderButton(gRenderer);
		hardButton.renderButton(gRenderer);
		backButton.renderButton(gRenderer);
	}

	if (gameRunning == true) {
		//Draw board
		sudokuBoard.renderBoard(gRenderer);

		//Draw save button
		saveButton.renderButton(gRenderer);

		//Draw load button
		loadButton.renderButton(gRenderer);

		//Draw check button
		checkBoardButton.renderButton(gRenderer);
	}
	
	//Update screen
	SDL_RenderPresent(gRenderer);
}

void Game::run() {
	//sudokuBoard.loadLevel();//Temp
	makeButton();
	while (!quit) { //Main loop for the game
		processEvents();

		render();

		SDL_Delay(16);
	}
	//Free resources and close SDL
	close();
}