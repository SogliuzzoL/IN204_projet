#include "Engine/Controls.hpp"

#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_scancode.h>

#include <cmath>

#include "Engine/Game_logic.hpp"

const float PI = 3.14159265358979323846f;
const float step_size = 0.1f;
float mouse_sensitivity = 0.1f;
bool isPaused = false;

void check_events(bool *done, player *t, bool *sel, Game_State *game_state) {
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  SDL_Event ev;

  while (SDL_PollEvent(&ev)) {
    // 1. Handle Quit
    if (ev.type == SDL_QUIT) {
      *done = true;
    }

    // 2. Handle Key Presses
    if (ev.type == SDL_KEYDOWN) {
      switch (ev.key.keysym.sym) {
        case SDLK_ESCAPE:
          *done = true;
          break;

        case SDLK_p:
          isPaused = !isPaused;
          SDL_SetRelativeMouseMode(isPaused ? SDL_FALSE : SDL_TRUE);
          SDL_SetWindowGrab(window, isPaused ? SDL_FALSE : SDL_TRUE);
          break;

        case SDLK_UP:
        case SDLK_DOWN:
          if (*game_state == STATE_MENU) {
            *sel = !(*sel);  // Correctly toggle the value
          }
          break;

        case SDLK_RETURN:
          if (*game_state == STATE_MENU) {
            (*sel) ? *game_state = STATE_PLAYING : *done = true;
          }
          break;
      }
    }

    // 3. Handle Mouse
    if (ev.type == SDL_MOUSEMOTION && !isPaused) {
      t->angle -= ev.motion.xrel * mouse_sensitivity;
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

  if (state[SDL_SCANCODE_RIGHT]) t->angle -= 2.0f;
  if (state[SDL_SCANCODE_LEFT]) t->angle += 2.0f;
}