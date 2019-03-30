#ifndef __SDL_WRAPPER_H__
#define __SDL_WRAPPER_H__

#include <SDL2/SDL.h>
#include <GLES3/gl3.h>
#include <string>

class SDLWrapper {
private:
	int width;
	int height;
	SDL_Window *window;

public:
	SDL_GLContext gl_ctx;
	
	SDLWrapper(std::string title, int width, int height);
	~SDLWrapper();

	auto Clear(float r, float g, float b, float a) -> void;
	auto UpdateSurface() -> void;
};

#endif