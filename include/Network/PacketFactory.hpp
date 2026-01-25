#pragma once
#include <SDL2/SDL_net.h>

#include "Network/Protocol.hpp"

class PacketFactory {
 public:
  /** Creates an InputPacket and fills the given UDPpacket.
   * @param packet Pointer to the UDPpacket to fill.
   * @param sequence Sequence number for the packet.
   * @param yaw Player's yaw.
   * @param pitch Player's pitch.
   * @param inputButtons Bitmask of input buttons pressed.
   */
  static void createInputPacket(UDPpacket* packet, uint32_t sequence, float yaw,
                                float pitch, uint8_t inputButtons);

  /** Creates a WorldStatePacket and fills the given UDPpacket.
   * @param packet Pointer to the UDPpacket to fill.
   * @param sequence Sequence number for the packet.
   * @param entities Array of entities in the world.
   * @param entityCount Number of entities in the array.
   */
  static void createWorldStatePacket(UDPpacket* packet, uint32_t sequence,
                                     const Entity* entities,
                                     uint8_t entityCount);

  /** Creates a SeedPacket and fills the given UDPpacket.
   * @param packet Pointer to the UDPpacket to fill.
   * @param sequence Sequence number for the packet.
   * @param seed The maze seed to send.
   */
  static void createSeedPacket(UDPpacket* packet, uint32_t sequence,
                               uint32_t seed);
};