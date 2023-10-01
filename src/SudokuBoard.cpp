#include "SudokuBoard.h"

SudokuBoard::SudokuBoard() {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			gCells[i][j].setCellPosition(i * CELL_SIZE, j *CELL_SIZE);
		}
	}

	for (int i = 0; i < 9; i++) {
		numbers[i] = NULL;
	}
}

SudokuBoard::~SudokuBoard() {
	close();
}

void SudokuBoard::loadNumbers(TTF_Font *gFont, SDL_Renderer *gRenderer) {
	SDL_Color textColor = { 0,0,0 };

	SDL_Surface* textSurface = TTF_RenderText_Blended(gFont, "1", textColor);
	numbers[0] = SDL_CreateTextureFromSurface(gRenderer, textSurface);

	textSurface = TTF_RenderText_Blended(gFont, "2", textColor);
	numbers[1] = SDL_CreateTextureFromSurface(gRenderer, textSurface);

	textSurface = TTF_RenderText_Blended(gFont, "3", textColor);
	numbers[2] = SDL_CreateTextureFromSurface(gRenderer, textSurface);

	textSurface = TTF_RenderText_Blended(gFont, "4", textColor);
	numbers[3] = SDL_CreateTextureFromSurface(gRenderer, textSurface);

	textSurface = TTF_RenderText_Blended(gFont, "5", textColor);
	numbers[4] = SDL_CreateTextureFromSurface(gRenderer, textSurface);

	textSurface = TTF_RenderText_Blended(gFont, "6", textColor);
	numbers[5] = SDL_CreateTextureFromSurface(gRenderer, textSurface);

	textSurface = TTF_RenderText_Blended(gFont, "7", textColor);
	numbers[6] = SDL_CreateTextureFromSurface(gRenderer, textSurface);

	textSurface = TTF_RenderText_Blended(gFont, "8", textColor);
	numbers[7] = SDL_CreateTextureFromSurface(gRenderer, textSurface);

	textSurface = TTF_RenderText_Blended(gFont, "9", textColor);
	numbers[8] = SDL_CreateTextureFromSurface(gRenderer, textSurface);

	SDL_FreeSurface(textSurface);
}

void SudokuBoard::renderBoard(SDL_Renderer *gRenderer) {
	//Draw the board
	for (int i = 0; i <= 9; i++) {
		if (i % 3 == 0) {
			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			SDL_RenderDrawLine(gRenderer, 0, i * CELL_SIZE, 450, i * CELL_SIZE);
		}
		else {
			SDL_SetRenderDrawColor(gRenderer, 180, 180, 180, 255);
			SDL_RenderDrawLine(gRenderer, 0, i * CELL_SIZE, 450, i * CELL_SIZE);
		}
		for (int j = 0; j <= 9; j++) {
			if (j % 3 == 0) {
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
				SDL_RenderDrawLine(gRenderer, j * CELL_SIZE, 0, j * CELL_SIZE, 450);
			}
			else {
				SDL_SetRenderDrawColor(gRenderer, 180, 180, 180, 255);
				SDL_RenderDrawLine(gRenderer, j * CELL_SIZE, 0, j * CELL_SIZE, 450);
			}
		}
	}

	//Color the chosen cell

	SDL_SetRenderDrawColor(gRenderer, 150, 0, 255, 125);
	if (currentCellX != -1 && currentCellY != -1) {
		SDL_Rect renderQuad = { currentCellX * CELL_SIZE + 1, currentCellY * CELL_SIZE + 1, CELL_SIZE - 1, CELL_SIZE - 1};
		SDL_RenderFillRect(gRenderer, &renderQuad);
	}

	//Draw the numbers
	
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			/*if (boardState[i][j] != 0) {
				SDL_Rect renderQuad = { CELL_SIZE / 2 - TEXT_WIDTH / 2 + j*50, CELL_SIZE / 2 - TEXT_HEIGHT / 2 + i*50,  TEXT_WIDTH, TEXT_HEIGHT };
				SDL_RenderCopy(gRenderer, numbers[boardState[i][j] - 1], NULL, &renderQuad);
			}*/

			if (gCells[i][j].getCellValue() != 0) {
				SDL_Rect renderQuad = { CELL_SIZE / 2 - TEXT_WIDTH / 2 + j*50, CELL_SIZE / 2 - TEXT_HEIGHT / 2 + i*50,  TEXT_WIDTH, TEXT_HEIGHT };
				SDL_RenderCopy(gRenderer, numbers[gCells[i][j].getCellValue() - 1], NULL, &renderQuad);
			}
		}
	}
}

