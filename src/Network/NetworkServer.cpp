#include "Network/NetworkServer.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#include <iostream>

#include "Network/PacketFactory.hpp"
#include "Network/PacketHandler.hpp"

static uint8_t g_NextPlayerID = 0;

bool NetworkServer::start(int port) {
  serverSocket = SDLNet_UDP_Open(port);
  if (serverSocket == nullptr) return false;

  packet = SDLNet_AllocPacket(512);
  if (packet == nullptr) return false;

  return true;
}

void NetworkServer::stop() {
  if (packet != nullptr) SDLNet_FreePacket(packet);
  if (serverSocket != nullptr) SDLNet_UDP_Close(serverSocket);
}

void NetworkServer::handleNewConnection(uint32_t clientKey, IPaddress& address,
                                        uint32_t now) {
  std::cout << "Nouveau joueur connecte (Key: " << clientKey << ")"
            << std::endl;

  players[clientKey] = ServerPlayer();
  players[clientKey].id = g_NextPlayerID++;
  players[clientKey].address = address;
  players[clientKey].lastSeenTime = now;

  bool exists = false;
  for (const auto& clientAddr : clients) {
    if (clientAddr.host == address.host && clientAddr.port == address.port)
      exists = true;
  }
  if (!exists) clients.push_back(address);
}

void NetworkServer::sendWelcomePacket(IPaddress& address, uint8_t assignedId) {
  UDPpacket* welcomePkt = SDLNet_AllocPacket(64);
  if (!welcomePkt) return;

  WelcomePacket* wp = (WelcomePacket*)welcomePkt->data;
  wp->header.type = PACKET_TYPE_WELCOME;
  wp->header.sequence = 0;
  wp->assignedId = assignedId;

  welcomePkt->len = sizeof(WelcomePacket);
  welcomePkt->address = address;

  SDLNet_UDP_Send(serverSocket, -1, welcomePkt);
  SDLNet_FreePacket(welcomePkt);
}

void NetworkServer::sendSeedPacket(IPaddress& address, uint32_t seed) {
  UDPpacket* seedPkt = SDLNet_AllocPacket(64);
  if (!seedPkt) return;

  SeedPacket* sp = (SeedPacket*)seedPkt->data;
  sp->header.type = PACKET_TYPE_SEED;
  sp->header.sequence = 0;
  sp->seed = seed;

  seedPkt->len = sizeof(SeedPacket);
  seedPkt->address = address;

  SDLNet_UDP_Send(serverSocket, -1, seedPkt);
  SDLNet_FreePacket(seedPkt);
}

int NetworkServer::handleIncomingData() {
  int received = SDLNet_UDP_Recv(serverSocket, packet);

  if (received) {
    uint32_t clientKey = packet->address.host + packet->address.port;
    PacketHeader* header = (PacketHeader*)packet->data;
    uint32_t now = SDL_GetTicks();

    if (players.find(clientKey) == players.end()) {
      handleNewConnection(clientKey, packet->address, now);
    }

    ServerPlayer& currentPlayer = players[clientKey];

    currentPlayer.lastSeenTime = now;

    if (header->type == PACKET_TYPE_INPUT) {
      InputPacket* input = (InputPacket*)packet->data;
      if (input->playerId == 255) {
        std::cout << "Nouveau joueur, envoi welcome ID="
                  << (int)currentPlayer.id << std::endl;
        sendWelcomePacket(packet->address, currentPlayer.id);
        // Send seed packet after welcome
        extern uint32_t g_mazeSeed;
        std::cout << "Envoi seed=" << g_mazeSeed
                  << " au joueur ID=" << (int)currentPlayer.id << std::endl;
        sendSeedPacket(packet->address, g_mazeSeed);
      }
    }

    PacketHandler::processServerPacket(packet->data, packet->len,
                                       currentPlayer);
  }
  return received;
}

bool NetworkServer::sendData(void* data, int size) {
  for (const auto& clientAddr : clients) {
    UDPpacket* sendPacket = SDLNet_AllocPacket(512);
    if (!sendPacket) return false;

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

void NetworkServer::checkDisconnects(uint32_t currentTime, uint32_t timeoutMs) {
  auto it = players.begin();
  while (it != players.end()) {
    if (currentTime > it->second.lastSeenTime + timeoutMs) {
      std::cout << "Deconnexion (Timeout) : " << it->first << std::endl;

      IPaddress deadIP = it->second.address;
      for (auto vIt = clients.begin(); vIt != clients.end();) {
        if (vIt->host == deadIP.host && vIt->port == deadIP.port) {
          vIt = clients.erase(vIt);
        } else {
          ++vIt;
        }
      }
      it = players.erase(it);
    } else {
      ++it;
    }
  }
}

std::map<uint32_t, ServerPlayer>& NetworkServer::getPlayersMutable() {
  return players;
}

const std::map<uint32_t, ServerPlayer>& NetworkServer::getPlayers() {
  return players;
}