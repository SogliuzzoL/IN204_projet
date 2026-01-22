#include "Engine/CameraRendering.hpp"
#include "Engine/Controls.hpp"
#include <GL/glu.h>
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
  glBegin(GL_QUADS);
  glColor3f(1, 1, 1);
  for (long unsigned int i = 0; i < walls.size(); i++) {
    glVertex3f(walls[i].x1, walls[i].y1,0);
    glVertex3f(walls[i].x1, walls[i].y1,2);
    glVertex3f(walls[i].x2, walls[i].y2,0);
    glVertex3f(walls[i].x2, walls[i].y2,2);
  }
  glEnd();
}

void draw_cube() {
    glBegin(GL_QUADS);

    // Front Face (Z = 0.5)
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);

    // Back Face (Z = -0.5)
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);

    // Top Face (Y = 0.5)
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);

    // Bottom Face (Y = -0.5)
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);

    // Right face (X = 0.5)
    glColor3f(1.0f, 0.0f, 1.0f); // Magenta
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);

    // Left Face (X = -0.5)
    glColor3f(0.0f, 1.0f, 1.0f); // Cyan
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f,  0.5f, -0.5f);

    glEnd();
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

void Render(player p,std::vector<wall> walls, int w, int h) {
    // 1. Perspective Setup
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)w / h;
    // FOV, Aspect, Near, Far
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);

    // 2. Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST); // IMPORTANT: Stops back faces from drawing over front faces

    // 3. Camera (Modelview) Setup
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Transform world relative to player
    // Note: We rotate X so that Z is "Up" and Y is "Forward"
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); 
    glRotatef(-p.angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(-p.x, -p.y, -2.0f); // -1.0 is player eye height

    // 4. Draw the Cube at some world position
    glPushMatrix();
        glTranslatef(2.0f, 5.0f, 0.5f); // Place cube at (2, 5) on the map
        draw_cube();
    glPopMatrix();
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