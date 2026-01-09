#pragma once
#include <cstdint>

#pragma pack(push, 1)

enum PacketType : uint8_t { PACKET_TYPE_INPUT, PACKET_TYPE_WORLD_STATE };

enum InputMasks : uint8_t {
  INPUT_FORWARD = 1 << 0,
  INPUT_BACKWARD = 1 << 1,
  INPUT_LEFT = 1 << 2,
  INPUT_RIGHT = 1 << 3,
  INPUT_JUMP = 1 << 4,
  INPUT_SHOOT = 1 << 5
};

struct PacketHeader {
  uint8_t type;
  uint32_t sequence;
  uint32_t timestamp;
};

struct InputPacket {
  PacketHeader header;
  float yaw;             // rotation around the vertical axis
  float pitch;           // rotation around the lateral axis
  uint8_t inputButtons;  // bitmask for input buttons
};

struct Entity {
  uint8_t id;
  float x;
  float y;
  float yaw;
  float pitch;
};

struct WorldStatePacket {
  PacketHeader header;
  uint8_t entityCount;
  Entity entities[];
};

#pragma pack(pop)