#include "Engine/GameWindow.hpp"
#include <SDL2/SDL_video.h>


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
  triangle tri = {0, 0, 0};
  rendering_loop(ctx, &frames, window, tri);
  timing_info(then, frames);
  quit(0);
  return 0;
}