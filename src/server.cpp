#include "server.hpp"

#include <cmath>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

#include "Engine/Maze.hpp"
#include "Network/NetworkServer.hpp"
#include "Network/PacketFactory.hpp"
#include "Network/Protocol.hpp"

uint32_t g_mazeSeed = 0;

/**
 * Runs the network server for the game.
 * Initializes the maze, starts a UDP server, and handles player movement
 * with collision detection against maze walls. Periodically broadcasts world
 * state to all connected clients.
 * @return 0 on successful shutdown, -1 on initialization error.
 */
int run_server() {
  std::cout << "Starting in server mode..." << std::endl;

  std::random_device rd;
  g_mazeSeed = rd();
  std::cout << "Seed du maze générée : " << g_mazeSeed << std::endl;

  set_maze_seed(g_mazeSeed);
  Maze<10> maze;
  maze.n_shifts(10000);
  std::cout << "Labyrinthe généré avec taille 10x10" << std::endl;

  NetworkServer server;

  if (!server.initialize()) {
    std::cerr << "Failed to initialize network." << std::endl;
    return -1;
  }

  if (!server.start(12345)) {
    std::cerr << "Failed to start server." << std::endl;
    return -1;
  }

  std::cout << "Server started. Waiting for packets (Ctrl+C to stop)..."
            << std::endl;

  UDPpacket* sendPacket = SDLNet_AllocPacket(512);
  uint32_t sequence = 0;

  while (true) {
    while (server.handleIncomingData() > 0) {
    }

    server.checkDisconnects(SDL_GetTicks(), 3000);

    for (auto& pair : server.getPlayersMutable()) {
      ServerPlayer& p = pair.second;
      float moveSpeed = 0.1f;

      if (p.inputButtons & INPUT_SPRINT) {
        moveSpeed *= 2.0f;
      }

      float rad = p.yaw * (3.14159f / 180.0f);
      float dirX = -sin(rad);
      float dirY = cos(rad);

      float newX = p.x;
      float newY = p.y;

      if (p.inputButtons & INPUT_FORWARD) {
        newX += dirX * moveSpeed;
        newY += dirY * moveSpeed;
      }
      if (p.inputButtons & INPUT_BACKWARD) {
        newX -= dirX * moveSpeed;
        newY -= dirY * moveSpeed;
      }
      if (p.inputButtons & INPUT_LEFT) {
        newX -= dirY * moveSpeed;
        newY += dirX * moveSpeed;
      }
      if (p.inputButtons & INPUT_RIGHT) {
        newX += dirY * moveSpeed;
        newY -= dirX * moveSpeed;
      }

      uint32_t mazeSize = 3 * 10;

      auto isPositionValid = [&](float x, float y) -> bool {
        int32_t gridX = static_cast<int32_t>(std::round(x));
        int32_t gridY = static_cast<int32_t>(std::round(y));

        if (gridX < 0 || gridX >= static_cast<int32_t>(mazeSize) || gridY < 0 ||
            gridY >= static_cast<int32_t>(mazeSize)) {
          return false;
        }

        return maze.is_open(static_cast<uint32_t>(gridY),
                            static_cast<uint32_t>(gridX));
      };

      if (isPositionValid(newX, newY)) {
        p.x = newX;
        p.y = newY;
      } else {
        if (isPositionValid(newX, p.y)) {
          p.x = newX;
        } else if (isPositionValid(p.x, newY)) {
          p.y = newY;
        }
      }
    }

    if (sequence % 2 == 0) {
      const auto& players = server.getPlayers();

      std::vector<Entity> entities;
      entities.reserve(players.size());

      for (const auto& pair : players) {
        const ServerPlayer& sp = pair.second;
        if (sp.id == 255) continue;

        Entity e;
        e.id = sp.id;
        e.x = sp.x;
        e.y = sp.y;
        e.yaw = sp.yaw;
        e.pitch = sp.pitch;
        entities.push_back(e);
      }

      if (!entities.empty()) {
        PacketFactory::createWorldStatePacket(
            sendPacket, sequence, entities.data(),
            static_cast<uint8_t>(entities.size()));
        server.sendData(sendPacket->data, sendPacket->len);
      }
    }

    sequence++;
    SDL_Delay(16);
  }

  SDLNet_FreePacket(sendPacket);
  server.stop();
  server.quit();
  return 0;
}