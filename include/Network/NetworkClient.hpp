#pragma once

#include <SDL2/SDL_net.h>

#include <string>

#include "Network/Network.hpp"

/**
 * Network client class handling client-side networking.
 */
class NetworkClient : public Network {
 private:
  UDPsocket clientSocket;
  UDPpacket* packet;
  IPaddress serverIP;

 public:
  /** Connects to the specified server.
   * @param host The server hostname or IP address.
   * @param port The server port number.
   * @return true if the connection was successful, false otherwise.
   */
  bool connect(const char* host, int port);

  /** Handles incoming data from the server.
   * @return 1 if a new packet is available, or -1 on error. 0 means no packets
   * were currently available.
   */
  int handleIncomingData() override;

  /** Sends data to the server.
   * @param data Pointer to the data to send.
   * @param size Size of the data in bytes.
   * @return true if the data was sent successfully, false otherwise.
   */
  bool sendData(void* data, int size) override;

  /** Stops the client and cleans up resources. */
  void stop() override;

  /** Gets the UDPpacket used by the client.
   * @return Pointer to the UDPpacket.
   */
  UDPpacket* getPacket() const { return packet; }
};