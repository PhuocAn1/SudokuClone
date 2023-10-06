#include "Game.h"

Game::Game() {
	gWindow = NULL;
	gRenderer = NULL;
	gFont = NULL;

	textInputRect = { SCREEN_WIDTH / 2 - textInputWidth / 2, SCREEN_HEIGHT / 2 - textInputHeight / 2, 
			textInputWidth, textInputHeight };

	textRect = { SCREEN_WIDTH / 2 - textInputWidth / 2, SCREEN_HEIGHT / 2 - textInputHeight / 2, 0, 0};

	for (int i = 0; i < 5; i++) {
		displayFileName[i] = { SCREEN_WIDTH / 2 - textInputWidth / 2, 100 + 30 * i, textInputWidth, textInputHeight };
	}
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

					//Reset button positions
					easyButton.setButtonPostion(SCREEN_WIDTH / 2 - 100 / 2, SCREEN_HEIGHT / 2 - 125);
					mediumButton.setButtonPostion(SCREEN_WIDTH / 2 - 100 / 2, SCREEN_HEIGHT / 2 - 50);
					hardButton.setButtonPostion(SCREEN_WIDTH / 2 - 100 / 2, SCREEN_HEIGHT / 2 + 25);
					backButton.setButtonPostion(SCREEN_WIDTH / 2 - 100 / 2, SCREEN_HEIGHT / 2 + 100);

					//Reset button sizes
					easyButton.setButtonSize(100, 50);
					mediumButton.setButtonSize(100, 50);
					hardButton.setButtonSize(100, 50);
					backButton.setButtonSize(100, 50);
				}

				if (continueButton.getClickAble() == true) {
					if (continueButton.isClicked(x, y)) {
						main_menu = false;
						gameRunning = true;

						//Reset button positions
						saveButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 25);
						loadButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 100);
						checkBoardButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 175);
					
						//Reset button sizes
						saveButton.setButtonSize(100, 50);
						loadButton.setButtonSize(100, 50);
						checkBoardButton.setButtonSize(100, 50);
					}
				}

				if (loadButton.isClicked(x, y)) {
					loadMenu = true;
					main_menu = false;
					fromMainMenu = true;

					fileNames = sudokuBoard.levelsManager.getFileName();

					warpHead = 0;

					if (fileNames.size() > 5) {
						warpTail = 5;
					}
					else {
						warpTail = fileNames.size();
					}

					for (int i = 0; i < fileNames.size(); i++) {
						printf("%s\n", fileNames[i].c_str());
					}

					//Set button positions
					loadButton.setButtonPostion(SCREEN_WIDTH / 2 - 225 / 2, SCREEN_HEIGHT / 2 - 50 / 2 + 100);
					backButton.setButtonPostion(SCREEN_WIDTH / 2 - 225 / 2 + 125, SCREEN_HEIGHT / 2 - 50 / 2 + 100);

					//Set button sizes
					loadButton.setButtonSize(100, 50);
					backButton.setButtonSize(100, 50);
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

					continueButton.setClickAble(true);

					//Reset button positions
					saveButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 25);
					loadButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 100);
					checkBoardButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 175);

					//Reset button sizes
					saveButton.setButtonSize(100, 50);
					loadButton.setButtonSize(100, 50);
					checkBoardButton.setButtonSize(100, 50);

					sudokuBoard.levelsManager.setDifficulty(0);
					sudokuBoard.loadLevel();
				}
				else if (mediumButton.isClicked(x, y)) {
					difficultyMenu = false;
					gameRunning = true;

					continueButton.setClickAble(true);

					//Reset button positions
					saveButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 25);
					loadButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 100);
					checkBoardButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 175);

					//Reset button sizes
					saveButton.setButtonSize(100, 50);
					loadButton.setButtonSize(100, 50);
					checkBoardButton.setButtonSize(100, 50);

					sudokuBoard.levelsManager.setDifficulty(1);
					sudokuBoard.loadLevel();
				}
				else if (hardButton.isClicked(x, y)) {
					difficultyMenu = false;
					gameRunning = true;

					continueButton.setClickAble(true);

					//Reset button positions
					saveButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 25);
					loadButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 100);
					checkBoardButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 175);

					//Reset button sizes
					saveButton.setButtonSize(100, 50);
					loadButton.setButtonSize(100, 50);
					checkBoardButton.setButtonSize(100, 50);

					sudokuBoard.levelsManager.setDifficulty(2);
					sudokuBoard.loadLevel();
				}
				else if (backButton.isClicked(x, y)) {
					if (fromGameRunning == true) {
						gameRunning = true;
						difficultyMenu = false;

						//Reset button positions
						saveButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 25);
						loadButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 100);
						checkBoardButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 175);

						//Reset button sizes
						saveButton.setButtonSize(100, 50);
						loadButton.setButtonSize(100, 50);
						checkBoardButton.setButtonSize(100, 50);
					}
					else {
						difficultyMenu = false;
						main_menu = true;

						//Reset button positions
						startButton.setButtonPostion(SCREEN_WIDTH / 2 - 150 / 2, SCREEN_HEIGHT / 2 - 25);
						continueButton.setButtonPostion(SCREEN_WIDTH / 2 - 150 / 2, SCREEN_HEIGHT / 2 - 100);
						loadButton.setButtonPostion(SCREEN_WIDTH / 2 - 150 / 2, SCREEN_HEIGHT / 2 + 50);

						//Reset button sizes
						startButton.setButtonSize(150, 50);
						continueButton.setButtonSize(150, 50);
						loadButton.setButtonSize(150, 50);
					}	
				}
			}
			else if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					if (fromGameRunning == true) {
						gameRunning = true;
						difficultyMenu = false;

						//Reset button positions
						saveButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 25);
						loadButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 100);
						checkBoardButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 175);

						//Reset button sizes
						saveButton.setButtonSize(100, 50);
						loadButton.setButtonSize(100, 50);
						checkBoardButton.setButtonSize(100, 50);
					}
					else {
						difficultyMenu = false;
						main_menu = true;

						//Reset button positions
						startButton.setButtonPostion(SCREEN_WIDTH / 2 - 150 / 2, SCREEN_HEIGHT / 2 - 25);
						continueButton.setButtonPostion(SCREEN_WIDTH / 2 - 150 / 2, SCREEN_HEIGHT / 2 - 100);
						loadButton.setButtonPostion(SCREEN_WIDTH / 2 - 150 / 2, SCREEN_HEIGHT / 2 + 50);

						//Reset button sizes
						startButton.setButtonSize(150, 50);
						continueButton.setButtonSize(150, 50);
						loadButton.setButtonSize(150, 50);
					}
				} 
			}
		}
	}

	//Process events for game
	if (gameRunning == true) {
		while (SDL_PollEvent(&e) > 0) {
			if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) {
				//Check close for which window
				if (SDL_GetWindowID(gWindow) == e.window.windowID) {
					quit = true;
					gMessageWindow.close();
				}
				else if (SDL_GetWindowID(gMessageWindow.getWindow()) == e.window.windowID) {
					gMessageWindow.close();
				}
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				int x, y;
				SDL_GetMouseState(&x, &y);

				if (x >= 0 && x <= 450 && y >= 0 && y <= 450) {
					sudokuBoard.chosenCell(y / 50, x / 50);
				}
				else {
					if (saveButton.isClicked(x, y)) {
						saveMenu = true;
						gameRunning = false;
						
						//Reset button positions
						saveButton.setButtonPostion(SCREEN_WIDTH / 2 - textInputWidth / 2 + 40, SCREEN_HEIGHT / 2 - textInputHeight / 2 + 50);
						backButton.setButtonPostion(SCREEN_WIDTH / 2 - textInputWidth / 2 + 160, SCREEN_HEIGHT / 2 - textInputHeight / 2 + 50);

						//Reset button sizes
						saveButton.setButtonSize(100, 50);
						backButton.setButtonSize(100, 50);

						SDL_StartTextInput();
					}

					if (loadButton.isClicked(x, y)) {
						fileNames = sudokuBoard.levelsManager.getFileName();

						warpHead = 0;

						if (fileNames.size() > 5) {
							warpTail = 5;
						}
						else {
							warpTail = fileNames.size();
						}

						for (int i = 0; i < fileNames.size(); i++) {
							printf("%s\n", fileNames[i].c_str());
						}

						loadMenu = true;
						gameRunning = false;

						//Set button positions
						loadButton.setButtonPostion(SCREEN_WIDTH / 2 - 225 / 2, SCREEN_HEIGHT / 2 - 50 / 2 + 100);
						backButton.setButtonPostion(SCREEN_WIDTH / 2 - 225 / 2 + 125, SCREEN_HEIGHT / 2 - 50 / 2 + 100);
					}

					if (checkBoardButton.isClicked(x, y)) {
						if (sudokuBoard.checkBoard() == true) {
							gMessageWindow.setWindowTitle("Board is correct");
							gMessageWindow.setMessage("Board is correct");
							//Create pop up window ????
							if (gMessageWindow.getWindow() == NULL && gMessageWindow.getRenderer() == NULL) {
								if (gMessageWindow.init() == false) {
									printf("Failed to initialize error window\n");
								}
								else {
									SDL_RaiseWindow(gMessageWindow.getWindow());
								}
							}
							printf("Board is correct\n");
						}
						else {
							gMessageWindow.setWindowTitle("Board is incorrect");
							gMessageWindow.setMessage("Board is incorrect");
							//Create pop up window ????
							if (gMessageWindow.getWindow() == NULL && gMessageWindow.getRenderer() == NULL) {
								if (gMessageWindow.init() == false) {
									printf("Failed to initialize error window\n");
								}
								else {
									SDL_RaiseWindow(gMessageWindow.getWindow());
								}
							}
							printf("Board is incorrect\n");
						}
					}

					if (solveBoardButton.isClicked(x, y)) {
						if (sudokuBoard.solveBoard() == true) {
							gMessageWindow.setWindowTitle("Board is solved");
							gMessageWindow.setMessage("Board is solved");
							//Create pop up window ????
							if (gMessageWindow.getWindow() == NULL && gMessageWindow.getRenderer() == NULL) {
								if (gMessageWindow.init() == false) {
									printf("Failed to initialize error window\n");
								}
								else {
									SDL_RaiseWindow(gMessageWindow.getWindow());
								}
							}
							printf("Board is solved\n");
						}
						else {
							gMessageWindow.setWindowTitle("Board is unsolvable");
							gMessageWindow.setMessage("Board is unsolvable");
							//Create pop up window ????
							if (gMessageWindow.getWindow() == NULL && gMessageWindow.getRenderer() == NULL) {
								if (gMessageWindow.init() == false) {
									printf("Failed to initialize error window\n");
								}
								else {
									SDL_RaiseWindow(gMessageWindow.getWindow());
								}
							}
							printf("Board is unsolvable\n");
						}
					}

					if (newGameButton.isClicked(x, y)) {
						fromGameRunning = true;

						//Go to difficulty menu
						difficultyMenu = true;
						gameRunning = false;

						//Reset button positions
						easyButton.setButtonPostion(SCREEN_WIDTH / 2 - 100 / 2, SCREEN_HEIGHT / 2 - 125);
						mediumButton.setButtonPostion(SCREEN_WIDTH / 2 - 100 / 2, SCREEN_HEIGHT / 2 - 50);
						hardButton.setButtonPostion(SCREEN_WIDTH / 2 - 100 / 2, SCREEN_HEIGHT / 2 + 25);
						backButton.setButtonPostion(SCREEN_WIDTH / 2 - 100 / 2, SCREEN_HEIGHT / 2 + 100);

						//Reset button sizes
						easyButton.setButtonSize(100, 50);
						mediumButton.setButtonSize(100, 50);
						hardButton.setButtonSize(100, 50);
						backButton.setButtonSize(100, 50);
					}

					if (resetBoardButton.isClicked(x, y)) {
						//Reset all editable cells to 0
						sudokuBoard.gSolver.reset(sudokuBoard.gCells);
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
					//Reset button positions
					startButton.setButtonPostion(SCREEN_WIDTH / 2 - 150 / 2, SCREEN_HEIGHT / 2 - 25);
					continueButton.setButtonPostion(SCREEN_WIDTH / 2 - 150 / 2, SCREEN_HEIGHT / 2 - 100);
					loadButton.setButtonPostion(SCREEN_WIDTH / 2 - 150 / 2, SCREEN_HEIGHT / 2 + 50);

					//Reset button sizes
					startButton.setButtonSize(150, 50);
					continueButton.setButtonSize(150, 50);
					loadButton.setButtonSize(150, 50);

					continueButton.setClickAble(true);
					break;
				default:
					break;
				}
			}
		}
	}

	//Process events for save menu
	if (saveMenu == true) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_TEXTINPUT) { //Get text input, must be done before SDL_KEYDOWN ???
				gTextInputBuffer += e.text.text;
			}
			else if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_BACKSPACE && gTextInputBuffer.length() > 0) {
					gTextInputBuffer.pop_back(); //Delete a character from the string
				}
				else if (e.key.keysym.sym == SDLK_ESCAPE) {
					saveMenu = false;
					gameRunning = true;

					//Reset button positions
					saveButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 25);
					loadButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 100);
					checkBoardButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 175);

					//Reset button sizes
					saveButton.setButtonSize(100, 50);
					loadButton.setButtonSize(100, 50);
					checkBoardButton.setButtonSize(100, 50);

					//Reset text input
					gTextInputBuffer = "";

					SDL_StopTextInput();
				}
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				if (saveButton.isClicked(x, y)) {
					printf("Do save file stuff\n");
					sudokuBoard.saveLevel(gTextInputBuffer);
				}
				else if (backButton.isClicked(x, y)) {
					saveMenu = false;
					gameRunning = true;

					//Reset button positions
					saveButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 25);
					loadButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 100);
					checkBoardButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 175);
				
					//Reset button sizes
					saveButton.setButtonSize(100, 50);
					loadButton.setButtonSize(100, 50);
					checkBoardButton.setButtonSize(100, 50);

					//Reset text input
					gTextInputBuffer = "";

					SDL_StopTextInput();
				}
			}	
		}	
	}

	//Process event for the load menu
	if (loadMenu == true) {
		while (SDL_PollEvent(&e) > 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				if (backButton.isClicked(x, y)) {
					if (continueButton.getClickAble() == false || fromMainMenu == true) {
						loadMenu = false;
						main_menu = true;
						fromMainMenu = false;

						//Reset button positions
						startButton.setButtonPostion(SCREEN_WIDTH / 2 - 150 / 2, SCREEN_HEIGHT / 2 - 25);
						continueButton.setButtonPostion(SCREEN_WIDTH / 2 - 150 / 2, SCREEN_HEIGHT / 2 - 100);
						loadButton.setButtonPostion(SCREEN_WIDTH / 2 - 150 / 2, SCREEN_HEIGHT / 2 + 50);

						//Reset button sizes
						startButton.setButtonSize(150, 50);
						continueButton.setButtonSize(150, 50);
						loadButton.setButtonSize(150, 50);

						//Reset head and tail
						warpHead = 0;
						warpTail = 0;

						//Pop all content in the fileNames vector
						sudokuBoard.levelsManager.depopulateFileName();
					}
					else { //Traffic comming from the gameRunning state
						loadMenu = false;
						gameRunning = true;

						//Reset button positions
						saveButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 25);
						loadButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 100);
						checkBoardButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 175);

						//Reset button sizes
						saveButton.setButtonSize(100, 50);
						loadButton.setButtonSize(100, 50);
						checkBoardButton.setButtonSize(100, 50);

						//Reset head and tail
						warpHead = 0;
						warpTail = 0;

						//Pop all content in the fileNames vector
						sudokuBoard.levelsManager.depopulateFileName();
					}
				}
				else if (loadButton.isClicked(x, y)) {
					//Load the save file
					if (gChosenFileName != "") {
						sudokuBoard.levelsManager.loadUserSavedLevel(sudokuBoard.gCells, gChosenFileName);

						loadMenu = false;
						gameRunning = true;

						//Reset button positions
						saveButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 25);
						loadButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 100);
						checkBoardButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 175);
					
						//Reset button sizes
						saveButton.setButtonSize(100, 50);
						loadButton.setButtonSize(100, 50);
						checkBoardButton.setButtonSize(100, 50);

						//Reset head and tail
						warpHead = 0;
						warpTail = 0;

						//Pop all content in the fileNames vector
						sudokuBoard.levelsManager.depopulateFileName();
					}					
				}
				else {
					//Check mouse position to see if it is within any line of text
					int counter = 0;
					for (size_t i = warpHead; i < warpTail; i++) {
						if ((x >= displayFileName[counter].x) && (x <= displayFileName[counter].x + displayFileName[counter].w)
							&& (y >= displayFileName[counter].y) && (y <= displayFileName[counter].y + displayFileName[counter].h)) {
							//Get the file name
							gChosenFileName = fileNames[i];
							std::cout << gChosenFileName << std::endl;
							gChosenLine = i;
						}
						counter++;
					}
				}
			}
			else if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					if (continueButton.getClickAble() == false || fromMainMenu == true) {
						loadMenu = false;
						main_menu = true;
						fromMainMenu = false;

						//Reset button positions
						startButton.setButtonPostion(SCREEN_WIDTH / 2 - 150 / 2, SCREEN_HEIGHT / 2 - 25);
						continueButton.setButtonPostion(SCREEN_WIDTH / 2 - 150 / 2, SCREEN_HEIGHT / 2 - 100);
						loadButton.setButtonPostion(SCREEN_WIDTH / 2 - 150 / 2, SCREEN_HEIGHT / 2 + 50);

						//Reset button sizes
						startButton.setButtonSize(150, 50);
						continueButton.setButtonSize(150, 50);
						loadButton.setButtonSize(150, 50);

						//Reset head and tail
						warpHead = 0;
						warpTail = 0;

						//Pop all content in the fileNames vector
						sudokuBoard.levelsManager.depopulateFileName();
					}
					else {
						loadMenu = false;
						gameRunning = true;

						//Reset button positions
						saveButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 25);
						loadButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 100);
						checkBoardButton.setButtonPostion(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 175);

						//Reset button sizes
						saveButton.setButtonSize(100, 50);
						loadButton.setButtonSize(100, 50);
						checkBoardButton.setButtonSize(100, 50);

						//Reset head and tail
						warpHead = 0;
						warpTail = 0;

						//Pop all content in the fileNames vector
						sudokuBoard.levelsManager.depopulateFileName();
					}		
				}

				if (e.key.keysym.sym == SDLK_DOWN) {
					//Scroll the file names down
					if (warpHead < fileNames.size() - 5) {
						warpHead++;
						warpTail++;
					}
				}

				if (e.key.keysym.sym == SDLK_UP) {
					//Scroll the file names up
					if (warpHead > 0) {
						warpHead--;
						warpTail--;
					}
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
	/*loadButton.setButton(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 100,
					100, 50, buttonColor, "Load", gRenderer, 18);*/

	loadButton.setButton(SCREEN_WIDTH / 2 - 150 / 2, SCREEN_HEIGHT / 2 + 50, 150, 50, buttonColor, "Load", gRenderer, 18);

	//Make the check button
	checkBoardButton.setButton(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 175,
							100, 50, buttonColor, "Check", gRenderer, 18);
	
	//Make the sovle button
	solveBoardButton.setButton(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 250,
									100, 50, buttonColor, "Solve", gRenderer, 18);

	//Make the new game button
	newGameButton.setButton(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 325,
											100, 50, buttonColor, "New Game", gRenderer, 18);

	//Make the reset board button
	resetBoardButton.setButton(sudokuBoard.getCellSize() * 9 + ((SCREEN_WIDTH - sudokuBoard.getCellSize() * 9) / 2) - 100 / 2, 400,
														100, 50, buttonColor, "Reset", gRenderer, 18);
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

void Game::renderFileName() {
	//To be implemented
	SDL_Color textColor = { 0, 0, 0, 0 };
	SDL_Rect renderQuad = { 0, 0, 0, 0 };
	int counter = 0;
	for (size_t i = warpHead; i < warpTail; i++) { //Issue regarding out of bound in vector
		SDL_Surface *textSurface = TTF_RenderText_Blended(gFont, fileNames[i].c_str(), textColor);

		if (textSurface == NULL) {
			printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
			return;
		}

		SDL_Texture *textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);

		if (textTexture == NULL) {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
			return;
		}

		//Font size 24 has height of 27
		renderQuad.x = displayFileName[counter].x;
		renderQuad.y = displayFileName[counter].y;
		renderQuad.w = textSurface->w;
		renderQuad.h = textSurface->h;

		//Fill the background of the chosen text
		if (i == gChosenLine) {
			SDL_SetRenderDrawColor(gRenderer, 160, 160, 160, 1);
			SDL_RenderFillRect(gRenderer, &displayFileName[counter]); // Issue here, need to fix 
		}

		SDL_RenderCopy(gRenderer, textTexture, NULL, &renderQuad);

		//Destroy old surface and texture
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);
		counter++;
	}
}

