#include "Engine/GameWindow.hpp"
#include <SDL2/SDL_video.h>
#include <cstddef>

triangle t = {0, 0, 0};

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
  window =
      SDL_CreateWindow("in201", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 1280, 800, SDL_WINDOW_OPENGL);

  if (window == NULL) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return NULL;
  }
  return window;
};

void rendering_settings() {
  /* Set rendering settings */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glShadeModel(GL_SMOOTH);
}

void rendering_loop(Uint32 then, Uint32 frames, SDL_Window *window) {
  /* Main render loop */
  int done = 0;

  while (!done) {
    /* Check for events */
    ++frames;
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_KEYDOWN) {
        switch (ev.key.keysym.sym) {
        case SDLK_ESCAPE:
          done = 1;
          break;
        case SDLK_w:
          t.y += 0.05f;
          break;
        case SDLK_s:
          t.y -= 0.05f;
          break;
        case SDLK_a:
          t.x -= 0.05f;
          break;
        case SDLK_d:
          t.x += 0.05f;
          break;
        case SDLK_q:
          t.angle += 5.0f;
          break;
        case SDLK_e:
          t.angle -= 5.0f;
          break;
        }
      }
    }
    int w, h;
    SDL_GL_MakeCurrent(window, ctx);
    SDL_GetWindowSize(window, &w, &h);
    glViewport(0, 0, w, h);
    Render(t, w, h);
    SDL_GL_SwapWindow(window);
  }
}

void timing_info(Uint32 then, Uint32 frames) {
  /* Print out some timing information */
  Uint32 now = SDL_GetTicks();
  if (now > then) {
    printf("%2.2f frames per second\n", ((double)frames * 1000) / (now - then));
  }
}

int main() {
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
  rendering_loop(then, frames, window);
  timing_info(then, frames);
  quit(0);
  return 0;
}