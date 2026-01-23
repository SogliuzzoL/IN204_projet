#include <iostream>
#include <string>
#include <vector>

#include "Engine/CameraRendering.hpp"
#include "Engine/GameWindow.hpp"
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

  Uint32 then, frames;

  int init = init_sdl();
  if (init == 1) {
    return 1;
  }
  window = make_window();
  if (window == NULL) {
    return 1;
  }

  // creating new context
  ctx = SDL_GL_CreateContext(window);
  SDL_GL_SetSwapInterval(1);

  rendering_settings();

  frames = 0;
  then = SDL_GetTicks();
  player p = {0, 0, 0};
  std::vector<wall> walls = {
      {0.5, 0.5, 1, 1}, {0, 0.5, 4, 2}, {0.5, 0.3, -1, 5}};
  rendering_loop(ctx, &frames, window, p, walls);
  timing_info(then, frames);
  quit(0);

  return 0;
}

int main(int argc, char** argv) {
  // Server Mode
  if (argc > 1)
    if (std::string(argv[1]) == "--server") return server();

  // Client Mode
  return client();
}