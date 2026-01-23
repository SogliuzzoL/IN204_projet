#pragma once
#include <SDL2/SDL_net.h>

#include <map>
#include <string>
#include <vector>

#include "Network/Network.hpp"
#include "server.hpp"

class NetworkServer : public Network {
 private:
  UDPsocket serverSocket;
  UDPpacket* packet;
  std::vector<IPaddress> clients;
  std::map<uint32_t, ServerPlayer> players;

  void handleNewConnection(uint32_t clientKey, IPaddress& address,
                           uint32_t now);
  void sendWelcomePacket(IPaddress& address, uint8_t assignedId);

 public:
  bool start(int port);
  void stop() override;
  int handleIncomingData() override;
  bool sendData(void* data, int size) override;
  void checkDisconnects(uint32_t currentTime, uint32_t timeoutMs);

  std::map<uint32_t, ServerPlayer>& getPlayersMutable();
  const std::map<uint32_t, ServerPlayer>& getPlayers();
};