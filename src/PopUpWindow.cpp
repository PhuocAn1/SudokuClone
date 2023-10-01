#include "PopUpWindow.h"

PopUpWindow::PopUpWindow() {
	gWindow = NULL;
	gRenderer = NULL;
	gFont = NULL;
}

PopUpWindow::~PopUpWindow() {
	close();
}

bool PopUpWindow::init() {
	//Create window
	gWindow = SDL_CreateWindow(gWindowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

	if (gWindow == NULL) {
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	//Create renderer for window
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

	if (gRenderer == NULL) {
		std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}

	//Open the font
	gFont = TTF_OpenFont("res/font/THSpatial.ttf", 24);

	if (gFont == NULL) {
		std::cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
		return false;
	}
}

void PopUpWindow::close() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	TTF_CloseFont(gFont);
	gFont = NULL;
}

void PopUpWindow::setWindowTitle(std::string title) {
	gWindowTitle = title;
}

void PopUpWindow::renderPopUpWindow() {
	SDL_Color textColor = { 0, 0, 0 };

	SDL_Surface *textSurface = TTF_RenderText_Blended(gFont, gMessage.c_str(), textColor);
	
	if (textSurface == NULL) {
		std::cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
	}
	else {
		SDL_Texture *text = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (text == NULL) {
			std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
		}
		else {
			SDL_Rect renderQuad = { WINDOW_WIDTH / 2 - textSurface->w / 2, WINDOW_HEIGHT / 2 - textSurface->h / 2, textSurface->w, textSurface->h };
			SDL_RenderCopy(gRenderer, text, NULL, &renderQuad);
		}
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(text);
	}
}

void PopUpWindow::render() {
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	renderPopUpWindow();

	SDL_RenderPresent(gRenderer);
}

SDL_Window* PopUpWindow::getWindow() {
	return gWindow;
}

SDL_Renderer* PopUpWindow::getRenderer() {
	return gRenderer;
}

void PopUpWindow::setMessage(std::string message) {
	gMessage = message;
}
