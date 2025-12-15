#include "Engine/GameWindow.hpp"
#include <SDL2/SDL.h>

bool GameWindow::initialize() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    return false;
  }

  SDL_Window *window =
      SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       width, height, SDL_WINDOW_SHOWN + SDL_WINDOW_FULLSCREEN);
  if (!window) {
    SDL_Quit();
    return false;
  }

  return true;
}

void GameWindow::kill() { SDL_Quit(); }