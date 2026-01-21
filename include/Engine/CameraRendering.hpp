#pragma once
#include "GameWindow.hpp"

void Render(triangle t, int w, int h);

void rendering_settings();

void rendering_loop(SDL_GLContext ctx, Uint32 *frames, SDL_Window *window,
                    triangle t);
