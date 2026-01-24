#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

GLuint LoadTextureSDL(const char* filename);

void Render_Menu(GLuint bgTex, GLuint atlasTex,bool new_game_selected);