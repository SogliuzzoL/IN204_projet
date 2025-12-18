#include <iostream>
#include <string>

#include "Engine/GameWindow.hpp"
#include "Network/NetworkServer.hpp"

int main(int argc, char** argv) {
  // Server Mode
  if (argc > 1) {
    if (std::string(argv[1]) == "--server") {
      std::cout << "Starting in server mode..." << std::endl;

      NetworkServer server;

      // Initialize network
      if (!server.initialize()) {
        std::cerr << "Failed to initialize network." << std::endl;
        return -1;
      }

      // Start server
      if (!server.start("0.0.0.0", 12345)) {
        std::cerr << "Failed to start server." << std::endl;
        server.quit();
        return -1;
      }

      // Stop server
      server.stop();
      // Quit network
      server.quit();

      std::cout << "Server shut down successfully." << std::endl;

      return 0;
    }
  }

  // Client Mode
  GameWindow window(1920, 1080, "DOOM");
  if (!window.initialize()) {
    std::cerr << "Failed to initialize the game window." << std::endl;
    return -1;
  }
  std::cout << "Game window initialized successfully." << std::endl;

  window.kill();

  return 0;
}