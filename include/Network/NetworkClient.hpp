#pragma once

#include <SDL2/SDL_net.h>

#include "Network/Network.hpp"

class NetworkClient : public Network {
 private:
  TCPsocket socket;

 public:
  /**
   * Start the network client.
   * @param host The host address to connect to.
   * @param port The port number to connect to.
   * @return true if the client started successfully, false otherwise.
   */
  bool start(const char* host, int port) override;

  /**
   * Stop the network client.
   */
  void stop() override;
};