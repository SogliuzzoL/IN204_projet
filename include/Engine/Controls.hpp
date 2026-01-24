#pragma once
#include "Game_logic.hpp"
#include "GameWindow.hpp"

typedef struct vector2f {
  float x, y;
} vector2f;

void check_events(bool *done, player *t, bool* sel, Game_State* state);