#include "Engine/Controls.hpp"

#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_scancode.h>

#include <cmath>

#include "Engine/Game_logic.hpp"
#include "Network/Protocol.hpp"

const float PI = 3.14159265358979323846f;
const float step_size = 0.1f;
float mouse_sensitivity = 0.1f;
bool isPaused = false;

uint8_t check_events(bool *done, player *t, Game_State *game_state) {
  SDL_Event ev;
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  uint8_t buttons = 0;

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
          break;
      }
    }

    // 3. Handle Window Events
    if (ev.type == SDL_WINDOWEVENT) {
      if (ev.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
        SDL_SetRelativeMouseMode(SDL_FALSE);
      }
    }

    // 4. Handle Mouse Click to Focus
    if (ev.type == SDL_MOUSEBUTTONDOWN) {
      if (SDL_GetRelativeMouseMode() == SDL_FALSE) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
      }
    }

    // 5. Handle Mouse Motion
    if (ev.type == SDL_MOUSEMOTION) {
      if (SDL_GetRelativeMouseMode() == SDL_TRUE && !isPaused) {
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