#include "Engine/GameWindow.hpp"
#include <SDL2/SDL.h>

bool init_sdl(SDL_Window **window, SDL_Renderer **renderer) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("SDL_Init Error: %s", SDL_GetError());
    return false;
  }

  *window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                             WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

  if (!*window) {
    SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
    SDL_Quit();
    return false;
  }

  *renderer = SDL_CreateRenderer(
      *window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!*renderer) {
    SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
    SDL_DestroyWindow(*window);
    SDL_Quit();
    return false;
  }

  return true;
}

void handle_events(bool *running) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      *running = false;
      break;
    default:
      break;
    }
  }
}

void render(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_Rect rect = {100, 100, 50, 50};
  SDL_RenderDrawRect(renderer, &rect);

  SDL_RenderPresent(renderer);
}

void cleanup(SDL_Window *window, SDL_Renderer *renderer) {
  if (renderer)
    SDL_DestroyRenderer(renderer);
  if (window)
    SDL_DestroyWindow(window);
  SDL_Quit();
}

int sdl_test() {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    bool running = true;

    SDL_SetHint("DOOMSIM", "my_sdl_app");

    if (!init_sdl(&window, &renderer)) {
        return 1;
    }

    while (running) {
        handle_events(&running);
        render(renderer);
    }

    cleanup(window, renderer);
    return 0;
}
