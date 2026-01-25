#include "client.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "Engine/CameraRendering.hpp"
#include "Engine/GameWindow.hpp"
#include "Network/NetworkClient.hpp"
#include "Network/PacketFactory.hpp"

int run_client(const std::string& serverIP) {
  std::cout << "Starting in client mode..." << std::endl;

  Uint32 then, frames;

  int init = init_sdl();
  if (init == 1) {
    return 1;
  }
  window = make_window();
  if (window == NULL) {
    return 1;
  }

  ctx = SDL_GL_CreateContext(window);
  SDL_GL_SetSwapInterval(1);

  rendering_settings();

  NetworkClient client;
  if (!client.initialize()) {
    std::cerr << "Erreur : Impossible d'initialiser SDL_net." << std::endl;
    return 1;
  }

  std::cout << "Tentative de connexion au serveur..." << std::endl;
  NetworkClient* clientPtr = nullptr;
  if (!client.connect(serverIP.c_str(), 12345)) {
    std::cerr << "Avertissement : Impossible de se connecter au serveur "
              << serverIP << ":12345 - Mode solo activé" << std::endl;
    clientPtr = nullptr;  // nullptr = mode solo
  } else {
    std::cout << "Connecté au serveur !" << std::endl;
    clientPtr = &client;
  }

  frames = 0;
  then = SDL_GetTicks();
  player p = {0.0f, 0.0f, 0};
  std::vector<wall> walls = {
      {0.5, 0.5, 1, 1}, {0, 0.5, 4, 2}, {0.5, 0.3, -1, 5}};

  rendering_loop(ctx, &frames, window, p, walls, clientPtr);

  timing_info(then, frames);

  client.stop();
  client.quit();
  quit(0);

  return 0;
}