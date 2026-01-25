#pragma once
#include <SDL2/SDL_net.h>

#include <cstdint>

/**
 * Represents a player connected to the server.
 */
struct ServerPlayer {
  uint8_t id = 255;
  IPaddress address;
  uint32_t lastSeenTime = 0;

  float x = 1;
  float y = 1;
  float yaw = 0;
  float pitch = 0;
  uint8_t inputButtons = 0;
  uint32_t lastSequence = 0;
};

/**
 * Starts and runs the network server.
 * @return Exit code indicating success or failure.
 */
int run_server();