#include "Network/Network.hpp"

#include <SDL2/SDL_net.h>

bool Network::initialize() {
  if (SDLNet_Init() == -1) {
    return false;
  }
  return true;
}

void Network::quit() { SDLNet_Quit(); }
