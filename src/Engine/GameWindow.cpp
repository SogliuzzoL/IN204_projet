#include "Engine/GameWindow.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

int sdl_test() {
  SDL_Window *window = NULL;
  SDL_GLContext gl_ctx;
  bool running = true;
  cam_pos position = {0,0,3};
  if (!init_sdl_gl(&window, &gl_ctx)) {
    return 1;
  }

  while (running) {
    handle_events(&running, position);
    render_scene();
    SDL_GL_SwapWindow(window);
  }

  cleanup(window, gl_ctx);
  return 0;
}

/* ---------- Init ---------- */
bool init_sdl_gl(SDL_Window **window, SDL_GLContext *gl_ctx) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("SDL_Init Error: %s", SDL_GetError());
    return false;
  }

  /* Request OpenGL compatibility profile */
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  *window = SDL_CreateWindow(
      "SDL OpenGL 3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

  if (!*window) {
    SDL_Log("Window Error: %s", SDL_GetError());
    return false;
  }

  *gl_ctx = SDL_GL_CreateContext(*window);
  SDL_GL_SetSwapInterval(1); // vsync

  /* ---------- OpenGL state ---------- */
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // gluPerspective(70.0, (double)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1, 100.0);

  glMatrixMode(GL_MODELVIEW);

  return true;
}

/* ---------- Events ---------- */
void handle_events(bool *running, cam_pos position) {
    SDL_Event e;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            *running = false;
    }

    /* WASD movement */
    if (keys[SDL_SCANCODE_W]) position.z -= CAM_SPEED;
    if (keys[SDL_SCANCODE_S]) position.z += CAM_SPEED;
    if (keys[SDL_SCANCODE_A]) position.x -= CAM_SPEED;
    if (keys[SDL_SCANCODE_D]) position.x += CAM_SPEED;
    if (keys[SDL_SCANCODE_SPACE]) position.y += CAM_SPEED;
    if (keys[SDL_SCANCODE_LCTRL]) position.y -= CAM_SPEED;

}


/* ---------- Render ---------- */
void render_scene(void) {
  glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -3.0f);

  glBegin(GL_TRIANGLES);
  glColor3f(1, 0, 0);
  glVertex3f(-1, -1, 0);

  glColor3f(0, 1, 0);
  glVertex3f(1, -1, 0);

  glColor3f(0, 0, 1);
  glVertex3f(0, 1, 0);
  glEnd();
}

/* ---------- Cleanup ---------- */
void cleanup(SDL_Window *window, SDL_GLContext gl_ctx) {
  SDL_GL_DeleteContext(gl_ctx);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
