#include "SudokuSolver.h"

SudokuSolver::SudokuSolver() {
}

SudokuSolver::~SudokuSolver() {
}

bool SudokuSolver::isNumberInRow(Cell gCells[9][9], int number, int row) {
	for (int i = 0; i < GRID_SIZE; i++) {
		if (gCells[row][i].getCellValue() == number) {
			return true;
		}
	}
	return false;
}

bool SudokuSolver::isNumberInCol(Cell gCells[9][9], int number, int row) {
	for (int i = 0; i < GRID_SIZE; i++) {
		if (gCells[i][row].getCellValue() == number) {
			return true;
		}
	}
	return false;
}

bool SudokuSolver::isNumberInBox(Cell gCells[9][9], int number, int row, int col) {
	int localBoxRow = row - row % 3;
	int localBoxCol = col - col % 3;

	for (int i = localBoxRow; i < localBoxRow + 3; i++) {
		for (int j = localBoxCol; j < localBoxCol + 3; j++) {
			if (gCells[i][j].getCellValue() == number) {
				return true;
			}
		}
	}

	return false;
}

bool SudokuSolver::isValidPlacement(Cell gCells[9][9], int number, int row, int col) {
	return !isNumberInRow(gCells, number, row) 
		&& !isNumberInCol(gCells, number, col) 
		&& !isNumberInBox(gCells, number, row, col);
}

void SudokuSolver::reset(Cell gCell[9][9]) {
	//Reset all editable cells to 0
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (gCell[i][j].isEditable() == true) {
				gCell[i][j].setCellValue(0);
			}
		}
	}
}

bool SudokuSolver::solveBoard(Cell gCell[9][9]) {
	for (int row = 0; row < GRID_SIZE; row++) {
		for (int col = 0; col < GRID_SIZE; col++) {
			if (gCell[row][col].getCellValue() == 0) {
				for (int numberToTry = 1; numberToTry <= GRID_SIZE; numberToTry++) {
					if (isValidPlacement(gCell, numberToTry, row, col)) {
						gCell[row][col].setCellValue(numberToTry);
						if (solveBoard(gCell)) {
							return true;
						}
						else {
							gCell[row][col].setCellValue(0);
						}
					}
				}
				return false; //Mean that we tried all numbers for this cell and none of them worked, the board is not solvable
			}
		}
	}
	return true;
}