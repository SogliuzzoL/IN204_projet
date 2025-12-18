#include "Network/NetworkClient.hpp"

#include <SDL2/SDL_net.h>

bool NetworkClient::start(const char* host, int port) {
  // Resolve the host and port
  IPaddress ip;
  if (SDLNet_ResolveHost(&ip, host, port) == -1) return false;

  // Open the client socket
  socket = SDLNet_TCP_Open(&ip);
  if (socket == nullptr) return false;

  return true;
}

void NetworkClient::stop() {
  // Close the client socket
  if (socket != nullptr) SDLNet_TCP_Close(socket);
}