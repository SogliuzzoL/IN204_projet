// #include "Engine/GameWindow.hpp"

// int main() {
//   int err = sdl_test();
//   return err;
// }

#include <SDL2/SDL_keycode.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

float triX = 0.0f;
float triY = 0.0f;
float triAngle = 0.0f;

static SDL_GLContext ctx;
static SDL_Window *window;
/* Call this instead of exit(), so we can clean up SDL: atexit() is evil. */
static void quit(int rc) {
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

static void Render(int w, int h)
{
    SetOrtho(w, h);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Move triangle
    glTranslatef(triX, triY, 0.0f);

    // Rotate around its center
    glRotatef(triAngle, 0.0f, 0.0f, 1.0f);

    float s = 0.2f;
    float htri = s * 0.8660254f;

    glBegin(GL_TRIANGLES);
        glColor3f(1,1,1);
        glVertex2f( 0.0f,        2.0f * htri / 3.0f);
        glVertex2f(-s / 2.0f,   -htri / 3.0f);
        glVertex2f( s / 2.0f,   -htri / 3.0f);
    glEnd();
}


int main() {
  int done;
  Uint32 then, now, frames;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  // Create window
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  window =
      SDL_CreateWindow("in201", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 1280, 800, SDL_WINDOW_OPENGL);

  if (window == NULL) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  // creating new context
  ctx = SDL_GL_CreateContext(window);
  SDL_GL_SetSwapInterval(1);

  /* Set rendering settings */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glShadeModel(GL_SMOOTH);

  /* Main render loop */
  frames = 0;
  then = SDL_GetTicks();
  done = 0;

  while (!done) {
    /* Check for events */
    ++frames;
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {

      if (ev.type == SDL_KEYDOWN) {
        switch (ev.key.keysym.sym) {
        case SDLK_ESCAPE:
          done = 1;
          break;
        case SDLK_w:
          triY += 0.05f;
          break;
        case SDLK_s:
          triY -= 0.05f;
          break;
        case SDLK_a:
          triX -= 0.05f;
          break;
        case SDLK_d:
          triX += 0.05f;
          break;
        case SDLK_q:
          triAngle += 5.0f;
          break;
        case SDLK_e:
          triAngle -= 5.0f;
          break;
        }
      }
    }
    int w, h;
    SDL_GL_MakeCurrent(window, ctx);
    SDL_GetWindowSize(window, &w, &h);
    glViewport(0, 0, w, h);
    Render(w, h);
    SDL_GL_SwapWindow(window);
  }

  /* Print out some timing information */
  now = SDL_GetTicks();
  if (now > then) {
    printf("%2.2f frames per second\n", ((double)frames * 1000) / (now - then));
  }
  quit(0);
  return 0;
}