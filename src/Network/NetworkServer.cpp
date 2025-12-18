#include "Network/NetworkServer.hpp"

#include <SDL2/SDL_net.h>

#include <iostream>

bool NetworkServer::start(const char* host, int port) {
  // Resolve the host and port
  IPaddress ip;
  if (SDLNet_ResolveHost(&ip, host, port) == -1) return false;

  // Open the server socket
  serverSocket = SDLNet_TCP_Open(&ip);
  if (serverSocket == nullptr) return false;

  // Allocate a socket set
  socketSet = SDLNet_AllocSocketSet(10);
  if (socketSet == nullptr) return false;

  // Add the server socket to the socket set
  if (SDLNet_TCP_AddSocket(socketSet, serverSocket) == -1) return false;

  return true;
}

void NetworkServer::stop() {
  // Close all client sockets
  for (auto& client : clients) {
    SDLNet_TCP_Close(client);
  }
  clients.clear();

  // Close the server socket and free the socket set
  if (serverSocket != nullptr) {
    SDLNet_TCP_Close(serverSocket);
    serverSocket = nullptr;
  }

  // Free the socket set
  if (socketSet != nullptr) {
    SDLNet_FreeSocketSet(socketSet);
    socketSet = nullptr;
  }
}

void NetworkServer::handleNewConnections() {
  // Check for ready sockets
  int numReady = SDLNet_CheckSockets(socketSet, 0);

  // If the server socket is ready, accept new connections
  if (numReady > 0) {
    if (SDLNet_SocketReady(serverSocket)) {
      TCPsocket newClient = SDLNet_TCP_Accept(serverSocket);
      // Add the new client to the list and socket set
      if (newClient != nullptr) {
        clients.push_back(newClient);
        SDLNet_TCP_AddSocket(socketSet, newClient);
        std::cout << "New client connected." << std::endl;
      }
    }
  }
}