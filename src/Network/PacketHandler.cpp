#include "Network/PacketHandler.hpp"

#include <iostream>

void PacketHandler::processPacket(void* data, __u_long size) {
  if (size < sizeof(PacketHeader)) {
    // Packet too small to contain a valid header
    return;
  }

  PacketHeader* header = static_cast<PacketHeader*>(data);

  switch (header->type) {
    case PACKET_TYPE_INPUT:
      if (size >= sizeof(InputPacket)) {
        handleInputPacket(static_cast<InputPacket*>(data));
      }
      break;

    case PACKET_TYPE_WORLD_STATE:
      if (size >= sizeof(WorldStatePacket)) {
        handleWorldStatePacket(static_cast<WorldStatePacket*>(data));
      }
      break;

    default:
      // Unknown packet type
      break;
  }
}

void PacketHandler::handleInputPacket(InputPacket* packet) {
  // Process the InputPacket
  std::cout << "Yaw: " << packet->yaw << ", Pitch: " << packet->pitch
            << ", Buttons: " << static_cast<int>(packet->inputButtons)
            << std::endl;
}

void PacketHandler::handleWorldStatePacket(WorldStatePacket* packet) {
  // Process the WorldStatePacket
  for (uint8_t i = 0; i < packet->entityCount; i++) {
    Entity& entity = packet->entities[i];
    std::cout << "Entity ID: " << static_cast<int>(entity.id) << ", Position: ("
              << entity.x << ", " << entity.y << ")"
              << ", Yaw: " << entity.yaw << ", Pitch: " << entity.pitch
              << std::endl;
  }
}