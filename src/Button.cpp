#include "Button.h"

Button::Button() {
	button = { 0,0,0,0 };
	color = { 0,0,0,0 };
	bgColor = { 0,0,0,0 };
	textTexture = NULL;
	textFont = NULL;
}

Button::~Button() {
	SDL_DestroyTexture(textTexture);
}

void Button::renderButton(SDL_Renderer* gRenderer) {
	SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, color.a);

	//Render text in the middle of the button
	SDL_Rect textRect = {button.x + button.w / 2 - textWidth / 2, button.y + button.h / 2 - textHeight / 2, textWidth, textHeight };
	SDL_RenderCopy(gRenderer, textTexture, NULL, &textRect);

	SDL_RenderDrawRect(gRenderer, &button);
}

void Button::setButton(int x, int y, int w, int h, SDL_Color color, std::string displayText, SDL_Renderer* gRenderer, int fontSize) {
	button = { x,y,w,h };
	this->color = color;
	this->fontSize = fontSize;

	textFont = TTF_OpenFont("res/font/THSpatial.ttf", fontSize);

	if (textFont == NULL) {
		std::cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
	}

	//Load text for button
	SDL_Surface *textSurface = TTF_RenderText_Blended(textFont, displayText.c_str(), color);

	if (textSurface == NULL) {
		std::cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
	}
	else {
		std::cout << "w: " << textSurface->w << " h: " << textSurface->h << std::endl;

		//Get the width and height of the text
		textWidth = textSurface->w;
		textHeight = textSurface->h;

		textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		SDL_FreeSurface(textSurface);
	}
	
}

bool Button::isClicked(int x, int y) {
	if (x >= button.x && x <= button.x + button.w && y >= button.y && y <= button.y + button.h) {
		return true;
	}
	return false;
}

void Button::setClickAble(bool clickAble) {
	this->clickAble = clickAble;
}

bool Button::getClickAble() {
	return clickAble;
}