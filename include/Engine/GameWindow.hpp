#pragma once

#include <SDL2/SDL_keycode.h>
#include <math.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

typedef struct triangle{
    float x,y,angle;
}triangle;

static SDL_GLContext ctx;
static SDL_Window *window;

void quit(int rc);
void Render(triangle t, int w, int h);