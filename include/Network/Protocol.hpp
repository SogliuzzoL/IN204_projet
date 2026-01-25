#pragma once
#include <cstdint>

#pragma pack(push, 1)

/**
 * Types of packets exchanged between client and server.
 */
enum PacketType : uint8_t {
  PACKET_TYPE_INPUT,
  PACKET_TYPE_WORLD_STATE,
  PACKET_TYPE_WELCOME,
  PACKET_TYPE_SEED
};

/**
 * Input button masks.
 */
enum InputMasks : uint8_t {
  INPUT_FORWARD = 1 << 0,
  INPUT_BACKWARD = 1 << 1,
  INPUT_LEFT = 1 << 2,
  INPUT_RIGHT = 1 << 3,
  INPUT_JUMP = 1 << 4,
  INPUT_SPRINT = 1 << 5
};

/**
 * Common packet header.
 */
struct PacketHeader {
  uint8_t type;
  uint32_t sequence;
  uint32_t timestamp;
};

/**
 * Packet sent from client to server containing player input.
 */
struct InputPacket {
  PacketHeader header;
  uint8_t playerId;
  float yaw;
  float pitch;
  uint8_t inputButtons;
};

/**
 * Packet sent from server to client upon connection, assigning an ID.
 */
struct WelcomePacket {
  PacketHeader header;
  uint8_t assignedId;
};

/**
 * Packet sent from server to client containing the maze seed.
 */
struct SeedPacket {
  PacketHeader header;
  uint32_t seed;
};

/**
 * Represents an entity in the world.
 */
struct Entity {
  uint8_t id;
  float x;
  float y;
  float yaw;
  float pitch;
};

/**
 * Packet sent from server to client containing the world state.
 */
struct WorldStatePacket {
  PacketHeader header;
  uint8_t entityCount;
  Entity entities[];
};

#pragma pack(pop)