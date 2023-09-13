#include "Cell.h"

Cell::Cell() {
	editable = true;
	cellValue = 0;
	x = 0;
	y = 0;
}

Cell::~Cell() {

}

bool Cell::isEditable() {
	return editable;
}

int Cell::getCellValue() {
	return cellValue;
}

void Cell::setCellPosition(int x, int y) {
	this->x = x;
	this->y = y;
}

void Cell::setCellValue(int value) {
	cellValue = value;
}

void Cell::setEditable(bool editable) {
	this->editable = editable;
}