#include "server.hpp"

#include <iostream>

#include "Network/NetworkServer.hpp"
#include "Network/PacketFactory.hpp"

int run_server() {
  std::cout << "Starting in server mode..." << std::endl;

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
    server.handleIncomingData();

    if (sequence % 60 == 0) {
      Entity dummyEntities[1];
      dummyEntities[0] = {1, 10.5f, -5.0f, 1.57f, 0.0f};

      PacketFactory::createWorldStatePacket(sendPacket, sequence, dummyEntities,
                                            1);

      server.sendData(sendPacket->data, sendPacket->len);
      std::cout << "Sent WorldStatePacket seq " << sequence << std::endl;
    }

    sequence++;
    SDL_Delay(16);
  }

  SDLNet_FreePacket(sendPacket);
  server.stop();
  server.quit();
  return 0;
}