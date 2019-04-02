#include "game.h"

#include <chrono>
#include <iostream>

DriveGame::DriveGame() : mesh(100, 100)
{
	this->sdl = std::make_shared<SDLWrapper>("Game title", 800, 600);
	this->gl = std::make_shared<GLWrapper>();
	this->running = true;

	this->gl->SetViewport(0, 0, 80, 60);

	const float verts[8] = {
		-0.5, -0.5,
		 0.5, -0.5,
		 0.5,  0.5,
		-0.5,  0.5,
	};

	const int inds[6] = {
		0, 1, 2, 0, 2, 3
	};

	mesh.Init();
	mesh.SetVerticies(verts, 8);
	mesh.SetIndicies(inds, 6);
	mesh.Scale(16, 14);
	mesh.Rotate(3.14159265 / 4);
	mesh.Buffer();
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
		}
		break;
	}
}

auto DriveGame::Update(float dt) -> void
{
	mesh.Translate(0.1, 0.1);
	mesh.Rotate(3.14159 * dt);
}

auto DriveGame::Draw() -> void
{
	sdl->Clear(0, 0, 0, 1);

	mesh.Render();

	sdl->UpdateSurface();
}
