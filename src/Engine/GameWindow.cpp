#include "Engine/GameWindow.hpp"

// Define global variables declared as extern in GameWindow.hpp
SDL_GLContext ctx = nullptr;
SDL_Window *window = nullptr;

/* Call this instead of exit(), so we can clean up SDL: atexit() is evil. */
void quit(int rc) {
  if (ctx) {
    /* SDL_GL_MakeCurrent(0, NULL); */ /* doesn't do anything */
    SDL_GL_DeleteContext(ctx);
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(rc);
}

int init_sdl() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }
  return 0;
};

SDL_Window *make_window() {
  // Create window
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_SetRelativeMouseMode(SDL_TRUE);
  window =
      SDL_CreateWindow("in201", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 1280, 800, SDL_WINDOW_OPENGL);

  if (window == NULL) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return NULL;
  }
  return window;
};

void timing_info(Uint32 then, Uint32 frames) {
  /* Print out some timing information */
  Uint32 now = SDL_GetTicks();
  if (now > then) {
    printf("%2.2f frames per second\n", ((double)frames * 1000) / (now - then));
  }
}
