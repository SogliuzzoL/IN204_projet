#pragma once
#include <SDL2/SDL_net.h>

#include "Network/Protocol.hpp"

class PacketFactory {
 public:
  /**
   * Create an InputPacket.
   * @param packet The UDP packet to fill.
   * @param sequence The sequence number of the packet.
   * @param yaw The yaw of the player.
   * @param pitch The pitch of the player.
   * @param inputButtons The input buttons state.
   * @return A pointer to the created InputPacket.
   */
  static void createInputPacket(UDPpacket* packet, uint32_t sequence, float yaw,
                                float pitch, uint8_t inputButtons);

  /**
   * Create a WorldStatePacket.
   * @param packet The UDP packet to fill.
   * @param sequence The sequence number of the packet.
   * @param entities The array of entities in the world.
   * @param entityCount The number of entities.
   * @return A pointer to the created WorldStatePacket.
   */
  static void createWorldStatePacket(UDPpacket* packet, uint32_t sequence,
                                     const Entity* entities,
                                     uint8_t entityCount);
};