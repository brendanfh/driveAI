#ifndef __GAME_H__
#define __GAME_H__

#include <memory>
#include "./gfx/sdl_wrapper.h"

class DriveGame {
private:
	std::shared_ptr<SDLWrapper> sdl;

public:
	DriveGame();
	~DriveGame();

	auto Update(float dt) -> void;
	auto Draw() -> void;
};

#endif