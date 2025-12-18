#pragma once
#include <SDL2/SDL_net.h>

#include <string>
#include <vector>

#include "Network/Network.hpp"

class NetworkServer : public Network {
 private:
  TCPsocket serverSocket;
  SDLNet_SocketSet socketSet;
  std::vector<TCPsocket> clients;

 public:
  bool start(const char* host, int port) override;
  void stop() override;
};