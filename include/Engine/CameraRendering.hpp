#pragma once
#include <vector>

#include "GameWindow.hpp"
#include "Game_logic.hpp"

class NetworkClient;

void rendering_settings();

void draw_cube();  // Utile si vous voulez l'utiliser ailleurs

void rendering_loop(SDL_GLContext ctx, Uint32 *frames, SDL_Window *window,
                    player t, std::vector<wall> walls, NetworkClient *client);