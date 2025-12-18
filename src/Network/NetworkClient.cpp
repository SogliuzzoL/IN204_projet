#include "Network/NetworkClient.hpp"

#include <SDL2/SDL_net.h>

bool NetworkClient::start(const char* host, int port) {
  IPaddress ip;
  if (SDLNet_ResolveHost(&ip, host, port) < 0) {
    return false;
  }
  socket = SDLNet_TCP_Open(&ip);
  return true;
}

void NetworkClient::stop() { SDLNet_TCP_Close(socket); }