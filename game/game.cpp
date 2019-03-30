#include "game.h"

DriveGame::DriveGame()
{
	this->sdl = std::make_shared<SDLWrapper>("Game title", 800, 600);
}

DriveGame::~DriveGame()
{

}

auto DriveGame::Update(float dt) -> void
{

}

auto DriveGame::Draw() -> void
{
	sdl->Clear(1, 0, 0, 1);
	sdl->UpdateSurface();
}
