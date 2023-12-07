#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

class PopUpWindow
{
//I didnt khnow SDL have simple pop up message, so i made my own :(((
public:
	PopUpWindow();
	~PopUpWindow();
	bool init();
	void close();
	void render();
	void setMessage(std::string message);
	void setWindowTitle(std::string title);
	void renderPopUpWindow();
	SDL_Window* getWindow();
	SDL_Renderer* getRenderer();
private:
	std::string gWindowTitle, gMessage;
	SDL_Window *gWindow;
	SDL_Renderer *gRenderer;
	TTF_Font *gFont;

	const int WINDOW_WIDTH = 300;
	const int WINDOW_HEIGHT = 250;
};

