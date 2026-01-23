#include "Network/NetworkServer.hpp"

#include <SDL2/SDL_net.h>

#include <iostream>

#include "Network/PacketHandler.hpp"

bool NetworkServer::start(int port) {
  // Open the server socket
  serverSocket = SDLNet_UDP_Open(port);
  if (serverSocket == nullptr) return false;

  // Allocate the packet
  packet = SDLNet_AllocPacket(512);
  if (packet == nullptr) return false;

  return true;
}

void NetworkServer::stop() {
  // Free the packet
  if (packet != nullptr) SDLNet_FreePacket(packet);

  // Close the server socket
  if (serverSocket != nullptr) SDLNet_UDP_Close(serverSocket);
}

int NetworkServer::handleIncomingData() {
  int received = SDLNet_UDP_Recv(serverSocket, packet);
  // Check for incoming packets
  if (received) {
    // Get sender's IP address and port
    const char* senderIP = SDLNet_ResolveIP(&packet->address);
    Uint16 port = SDLNet_Read16(&packet->address.port);

    // Process the received packet
    PacketHandler::processPacket(packet->data, packet->len);

    // Add new client to the list if not already present
    bool clientExists = false;
    for (const auto& clientAddr : clients) {
      if (clientAddr.host == packet->address.host &&
          clientAddr.port == packet->address.port) {
        clientExists = true;
        break;
      }
    }
    if (!clientExists) {
      clients.push_back(packet->address);
    }
  }
  return received;
}

bool NetworkServer::sendData(void* data, int size) {
  // Send the message to all connected clients
  for (const auto& clientAddr : clients) {
    UDPpacket* sendPacket = SDLNet_AllocPacket(512);
    if (sendPacket == nullptr) return false;

    sendPacket->address = clientAddr;
    sendPacket->len = size;
    memcpy(sendPacket->data, data, size);

    if (SDLNet_UDP_Send(serverSocket, -1, sendPacket) == 0) {
      SDLNet_FreePacket(sendPacket);
      return false;
    }

    SDLNet_FreePacket(sendPacket);
  }
  return true;
}