#include "Network/PacketHandler.hpp"

#include <cmath>
#include <iostream>

#include "server.hpp"

void PacketHandler::processServerPacket(void* data, __u_long size,
                                        ServerPlayer& player) {
  if (size < sizeof(PacketHeader)) return;
  PacketHeader* header = static_cast<PacketHeader*>(data);

  if (header->type == PACKET_TYPE_INPUT) {
    if (size >= sizeof(InputPacket)) {
      handleInputPacket(static_cast<InputPacket*>(data), player);
    }
  }
}

void PacketHandler::handleInputPacket(InputPacket* packet,
                                      ServerPlayer& player) {
  player.yaw = packet->yaw;
  player.pitch = packet->pitch;

  player.inputButtons = packet->inputButtons;
}

void PacketHandler::processClientPacket(void* data, __u_long size,
                                        std::vector<Entity>& worldState,
                                        uint8_t& outMyID) {
  if (size < sizeof(PacketHeader)) return;
  PacketHeader* header = static_cast<PacketHeader*>(data);

  if (header->type == PACKET_TYPE_WORLD_STATE) {
    if (size >= sizeof(PacketHeader) + sizeof(uint8_t)) {
      handleWorldStatePacket(static_cast<WorldStatePacket*>(data), worldState);
    }
  } else if (header->type == PACKET_TYPE_WELCOME) {
    if (size >= sizeof(WelcomePacket)) {
      WelcomePacket* wp = static_cast<WelcomePacket*>(data);
      outMyID = wp->assignedId;
      std::cout << "ID Reçu du serveur : " << (int)outMyID << std::endl;
    }
  } else if (header->type == PACKET_TYPE_SEED) {
    // Seed packet is handled separately with a different signature
  }
}

void PacketHandler::handleWorldStatePacket(WorldStatePacket* packet,
                                           std::vector<Entity>& worldState) {
  worldState.clear();
  for (int i = 0; i < packet->entityCount; i++) {
    worldState.push_back(packet->entities[i]);
  }
}

void PacketHandler::handleSeedPacket(SeedPacket* packet, uint32_t& outSeed) {
  outSeed = packet->seed;
  std::cout << "Seed reçue du serveur : " << outSeed << std::endl;
}

void PacketHandler::processClientPacket(void* data, __u_long size,
                                        std::vector<Entity>& worldState,
                                        uint8_t& outMyID, uint32_t& outSeed) {
  if (size < sizeof(PacketHeader)) return;
  PacketHeader* header = static_cast<PacketHeader*>(data);

  if (header->type == PACKET_TYPE_WORLD_STATE) {
    if (size >= sizeof(PacketHeader) + sizeof(uint8_t)) {
      handleWorldStatePacket(static_cast<WorldStatePacket*>(data), worldState);
    }
  } else if (header->type == PACKET_TYPE_WELCOME) {
    if (size >= sizeof(WelcomePacket)) {
      WelcomePacket* wp = static_cast<WelcomePacket*>(data);
      outMyID = wp->assignedId;
      std::cout << "ID Reçu du serveur : " << (int)outMyID << std::endl;
    }
  } else if (header->type == PACKET_TYPE_SEED) {
    if (size >= sizeof(SeedPacket)) {
      handleSeedPacket(static_cast<SeedPacket*>(data), outSeed);
    }
  }
}