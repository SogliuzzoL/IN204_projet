#include "client.hpp"

#include <iostream>

#include "Network/NetworkClient.hpp"
#include "Network/PacketFactory.hpp"

int run_client() {
  std::cout << "Starting in client mode..." << std::endl;

  NetworkClient client;

  if (!client.initialize()) {
    std::cerr << "Failed to initialize network." << std::endl;
    return -1;
  }

  if (!client.connect("127.0.0.1", 12345)) {
    std::cerr << "Failed to connect to server." << std::endl;
    return -1;
  }

  std::cout << "Client connected. Sending inputs..." << std::endl;

  UDPpacket* sendPacket = SDLNet_AllocPacket(512);
  uint32_t sequence = 0;

  while (true) {
    if (sequence % 60 == 0) {
      PacketFactory::createInputPacket(sendPacket, sequence, 3.14f, 0.0f, 33);
      client.sendData(sendPacket->data, sendPacket->len);
    }

    client.handleIncomingData();

    sequence++;
    SDL_Delay(16);
  }

  SDLNet_FreePacket(sendPacket);
  client.stop();
  client.quit();
  return 0;
}