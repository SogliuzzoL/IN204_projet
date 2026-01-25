#include "Engine/Menu.hpp"

#include <GL/glext.h>

// Pass the pixel X/Y and the total width/height of the atlas
void glTexCoordPixel(float x, float y, float atlasW, float atlasH) {
  glTexCoord2f(x / atlasW, y / atlasH);
}
GLuint LoadTextureSDL(const char* filename) {
  SDL_Surface* tempSurf = IMG_Load(filename);
  if (!tempSurf) {
    printf("SDL_image Error: %s\n", IMG_GetError());
    return 0;
  }

  // NEW: Convert any image format into standardized 32-bit RGBA
  // This handles the "Indigo-to-Alpha" and byte-order issues automatically
  SDL_Surface* surface =
      SDL_ConvertSurfaceFormat(tempSurf, SDL_PIXELFORMAT_ABGR8888, 0);
  SDL_FreeSurface(tempSurf);  // Delete the original unformatted surface

  if (!surface) {
    printf("Surface conversion error: %s\n", SDL_GetError());
    return 0;
  }

  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Now we KNOW it is 4 bytes and specifically GL_RGBA compatible
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, surface->pixels);

  SDL_FreeSurface(surface);
  return textureID;
}

void draw_new_game() {
  glBegin(GL_QUADS);
  glTexCoordPixel(273, 152, 750, 519);
  glVertex2f(350, 350);
  glTexCoordPixel(383, 152, 750, 519);
  glVertex2f(920, 350);
  glTexCoordPixel(383, 167, 750, 519);
  glVertex2f(920, 300);
  glTexCoordPixel(273, 167, 750, 519);
  glVertex2f(350, 300);
  glEnd();
}

void draw_quit_game() {
  glBegin(GL_QUADS);
  glTexCoordPixel(271, 242, 750, 519);
  glVertex2f(360, 250);
  glTexCoordPixel(386, 242, 750, 519);
  glVertex2f(910, 250);
  glTexCoordPixel(386, 260, 750, 519);
  glVertex2f(910, 190);
  glTexCoordPixel(271, 260, 750, 519);
  glVertex2f(360, 190);
  glEnd();
}
void draw_logo() {
  glBegin(GL_QUADS);
  glTexCoordPixel(157, 63, 750, 519);
  glVertex2f(390, 700);
  glTexCoordPixel(280, 63, 750, 519);
  glVertex2f(880, 700);
  glTexCoordPixel(280, 123, 750, 519);
  glVertex2f(880, 460);
  glTexCoordPixel(157, 123, 750, 519);
  glVertex2f(390, 460);
  glEnd();
}
void draw_heads(bool select) {
  int y_shift = (select) ? 0 : -100;
  glBegin(GL_QUADS);
  glTexCoordPixel(118, 102, 750, 519);
  glVertex2f(280, 350 + y_shift);
  glTexCoordPixel(139, 102, 750, 519);
  glVertex2f(330, 350 + y_shift);
  glTexCoordPixel(139, 120, 750, 519);
  glVertex2f(330, 300 + y_shift);
  glTexCoordPixel(118, 120, 750, 519);
  glVertex2f(280, 300 + y_shift);
  glEnd();
}

void Render_Menu(GLuint bgTex, GLuint atlasTex, bool new_game_selected) {
  // 1. Setup the "Camera" for 2D
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 1280, 0, 720, -1, 1);  // Left, Right, Bottom, Top, Near, Far

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_TEXTURE_2D);  // VERY IMPORTANT: Legacy GL needs this enabled!

  glColor3f(1.0f, 1.0f, 1.0f);  // Reset color to white so texture isn't tinted

  glBindTexture(GL_TEXTURE_2D, atlasTex);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.1f);  // Only draw pixels with alpha > 10%
  draw_logo();
  draw_new_game();
  draw_quit_game();
  draw_heads(new_game_selected);

  glBindTexture(GL_TEXTURE_2D, bgTex);

  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(0, 720);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(1280, 720);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(1280, 0);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(0, 0);
  glEnd();
}
