#pragma once

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <math.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_video.h>

typedef struct triangle {
  float x, y, angle;
} triangle;

static SDL_GLContext ctx;
static SDL_Window *window;

void quit(int rc);
void Render(triangle t, int w, int h);
int init_sdl();
SDL_Window *make_window();
void rendering_settings();

void check_events(int *done, triangle *t);

void rendering_loop(SDL_GLContext ctx, Uint32 *frames, SDL_Window *window, triangle t);

void timing_info(Uint32 then, Uint32 frames);