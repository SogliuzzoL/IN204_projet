#include "Network/NetworkClient.hpp"

#include <SDL2/SDL_net.h>

#include <iostream>

#include "Network/PacketHandler.hpp"

bool NetworkClient::connect(const char* host, int port) {
  // Ouvrir la socket sur un port aléatoire
  clientSocket = SDLNet_UDP_Open(0);
  if (!clientSocket) return false;

  // Résoudre l'adresse du serveur
  if (SDLNet_ResolveHost(&serverIP, host, port) == -1) return false;

  // Allouer un paquet pour l'envoi
  packet = SDLNet_AllocPacket(512);
  return packet != nullptr;
}

int NetworkClient::handleIncomingData() {
  int received = SDLNet_UDP_Recv(clientSocket, packet);
  // Vérifier les paquets entrants
  if (received) {
    // Traiter le paquet reçu
    PacketHandler::processPacket(packet->data, packet->len);
  }
  return received;
}

bool NetworkClient::sendData(void* data, int size) {
  // Préparation du paquet vers le serveur
  packet->address = serverIP;
  packet->len = size;
  memcpy(packet->data, data, size);
  if (SDLNet_UDP_Send(clientSocket, -1, packet) == 0) return false;
  return true;
}

void NetworkClient::stop() {
  if (packet) SDLNet_FreePacket(packet);
  if (clientSocket) SDLNet_UDP_Close(clientSocket);
}