void SudokuBoard::close() {
	for (int i = 0; i < 9; i++) {
		SDL_DestroyTexture(numbers[i]);
	}
}

int SudokuBoard::getCellSize() {
	return CELL_SIZE;
}

void SudokuBoard::loadLevel() {
	levelsManager.loadLevel(gCells);

	for (int i = 0; i < 9; i++) {
		for (int j=0; j<9; j++) {
			if (gCells[i][j].getCellValue() != 0) {
				gCells[i][j].setEditable(false);
				gCells[i][j].setCellPosition(i * CELL_SIZE, j * CELL_SIZE);
			}
			else {
				gCells[i][j].setEditable(true);
				gCells[i][j].setCellPosition(i * CELL_SIZE, j * CELL_SIZE);
			}
		}
	}
}

void SudokuBoard::chosenCell(int row, int col) {
	if (gCells[row][col].isEditable() == true) {
		this->currentCellX = col;
		this->currentCellY = row;
	}
	printf("Cell %d %d\n", row, col);
}

void SudokuBoard::setNumber(int number) {
	if (this->currentCellY != -1 && this->currentCellX != -1) {
		gCells[currentCellY][currentCellX].setCellValue(number);
	}
}

bool SudokuBoard::checkBoard() {
	bool result = true;
	bool registerNumbers[10] = { false, false, false, false, false, false, false, false, false, false };

	//Check rows and cols to see if there are duplicates
	for (int i = 0; i < 9; i++) {
		//Check rows
		for (int j = 0; j < 9; j++) {
			if (gCells[i][j].getCellValue() == 0) {
				printf("Empty cell in row %d\n", i);
				result = false;
				break;
			}
			else if (gCells[i][j].getCellValue() != 0) {
				if (registerNumbers[gCells[i][j].getCellValue()] == false) {
					registerNumbers[gCells[i][j].getCellValue()] = true;
				}
				else {
					printf("Duplicate number %d in row %d\n", gCells[i][j].getCellValue(), i);
					result = false;
					break;
				}
			}
			
		}

		//Reset register
		for (int j = 0; j < 10; j++) {
			registerNumbers[j] = false;
		}

		//Check cols
		for (int j = 0; j < 9; j++) {
			if (gCells[j][i].getCellValue() == 0) {
				printf("Empty cell in col %d\n", i);
				result = false;
				break;
			} else if (gCells[j][i].getCellValue() != 0) {
				if (registerNumbers[gCells[j][i].getCellValue()] == false) {
					registerNumbers[gCells[j][i].getCellValue()] = true;
				}
				else {
					printf("Duplicate number %d in col %d\n", gCells[j][i].getCellValue(), i);
					result = false;
					break;
				}
			}
			
		}
		//Reset register
		for (int j = 0; j < 10; j++) {
			registerNumbers[j] = false;
		}
	}

	//Check 3x3 squares (Nonet)
	for (int i = 0; i < 9; i += 3) {
		for (int j = 0; j < 9; j += 3) {

			//Check the nonet
			for (int k = i; k < i + 3; k++) {
				for (int l = j; l < j + 3; l++) {
					if (gCells[k][l].getCellValue() != 0) {
						if (registerNumbers[gCells[k][l].getCellValue()] == false) {
							registerNumbers[gCells[k][l].getCellValue()] = true;
						}
						else {
							printf("Duplicate number %d in nonet %d %d\n", gCells[k][l].getCellValue(), i, j);
							result = false;
							break;
						}
					}
				}
			}

			//Reset register
			for (int k = 0; k < 10; k++) {
				registerNumbers[k] = false;
			}
		}
	}
	return result;
}

void SudokuBoard::saveLevel(std::string gFileName) {
	levelsManager.saveLevel(gCells, gFileName);
}

bool SudokuBoard::solveBoard() {
	gSolver.reset(gCells);
	return gSolver.solveBoard(gCells);
}