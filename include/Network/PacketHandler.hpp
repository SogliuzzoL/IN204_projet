#pragma once
#include "Network/Protocol.hpp"

class PacketHandler {
 public:
  /**
   * Process an incoming packet.
   * @param data Pointer to the packet data.
   * @param size Size of the packet data.
   */
  static void processPacket(void* data, __u_long size);

 private:
  /**
   * Handle an InputPacket.
   * @param packet Pointer to the InputPacket.
   */
  static void handleInputPacket(InputPacket* packet);

  /**
   * Handle a WorldStatePacket.
   * @param packet Pointer to the WorldStatePacket.
   */
  static void handleWorldStatePacket(WorldStatePacket* packet);
};