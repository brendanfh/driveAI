#ifndef __SDL_WRAPPER_H__
#define __SDL_WRAPPER_H__

#include <SDL2/SDL.h>
#include <GLES3/gl3.h>
#include <string>
#include <memory>

#include "gl_wrapper.h"

class SDLWrapper {
private:
	int width;
	int height;
	SDL_Window *window;

public:
	SDL_GLContext gl_ctx;
	std::shared_ptr<GLWrapper> gl;

	SDLWrapper(std::string title, int width, int height);
	~SDLWrapper();

	auto Clear(float r, float g, float b, float a) const -> void;
	auto UpdateSurface() -> void;

	auto PollEvents(SDL_Event* event) -> bool;
};

#endif