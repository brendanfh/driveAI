/* Made by Brendan Hansen */

#ifndef __GAME_H__
#define __GAME_H__

#include <memory>
#include <unistd.h>
#include "./gfx/sdl_wrapper.h"
#include "./input/keyboard.h"
#include "./world/car.h"
#include "./world/track.h"

class DriveGame {
private:
	std::shared_ptr<SDLWrapper> sdl;
	std::shared_ptr<GLWrapper> gl;

	std::shared_ptr<Car> car;
	std::shared_ptr<Track> track;

	std::shared_ptr<KeyboardManager> keyboard;

	bool running;

public:
	DriveGame(std::shared_ptr<SDLWrapper> sdl, std::shared_ptr<GLWrapper> gl);
	~DriveGame();

	auto Run() -> void;
	auto HandleEvent(SDL_Event *ev) -> void;
	auto Update(float dt) -> void;
	auto Draw() -> void;

	auto SetInputs(std::vector<float>& inputs) -> void;

	auto GetCar() -> std::shared_ptr<Car>;
	auto GetTrack() -> std::shared_ptr<Track>;
};

#endif
