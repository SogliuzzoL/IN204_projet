#include "Engine/GameWindow.hpp"

/* Call this instead of exit(), so we can clean up SDL: atexit() is evil. */
void quit(int rc) {
  if (ctx) {
    /* SDL_GL_MakeCurrent(0, NULL); */ /* doesn't do anything */
    SDL_GL_DeleteContext(ctx);
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(rc);
}

void SetOrtho(int w, int h) {
  float aspect = (float)w / (float)h;
  float viewSize = 1.0f;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (aspect >= 1.0f) {
    glOrtho(-viewSize * aspect, viewSize * aspect, -viewSize, viewSize, -1.0,
            1.0);
  } else {
    glOrtho(-viewSize, viewSize, -viewSize / aspect, viewSize / aspect, -1.0,
            1.0);
  }

  glMatrixMode(GL_MODELVIEW);
}

void Render(triangle t, int w, int h) {
  SetOrtho(w, h);

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Move triangle
  glTranslatef(t.x, t.y, 0.0f);

  // Rotate around its center
  glRotatef(t.angle, 0.0f, 0.0f, 1.0f);

  float s = 0.2f;
  float htri = s * 0.8660254f;

  glBegin(GL_TRIANGLES);
  glColor3f(1, 1, 1);
  glVertex2f(0.0f, 2.0f * htri / 3.0f);
  glVertex2f(-s / 2.0f, -htri / 3.0f);
  glVertex2f(s / 2.0f, -htri / 3.0f);
  glEnd();
}
