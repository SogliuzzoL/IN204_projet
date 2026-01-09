#pragma once
#include <SDL2/SDL_net.h>

#include <string>
#include <vector>

#include "Network/Network.hpp"

class NetworkServer : public Network {
 private:
  UDPsocket serverSocket;
  UDPpacket* packet;
  std::vector<IPaddress> clients;

 public:
  /**
   * Start the network server.
   * @param port The port number to bind the server to.
   * @return true if the server started successfully, false otherwise.
   */
  bool start(int port);

  /**
   * Stop the network server.
   */
  void stop() override;

  /**
   * Handle incoming data from clients.
   * @return 1 if a new packet is available, or -1 on error. 0 means no packets
   * were currently available.
   */
  int handleIncomingData() override;

  /**
   * Send a message to all connected clients.
   * @param message The message to send.
   * @return true if the message was sent successfully, false otherwise.
   */
  bool sendData(void* data, int size) override;
};