#include <iostream>
#include <string>

#include "Network/NetworkClient.hpp"
#include "Network/NetworkServer.hpp"

int server() {
  std::cout << "Starting in server mode..." << std::endl;

  NetworkServer server;

  // Initialize network
  if (!server.initialize()) {
    std::cerr << "Failed to initialize network." << std::endl;
    return -1;
  }

  // Start server
  if (!server.start(12345)) {
    std::cerr << "Failed to start server." << std::endl;
    server.quit();
    return -1;
  }

  std::cout << "Server started successfully. Waiting for connections..."
            << std::endl;

  int i = 0;
  while (1) {
    server.handleIncomingData();
    if (i % 1000000 == 0) {
      server.sendData("Hello from server!");
      i = 0;
    }
    i++;
  }

  // Stop server
  server.stop();
  // Quit network
  server.quit();

  std::cout << "Server shut down successfully." << std::endl;

  return 0;
}

int client() {
  std::cout << "Starting in client mode..." << std::endl;

  NetworkClient client;
  // Initialize network
  if (!client.initialize()) {
    std::cerr << "Failed to initialize network." << std::endl;
    return -1;
  }
  // Start client
  if (!client.connect("localhost", 12345)) {
    std::cerr << "Failed to start client." << std::endl;
    client.quit();
    return -1;
  }

  int i = 0;
  while (1) {
    client.handleIncomingData();
    if (i % 1000000 == 0) {
      client.sendData("Hello from client!");
      i = 0;
    }
    i++;
  }

  // Stop client
  client.stop();
  // Quit network
  client.quit();
  std::cout << "Client shut down successfully." << std::endl;

  return 0;
}

int main(int argc, char** argv) {
  // Server Mode
  if (argc > 1)
    if (std::string(argv[1]) == "--server") return server();

  // Client Mode
  return client();
}