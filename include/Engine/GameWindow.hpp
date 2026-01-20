#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_opengl.h>


#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define CAM_SPEED 0.05
#define WINDOW_TITLE  "D00M"

typedef struct cam_pos{
    float x,y,z;
}cam_pos;

bool init_sdl_gl(SDL_Window **window, SDL_GLContext *gl_ctx);
void handle_events(bool *running, cam_pos position);
void render_scene(void);
void cleanup(SDL_Window *window, SDL_GLContext gl_ctx);

int sdl_test ();