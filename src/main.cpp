#include "Game.h"

int main(int argc, char* args[]) {
	Game game;
	if (!game.init()) {
		std::cout << "Failed to initialize!" << std::endl;
	}
	else {
		game.run();
	}
	return 0;
}