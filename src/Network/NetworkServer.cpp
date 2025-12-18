#include "Network/NetworkServer.hpp"

#include <SDL2/SDL_net.h>

#include <iostream>

bool NetworkServer::start(const char* host, int port) {
  IPaddress ip;
  if (SDLNet_ResolveHost(&ip, host, port) == -1) return false;

  serverSocket = SDLNet_TCP_Open(&ip);
  socketSet = SDLNet_AllocSocketSet(10);
  SDLNet_TCP_AddSocket(socketSet, serverSocket);

  return true;
}

void NetworkServer::stop() { return; }