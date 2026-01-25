#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include <stdlib.h>

extern SDL_GLContext ctx;
extern SDL_Window *window;
SDL_Window *make_window();

void quit(int rc);
int init_sdl();

void timing_info(Uint32 then, Uint32 frames);