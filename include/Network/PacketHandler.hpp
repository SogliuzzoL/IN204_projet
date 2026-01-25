#pragma once
#include <vector>

#include "Network/Protocol.hpp"
#include "server.hpp"

/**
 * Handles processing of network packets.
 */
class PacketHandler {
 public:
  /** Processes a packet received by the client. */
  static void processClientPacket(void* data, __u_long size,
                                  std::vector<Entity>& worldState,
                                  uint8_t& outMyID);

  /** Processes a packet received by the server. */
  static void processServerPacket(void* data, __u_long size,
                                  ServerPlayer& player);

 private:
  /** Handles an input packet on the server side. */
  static void handleInputPacket(InputPacket* packet, ServerPlayer& player);

  /** Handles a world state packet on the client side. */
  static void handleWorldStatePacket(WorldStatePacket* packet,
                                     std::vector<Entity>& worldState);
};