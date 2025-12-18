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
  /**
   * Start the network server.
   * @param host The host address to bind.
   * @param port The port number to bind.
   * @return true if the server started successfully, false otherwise.
   */
  bool start(const char* host, int port) override;

  /**
   * Stop the network server.
   */
  void stop() override;
};