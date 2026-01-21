#pragma once
#include "GameWindow.hpp"

void Render(player t, int w, int h);

void rendering_settings();

void rendering_loop(SDL_GLContext ctx, Uint32 *frames, SDL_Window *window,
                    player t);
