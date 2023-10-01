#pragma once
#include "Cell.h"
class SudokuSolver
{
public:
	SudokuSolver();
	~SudokuSolver();
	void reset(Cell gCells[9][9]);
	bool solveBoard(Cell gCells[9][9]);
private:
	bool isNumberInRow(Cell gCells[9][9], int number, int row);
	bool isNumberInCol(Cell gCells[9][9], int number, int col);
	bool isNumberInBox(Cell gCells[9][9], int number, int row, int col);
	bool isValidPlacement(Cell gCells[9][9], int number, int row, int col);
	const int GRID_SIZE = 9;
};

