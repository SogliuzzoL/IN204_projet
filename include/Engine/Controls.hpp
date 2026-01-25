#pragma once
#include <cstdint>

#include "GameWindow.hpp"
#include "Game_logic.hpp"

typedef struct vector2f {
  float x, y;
} vector2f;

uint8_t check_events(bool *done, player *t, Game_State *state, bool *menu_sel);
