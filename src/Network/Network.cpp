#include "Network/Network.hpp"

#include <SDL2/SDL_net.h>

bool Network::initialize() {
  // Initialize SDL_net
  if (SDLNet_Init() == -1) return false;
  return true;
}

void Network::quit() {
  // Quit SDL_net
  SDLNet_Quit();
}
