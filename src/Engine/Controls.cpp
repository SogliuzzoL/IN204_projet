#include "Engine/Controls.hpp"
#include <SDL2/SDL_opengl.h>
#include <cmath>

const float PI = 3.14159265358979323846f;
const float step_size = 0.05f;

vector2f dirvec(float angle);
vector2f dirvec(float angle) {
  float radians = angle * (PI / 180.0f);
  return (vector2f){step_size * -sin(radians), step_size * cos(radians)};
}

void check_events(int *done, player *t) {
  SDL_Event ev;
  bool move_event = false;
  float add_angle = 0;
  while (SDL_PollEvent(&ev)) {
    if (ev.type == SDL_KEYDOWN) {
      switch (ev.key.keysym.sym) {
        // exiting the window
      case SDLK_ESCAPE:
        *done = 1;
        break;
        // rotating the player
      case SDLK_q:
        t->angle += 5.0f;
        break;
      case SDLK_e:
        t->angle -= 5.0f;
        break;
        // going forwards witht the player
      case SDLK_w:
        move_event = true;
        break;
      case SDLK_s:
        move_event = true;
        add_angle = 180;
        break;
      case SDLK_a:
        move_event = true;
        add_angle = 90;
        break;
      case SDLK_d:
        move_event = true;
        add_angle = 270;
        break;
      }
    }
  }
  if (move_event) {
    vector2f dir = dirvec(t->angle + add_angle);
    t->x += dir.x;
    t->y += dir.y;
  }
  return;
}
