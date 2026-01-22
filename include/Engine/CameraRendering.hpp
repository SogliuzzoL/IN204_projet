#pragma once
#include "GameWindow.hpp"
#include "Game_logic.hpp"
#include <vector>

void rendering_settings();

void rendering_loop(SDL_GLContext ctx, Uint32 *frames, SDL_Window *window,
                    player t, std::vector<wall> walls);
