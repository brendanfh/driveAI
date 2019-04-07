#include "game.h"

#include <chrono>
#include <iostream>

DriveGame::DriveGame()
{
	this->sdl = std::make_shared<SDLWrapper>("Game title", 800, 600);
	this->gl = std::make_shared<GLWrapper>();
	this->car = std::make_shared<Car>(80, 60);
	this->running = true;

	this->gl->SetViewport(0, 0, 160, 120);

	this->keyboard = std::make_shared<KeyboardManager>();
}

DriveGame::~DriveGame()
{

}

auto DriveGame::Run() -> void
{
	auto lastTime = std::chrono::system_clock::now();
	auto now = lastTime;

	SDL_Event event;
	while (this->running) {
		lastTime = now;
		now = std::chrono::system_clock::now();

		while (this->sdl->PollEvents(&event)) {
			this->HandleEvent(&event);
		}

		float delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime).count() / 1000.f;
		this->Update(delta);
		this->Draw();

		usleep(1000);
	}	
}

auto DriveGame::HandleEvent(SDL_Event *ev) -> void
{
	switch (ev->type) {
	case SDL_QUIT:
		this->running = false;
		break;

	case SDL_KEYDOWN:
		switch (ev->key.keysym.sym) {
		case SDLK_ESCAPE:
			this->running = false;
		default:
			this->keyboard->PressKey(ev->key.keysym.sym);
		}

		break;

	case SDL_KEYUP:
		this->keyboard->ReleaseKey(ev->key.keysym.sym);
		break;
	}
}

auto DriveGame::Update(float dt) -> void
{
	car->Update(dt, this->keyboard);
}

auto DriveGame::Draw() -> void
{
	sdl->Clear(0.4, 0.7, 0.9, 1);

	car->Render();

	sdl->UpdateSurface();
}
