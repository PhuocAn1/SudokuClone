#pragma once
class Cell
{
public:
	Cell();
	~Cell();
	bool isEditable();
	int getCellValue();
	void setCellPosition(int x, int y);
	void setCellValue(int value);
	void setEditable(bool editable);

private:
	int x;
	int y;
	bool editable;
	int cellValue;
};

