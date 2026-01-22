#include "Engine/Controls.hpp"
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_scancode.h>
#include <cmath>

const float PI = 3.14159265358979323846f;
const float step_size = 0.1f;

void check_events(int *done, player *t) {
  SDL_Event ev;

  const Uint8 *state = SDL_GetKeyboardState(NULL);

  // Handle quit-event
  while (SDL_PollEvent(&ev)) {
    if (ev.type == SDL_QUIT ||
        (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE)) {
      *done = 1;
    }
  }

  float rad = t->angle * (PI / 180.0f);

  vector2f fwd = {-sin(rad), cos(rad)};

  vector2f right = {cos(rad), sin(rad)};

  float moveX = 0;
  float moveY = 0;

  if (state[SDL_SCANCODE_W]) {
    moveX += fwd.x;
    moveY += fwd.y;
  }
  if (state[SDL_SCANCODE_S]) {
    moveX -= fwd.x;
    moveY -= fwd.y;
  }
  if (state[SDL_SCANCODE_A]) {
    moveX -= right.x;
    moveY -= right.y;
  }
  if (state[SDL_SCANCODE_D]) {
    moveX += right.x;
    moveY += right.y;
  }

  int sprint = (state[SDL_SCANCODE_LCTRL]) ? 2 : 1;

  t->x += moveX * step_size * sprint;
  t->y += moveY * step_size * sprint;

  if (state[SDL_SCANCODE_RIGHT])
    t->angle -= 2.0f;
  if (state[SDL_SCANCODE_LEFT])
    t->angle += 2.0f;
}