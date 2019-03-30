#include "game/game.h"

#include <unistd.h>

int main() {
	DriveGame game;	

	game.Draw();

	sleep(10);

	return 0;
}