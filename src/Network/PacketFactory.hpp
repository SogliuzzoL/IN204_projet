#include <SDL2/SDL_net.h>

#include <cstring>

#include "Network/PacketFactory.hpp"
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
  WorldStatePacket worldStatePacket;
  worldStatePacket.header.type = PACKET_TYPE_WORLD_STATE;
  worldStatePacket.header.sequence = sequence;
  worldStatePacket.header.timestamp = SDL_GetTicks();
  worldStatePacket.entityCount = entityCount;
  std::memcpy(worldStatePacket.entities, entities,
              entityCount * sizeof(Entity));

  packet->len =
      sizeof(PacketHeader) + sizeof(uint8_t) + entityCount * sizeof(Entity);
  std::memcpy(packet->data, &worldStatePacket, packet->len);
}