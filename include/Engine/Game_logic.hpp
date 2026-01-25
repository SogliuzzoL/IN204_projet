#pragma once

typedef struct player {
  float x, y, angle;
} player;

typedef struct wall {
  float x1, y1, x2, y2;
} wall;

typedef enum State {
  STATE_MENU,
  STATE_PLAYING,
  STATE_PAUSED,
} Game_State;