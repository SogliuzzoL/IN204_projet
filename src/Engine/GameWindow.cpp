#include "Engine/GameWindow.hpp"
#include <cstdio>
#include <SDL2/SDL.h>

void sdl_test () {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    }

    SDL_Window* window = SDL_CreateWindow(
        "SDL Hello World",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
    }

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        SDL_Delay(16); // ~60 FPS idle
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
};
