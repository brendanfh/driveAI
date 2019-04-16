/* Made by Brendan Hansen */

#include "sdl_wrapper.h"

SDLWrapper::SDLWrapper(std::string title, int width, int height)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	this->window = SDL_CreateWindow(title.c_str(),
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (this->window == NULL) {
		throw "Window not initialized correctly!";
	}

	this->gl_ctx = SDL_GL_CreateContext(this->window);

	SDL_GL_SetSwapInterval(1);
}

SDLWrapper::~SDLWrapper()
{
	SDL_GL_DeleteContext(this->gl_ctx);
	SDL_DestroyWindow(this->window);
	this->window = NULL;

	SDL_Quit();
}

auto SDLWrapper::Clear(float r, float g, float b, float a) const -> void
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

auto SDLWrapper::UpdateSurface() -> void
{
	SDL_GL_SwapWindow(this->window);
}

auto SDLWrapper::PollEvents(SDL_Event *event) -> bool
{
	return SDL_PollEvent(event) > 0;	
}
