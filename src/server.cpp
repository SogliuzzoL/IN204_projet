#include "server.hpp"

#include <cmath>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

#include "Network/NetworkServer.hpp"
#include "Network/PacketFactory.hpp"
#include "Network/Protocol.hpp"

uint32_t g_mazeSeed = 0;

int run_server() {
  std::cout << "Starting in server mode..." << std::endl;

  // Générer une seed aléatoire pour le maze
  std::random_device rd;
  g_mazeSeed = rd();
  std::cout << "Seed du maze générée : " << g_mazeSeed << std::endl;

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

      if (p.inputButtons & INPUT_FORWARD) {
        p.x += dirX * moveSpeed;
        p.y += dirY * moveSpeed;
      }
      if (p.inputButtons & INPUT_BACKWARD) {
        p.x -= dirX * moveSpeed;
        p.y -= dirY * moveSpeed;
      }
      if (p.inputButtons & INPUT_LEFT) {
        p.x -= dirY * moveSpeed;
        p.y += dirX * moveSpeed;
      }
      if (p.inputButtons & INPUT_RIGHT) {
        p.x += dirY * moveSpeed;
        p.y -= dirX * moveSpeed;
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