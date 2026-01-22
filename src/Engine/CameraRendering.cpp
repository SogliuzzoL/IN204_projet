#include "Engine/CameraRendering.hpp"
#include "Engine/Controls.hpp"
#include <SDL2/SDL_opengl.h>
#include <vector>

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

void draw_player() {
  float s = 0.2f;
  float htri = s * 0.8660254f;
  glBegin(GL_TRIANGLES);
  glColor3f(1, 0, 1);
  glVertex2f(0.0f, 2.0f * htri / 3.0f);
  glColor3f(1, 1, 1);
  glVertex2f(-s / 2.0f, -htri / 3.0f);
  glVertex2f(s / 2.0f, -htri / 3.0f);
  glEnd();
}

void draw_walls(std::vector<wall> walls) {
  glBegin(GL_LINES);
  glColor3f(1, 1, 1);
  for (long unsigned int i = 0; i < walls.size(); i++) {
    glVertex2f(walls[i].x, walls[i].y);
    glVertex2f(0,0);
  }
  glEnd();
}


void Render(player p, std::vector<wall> walls, int w, int h) {
  SetOrtho(w, h);

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  draw_player();
  
  
  // Rotate around its center
  glRotatef(-p.angle, 0.0f, 0.0f, 1.0f);
  // the world around moves opposite of the player
  glTranslatef(-p.x, -p.y, 0.0f);
  
  // CAREFUL: The translation is done before the rotation
  
  draw_walls(walls);
}

void rendering_settings() {
  /* Set rendering settings */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glShadeModel(GL_SMOOTH);
}
void rendering_loop(SDL_GLContext ctx, Uint32 *frames, SDL_Window *window,
                    player t, std::vector<wall> walls) {
  /* Main render loop */
  int done = 0;

  while (!done) {
    /* Check for events */
    ++(*frames);
    check_events(&done, &t);
    int w, h;
    SDL_GL_MakeCurrent(window, ctx);
    SDL_GetWindowSize(window, &w, &h);
    glViewport(0, 0, w, h);
    Render(t, walls, w, h);
    SDL_GL_SwapWindow(window);
  }
}