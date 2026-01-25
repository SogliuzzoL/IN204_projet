#include "Engine/CameraRendering.hpp"

#include <GL/glu.h>
#include <SDL2/SDL_opengl.h>

#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

#include "Engine/Controls.hpp"
#include "Engine/Game_logic.hpp"
#include "Engine/Maze.hpp"
#include "Engine/Menu.hpp"
#include "Network/NetworkClient.hpp"
#include "Network/PacketFactory.hpp"
#include "Network/PacketHandler.hpp"

float __lerp(float current, float target, float factor) {
  return current + factor * (target - current);
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

void draw_enemy(float x, float y, float yaw, GLuint ennemyTex) {
  glPushMatrix();
  glTranslatef(x, y, 1.8f);
  glRotatef(yaw, 0, 0, 1);
  draw_cube(ennemyTex);
  glPopMatrix();
}

void draw_walls(std::vector<wall> walls) {
  glBegin(GL_QUADS);
  glColor3f(1, 1, 1);
  for (long unsigned int i = 0; i < walls.size(); i++) {
    glVertex3f(walls[i].x1, walls[i].y1, 0);
    glVertex3f(walls[i].x1, walls[i].y1, 2);
    glVertex3f(walls[i].x2, walls[i].y2, 0);
    glVertex3f(walls[i].x2, walls[i].y2, 2);
  }
  glEnd();
}

/**
 * Draws a 3D cube with the given texture.
 * @param texture OpenGL texture ID to apply to the cube.
 */
void draw_cube(GLuint texture) {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  glBegin(GL_QUADS);
  glColor3f(1.0f, 1.0f, 1.0f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(0.5f, 0.5f, -0.5f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(0.5f, -0.5f, -0.5f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0.5f, -0.5f, 0.5f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.5f, -0.5f, 0.5f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(0.5f, -0.5f, -0.5f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(0.5f, 0.5f, -0.5f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(0.5f, -0.5f, 0.5f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-0.5f, -0.5f, 0.5f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glEnd();
}

/**
 * Draws a floor tile with the given texture.
 * @param floorTex OpenGL texture ID for the floor.
 */
void draw_floor(GLuint floorTex) {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, floorTex);
  glBegin(GL_QUADS);
  glColor3f(1.0f, 1.0f, 1.0f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0.5f, 0.5f, -0.5f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(0.5f, -0.5f, -0.5f);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void rendering_settings() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Black background
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glShadeModel(GL_SMOOTH);
}

/**
 * Renders the game scene with the player, maze walls, and floor.
 * Sets up perspective projection and renders all visible maze elements.
 * @param p Player object containing position and angle.
 * @param walls Vector of wall objects for collision testing.
 * @param w Window width in pixels.
 * @param h Window height in pixels.
 * @param maze Reference to the maze structure.
 * @param floorTex OpenGL texture ID for floor tiles.
 * @param wallTex OpenGL texture ID for wall blocks.
 */
template <size_t Size>
void Render(player p, std::vector<wall> walls, int w, int h, Maze<Size>& maze,
            GLuint floorTex, GLuint wallTex) {
  (void)walls;  // intentionally unused
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_ALPHA_TEST);
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float aspect = (float)w / h;
  gluPerspective(45.0f, aspect, 0.01f, 500.0f);  // Larger far plane

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(-p.angle, 0.0f, 0.0f, 1.0f);
  glTranslatef(-p.x, -p.y, -2.0f);

  glPushMatrix();

  static bool logged = false;
  int cubeCount = 0;
  for (uint32_t y = 0; y < 3 * Size; ++y) {
    for (uint32_t x = 0; x < 3 * Size; ++x) {
      if (!maze.is_open(y, x)) {
        cubeCount++;
        glPushMatrix();
        glTranslatef((float)x, (float)y, 1.8f);
        draw_cube(wallTex);
        glPopMatrix();
      } else {
        glPushMatrix();
        glTranslatef((float)x, (float)y, 1.8f);
        draw_floor(floorTex);
        glPopMatrix();
      }
    }
  }

  if (!logged) {
    std::cout << "Rendu: " << cubeCount << " cubes dessinés sur "
              << (3 * Size * 3 * Size) << " cases" << std::endl;
    std::cout << "Position joueur: x=" << p.x << " y=" << p.y
              << " angle=" << p.angle << std::endl;
    std::cout << "Viewport: " << w << "x" << h << std::endl;
    logged = true;
  }

  glPopMatrix();
}

/**
 * Main rendering loop for the game.
 * Handles input, network communication, maze initialization, and rendering.
 * @param ctx SDL OpenGL context.
 * @param frames Pointer to frame counter.
 * @param window SDL window handle.
 * @param t Player object for local control.
 * @param walls Vector of wall objects for environment.
 * @param client Network client for multiplayer communication, nullptr for solo
 * mode.
 */
void rendering_loop(SDL_GLContext ctx, Uint32* frames, SDL_Window* window,
                    player t, std::vector<wall> walls, NetworkClient* client) {
  bool done = 0;
  Game_State current_state = STATE_MENU;
  int w, h;

  std::vector<Entity> otherPlayers;
  UDPpacket* inputPacket = SDLNet_AllocPacket(512);
  uint8_t myPlayerID = 255;
  float target_x = t.x;
  float target_y = t.y;

  if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) &
        (IMG_INIT_JPG | IMG_INIT_PNG))) {
    printf("IMG_Init Error: %s\n", IMG_GetError());
  }

  GLuint menuBG = LoadTextureSDL("Assets/menu_back.jpg");
  GLuint logo = LoadTextureSDL("Assets/doom_menu_buttons.png");
  GLuint floorTex = LoadTextureSDL("Assets/floor.png");
  GLuint wallTex = LoadTextureSDL("Assets/wallTex.jpg");
  GLuint ennemyTex = LoadTextureSDL("Assets/ennemyTex.jpg");
  bool sel = true;

  uint32_t mazeSeed = 0;
  uint32_t receivedSeed = 0;
  bool mazeInitialized = false;
  Maze<10> maze;
  uint32_t statePlayingStartTime = 0;

  while (!done) {
    ++(*frames);
    SDL_GL_MakeCurrent(window, ctx);
    SDL_GetWindowSize(window, &w, &h);

    Game_State prev_state = current_state;
    uint8_t buttons = check_events(&done, &t, &current_state, &sel);

    static bool loggedStateChange = false;
    if (prev_state != current_state && !loggedStateChange) {
      std::cout << "État changé: " << (int)prev_state << " -> "
                << (int)current_state << std::endl;
      loggedStateChange = true;
    }

    if (prev_state != STATE_PLAYING && current_state == STATE_PLAYING &&
        !mazeInitialized) {
      statePlayingStartTime = SDL_GetTicks();
      std::cout << "Initialisation du maze: client=" << (client ? "oui" : "non")
                << std::endl;
      if (!client) {
        // Solo mode: use random seed
        std::random_device rd;
        mazeSeed = rd();
        set_maze_seed(mazeSeed);
        maze.n_shifts(10000);
        mazeInitialized = true;
        std::cout << "Mode solo - Maze initialisé avec seed: " << mazeSeed
                  << std::endl;
      } else {
        std::cout << "Mode multijoueur - En attente de la seed du serveur..."
                  << std::endl;
      }
    }

    if (client && current_state == STATE_PLAYING && !mazeInitialized &&
        SDL_GetTicks() - statePlayingStartTime > 500) {
      std::cout << "Timeout serveur - Initialisation en mode solo..."
                << std::endl;
      std::random_device rd;
      mazeSeed = rd();
      set_maze_seed(mazeSeed);
      maze.n_shifts(10000);
      mazeInitialized = true;
    }

    if (client && current_state == STATE_PLAYING) {
      InputPacket* pkt = (InputPacket*)inputPacket->data;
      pkt->header.type = PACKET_TYPE_INPUT;
      pkt->header.sequence = *frames;
      pkt->playerId = myPlayerID;
      pkt->yaw = t.angle;
      pkt->pitch = 0;
      pkt->inputButtons = buttons;
      inputPacket->len = sizeof(InputPacket);
      client->sendData(inputPacket->data, inputPacket->len);

      while (client->handleIncomingData() > 0) {
        UDPpacket* p = client->getPacket();
        PacketHandler::processClientPacket(p->data, p->len, otherPlayers,
                                           myPlayerID, receivedSeed);
      }

      if (receivedSeed != 0 && receivedSeed != mazeSeed) {
        mazeSeed = receivedSeed;
        set_maze_seed(mazeSeed);
        maze.n_shifts(10000);
        mazeInitialized = true;
        std::cout << "Maze mis à jour avec seed du serveur: " << mazeSeed
                  << std::endl;
      }

      if (myPlayerID != 255) {
        for (const auto& entity : otherPlayers) {
          if (entity.id == myPlayerID) {
            target_x = entity.x;
            target_y = entity.y;
            float dist = (t.x - target_x) * (t.x - target_x) +
                         (t.y - target_y) * (t.y - target_y);
            if (dist > 5.0f) {
              t.x = target_x;
              t.y = target_y;
            }
            break;
          }
        }
      }
    }

    if (current_state == STATE_PLAYING) {
      if (!client || !mazeInitialized) {
        float rad = t.angle * (3.14159f / 180.0f);
        float dirX = -sin(rad);
        float dirY = cos(rad);
        float moveSpeed = 0.05f;

        if (buttons & INPUT_FORWARD) {
          t.x += dirX * moveSpeed;
          t.y += dirY * moveSpeed;
        }
        if (buttons & INPUT_BACKWARD) {
          t.x -= dirX * moveSpeed;
          t.y -= dirY * moveSpeed;
        }
        if (buttons & INPUT_LEFT) {
          t.x -= dirY * moveSpeed;
          t.y += dirX * moveSpeed;
        }
        if (buttons & INPUT_RIGHT) {
          t.x += dirY * moveSpeed;
          t.y -= dirX * moveSpeed;
        }
      }

      if (client && mazeInitialized) {
        float smoothFactor = 0.2f;
        t.x = __lerp(t.x, target_x, smoothFactor);
        t.y = __lerp(t.y, target_y, smoothFactor);
      }
    }

    glViewport(0, 0, w, h);

    switch (current_state) {
      case STATE_MENU:
        glViewport(0, 0, w, h);
        Render_Menu(menuBG, logo, sel);
        break;

      case STATE_PLAYING:
        if (mazeInitialized) {
          Render(t, walls, w, h, maze, floorTex, wallTex);
          for (const auto& entity : otherPlayers) {
            if (entity.id != myPlayerID) {
              draw_enemy(entity.x, entity.y, entity.yaw, ennemyTex);
            }
          }
        } else {
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        break;

      case STATE_PAUSED:
        // Render game in background and pause overlay
        if (mazeInitialized) {
          Render(t, walls, w, h, maze, floorTex, wallTex);
        }
        break;
    }

    SDL_GL_SwapWindow(window);
  }

  if (inputPacket) SDLNet_FreePacket(inputPacket);
}