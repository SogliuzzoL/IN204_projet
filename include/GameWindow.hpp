#pragma once

struct GameWindow {
private:
  int width;
  int height;
  const char *title;

public:
  GameWindow(int w, int h, const char *t) : width(w), height(h), title(t) {}
  bool initialize();
  void kill();
};