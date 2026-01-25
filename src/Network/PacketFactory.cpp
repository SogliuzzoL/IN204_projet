#include "Network/PacketFactory.hpp"

#include <SDL2/SDL_net.h>

#include <cstring>

#include "Network/Protocol.hpp"

void PacketFactory::createInputPacket(UDPpacket* packet, uint32_t sequence,
                                      float yaw, float pitch,
                                      uint8_t inputButtons) {
  InputPacket inputPacket;
  inputPacket.header.type = PACKET_TYPE_INPUT;
  inputPacket.header.sequence = sequence;
  inputPacket.header.timestamp = SDL_GetTicks();
  inputPacket.yaw = yaw;
  inputPacket.pitch = pitch;
  inputPacket.inputButtons = inputButtons;

  packet->len = sizeof(InputPacket);
  std::memcpy(packet->data, &inputPacket, sizeof(InputPacket));
}

void PacketFactory::createWorldStatePacket(UDPpacket* packet, uint32_t sequence,
                                           const Entity* entities,
                                           uint8_t entityCount) {
  size_t packetSize =
      sizeof(PacketHeader) + sizeof(uint8_t) + (entityCount * sizeof(Entity));

  if (packetSize > (size_t)packet->maxlen) {
    return;
  }

  WorldStatePacket* wsPacket =
      reinterpret_cast<WorldStatePacket*>(packet->data);

  wsPacket->header.type = PACKET_TYPE_WORLD_STATE;
  wsPacket->header.sequence = sequence;
  wsPacket->header.timestamp = SDL_GetTicks();
  wsPacket->entityCount = entityCount;

  std::memcpy(wsPacket->entities, entities, entityCount * sizeof(Entity));

  packet->len = packetSize;
}

void PacketFactory::createSeedPacket(UDPpacket* packet, uint32_t sequence,
                                     uint32_t seed) {
  SeedPacket seedPacket;
  seedPacket.header.type = PACKET_TYPE_SEED;
  seedPacket.header.sequence = sequence;
  seedPacket.header.timestamp = SDL_GetTicks();
  seedPacket.seed = seed;

  packet->len = sizeof(SeedPacket);
  std::memcpy(packet->data, &seedPacket, sizeof(SeedPacket));
}