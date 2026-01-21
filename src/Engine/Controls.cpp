#include "Engine/Controls.hpp"

void check_events(int *done, triangle *t) {
  SDL_Event ev;
  while (SDL_PollEvent(&ev)) {
    if (ev.type == SDL_KEYDOWN) {
      switch (ev.key.keysym.sym) {
      case SDLK_ESCAPE:
        *done = 1;
        break;
      case SDLK_w:
        t->y += 0.05f;
        break;
      case SDLK_s:
        t->y -= 0.05f;
        break;
      case SDLK_a:
        t->x -= 0.05f;
        break;
      case SDLK_d:
        t->x += 0.05f;
        break;
      case SDLK_q:
        t->angle += 5.0f;
        break;
      case SDLK_e:
        t->angle -= 5.0f;
        break;
      }
    }
  }
  return;
}
