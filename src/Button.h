#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>

class Button
{
public:
	Button();
	~Button();

	void renderButton(SDL_Renderer* gRenderer);
	void setButton(int x, int y, int w, int h, SDL_Color color, std::string displayText, SDL_Renderer* gRenderer, int fontSize);
	bool isClicked(int x, int y);
	void setButtonPostion(int x, int y);
	void setButtonSize(int w, int h);
	void setClickAble(bool clickAble);
	bool getClickAble();

private:
	std::string displayText;
	TTF_Font *textFont;
	SDL_Rect button;
	SDL_Color color, bgColor;//Todo 
	SDL_Texture *textTexture;
	bool clickAble = true;

	int fontSize = 0;
	int textWidth = 0, textHeight = 0;
};

