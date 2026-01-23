#include "Engine/Controls.hpp"

#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_scancode.h>

#include <cmath>

#include "Network/Protocol.hpp"

const float PI = 3.14159265358979323846f;
const float step_size = 0.1f;
float mouse_sensitivity = 0.1f;
bool isPaused = false;

uint8_t check_events(bool *done, player *t) {
  SDL_Event ev;
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  uint8_t buttons = 0;

  while (SDL_PollEvent(&ev)) {
    if (ev.type == SDL_QUIT ||
        (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE)) {
      *done = 1;
    }

    if (ev.type == SDL_WINDOWEVENT) {
      if (ev.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
        SDL_SetRelativeMouseMode(SDL_FALSE);
      }
    }

    if (ev.type == SDL_MOUSEBUTTONDOWN) {
      if (SDL_GetRelativeMouseMode() == SDL_FALSE) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
      }
    }

    if (ev.type == SDL_MOUSEMOTION) {
      if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
        t->angle -= ev.motion.xrel * mouse_sensitivity;
      }
    }
  }

  if (state[SDL_SCANCODE_W]) buttons |= INPUT_FORWARD;
  if (state[SDL_SCANCODE_S]) buttons |= INPUT_BACKWARD;
  if (state[SDL_SCANCODE_A]) buttons |= INPUT_LEFT;
  if (state[SDL_SCANCODE_D]) buttons |= INPUT_RIGHT;
  if (state[SDL_SCANCODE_SPACE]) buttons |= INPUT_JUMP;
  if (state[SDL_SCANCODE_LCTRL]) buttons |= INPUT_SPRINT;

  if (state[SDL_SCANCODE_RIGHT]) t->angle -= 2.0f;
  if (state[SDL_SCANCODE_LEFT]) t->angle += 2.0f;

  return buttons;
}