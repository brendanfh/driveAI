#ifndef __GAME_H__
#define __GAME_H__

#include <memory>
#include <unistd.h>
#include "./gfx/sdl_wrapper.h"

class DriveGame {
private:
	std::shared_ptr<SDLWrapper> sdl;
	std::shared_ptr<GLWrapper> gl;
	GLMesh mesh;

	bool running;

public:
	DriveGame();
	~DriveGame();

	auto Run() -> void;
	auto HandleEvent(SDL_Event *ev) -> void;
	auto Update(float dt) -> void;
	auto Draw() -> void;
};

#endif