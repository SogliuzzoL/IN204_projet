#pragma once

#include <SDL2/SDL_net.h>

#include <string>

#include "Network/Network.hpp"

class NetworkClient : public Network {
 private:
  UDPsocket clientSocket;
  UDPpacket* packet;
  IPaddress serverIP;

 public:
  /**
   * Start the network client.
   * @param host The host address to connect to.
   * @param port The port number to connect to.
   * @return true if the client started successfully, false otherwise.
   */
  bool connect(const char* host, int port);

  /**
   * Handle incoming data from the server.
   * @return 1 if a new packet is available, or -1 on error. 0 means no packets
   * were currently available.
   */
  int handleIncomingData() override;

  /**
   * Send data to the server.
   * @param message The message to send.
   * @return true if the message was sent successfully, false otherwise.
   */
  bool sendData(const std::string& message) override;

  /**
   * Stop the network client.
   */
  void stop() override;
};