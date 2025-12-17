#include <iostream>

#include "Engine/GameWindow.hpp"

int main() {
  GameWindow window(1920, 1080, "DOOM");
  if (!window.initialize()) {
    std::cerr << "Failed to initialize the game window." << std::endl;
    return -1;
  }
  std::cout << "Game window initialized successfully." << std::endl;

  window.kill();

  return 0;
}