void Game::renderTextInput() {
	SDL_Color textColor = { 0, 0, 0, 0 };
	SDL_Surface* textSurface = TTF_RenderText_Blended(gFont, gTextInputBuffer.c_str(), textColor);

	if (textSurface == NULL) {
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		return;
	}

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);

	if (textTexture == NULL) {
		printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		return;
	}

	//Font size 24 has height of 27

	textRect.w = textSurface->w;
	textRect.h = textSurface->h;

	SDL_RenderCopy(gRenderer, textTexture, NULL, &textRect);

	//Destroy old surface and texture
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}

//Note: Before rendering button make sure to set its position back to the original position
void Game::render() {
	//Clear screen
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderClear(gRenderer);

	if (main_menu == true) {
		startButton.renderButton(gRenderer);

		if (continueButton.getClickAble() == true) {
			continueButton.renderButton(gRenderer);
		}

		loadButton.renderButton(gRenderer);
	}

	if (difficultyMenu == true) {
		easyButton.renderButton(gRenderer);
		mediumButton.renderButton(gRenderer);
		hardButton.renderButton(gRenderer);
		backButton.renderButton(gRenderer);
	}

	if (gameRunning == true) {
		//Render for the other window
		if (gMessageWindow.getWindow() != NULL) {
			gMessageWindow.render();
		}

		//Draw board
		sudokuBoard.renderBoard(gRenderer);

		//Draw save button
		saveButton.renderButton(gRenderer);

		//Draw load button
		loadButton.renderButton(gRenderer);

		//Draw check button
		checkBoardButton.renderButton(gRenderer);

		//Draw solve button
		solveBoardButton.renderButton(gRenderer);

		//Draw new game button
		newGameButton.renderButton(gRenderer);

		//Draw reset board button
		resetBoardButton.renderButton(gRenderer);
	}

	if (saveMenu == true) {
		//Render the text
		if (gTextInputBuffer.size() != 0) {
			renderTextInput();
		}
		//Draw the  text box
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(gRenderer, &textInputRect);

		//Render the button
		saveButton.renderButton(gRenderer);
		backButton.renderButton(gRenderer);
	}

	if (loadMenu == true) {
		//Render the files name

		if (fileNames.size() != 0) {
			renderFileName();
		}

		//Render the button
		loadButton.renderButton(gRenderer);
		backButton.renderButton(gRenderer);
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

		SDL_Delay(1000 / 60);
	}
	//Free resources and close SDL
	close();
}