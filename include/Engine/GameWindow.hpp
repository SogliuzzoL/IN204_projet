#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE  "D00M"

bool init_sdl(SDL_Window **window, SDL_Renderer **renderer);
void handle_events(bool *running);
void render(SDL_Renderer *renderer);
void cleanup(SDL_Window *window, SDL_Renderer *renderer);

int sdl_test ();