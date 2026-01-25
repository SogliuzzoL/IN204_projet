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

void draw_enemy(float x, float y, float yaw) {
  glPushMatrix();
  glTranslatef(x, y, 0);
  glRotatef(yaw, 0, 0, 1);
  draw_cube();
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

void draw_cube() {
  glBegin(GL_QUADS);
  // Front Face (Z = 0.5)
  glColor3f(1.0f, 0.0f, 0.0f);  // Red
  glVertex3f(-0.5f, -0.5f, 0.5f);
  glVertex3f(0.5f, -0.5f, 0.5f);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  // Back Face (Z = -0.5)
  glColor3f(0.0f, 1.0f, 0.0f);  // Green
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glVertex3f(0.5f, 0.5f, -0.5f);
  glVertex3f(0.5f, -0.5f, -0.5f);
  // Top Face (Y = 0.5)
  glColor3f(0.0f, 0.0f, 1.0f);  // Blue
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glVertex3f(0.5f, 0.5f, -0.5f);
  // Bottom Face (Y = -0.5)
  glColor3f(1.0f, 1.0f, 0.0f);  // Yellow
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glVertex3f(0.5f, -0.5f, -0.5f);
  glVertex3f(0.5f, -0.5f, 0.5f);
  glVertex3f(-0.5f, -0.5f, 0.5f);
  // Right face (X = 0.5)
  glColor3f(1.0f, 0.0f, 1.0f);  // Magenta
  glVertex3f(0.5f, -0.5f, -0.5f);
  glVertex3f(0.5f, 0.5f, -0.5f);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glVertex3f(0.5f, -0.5f, 0.5f);
  // Left Face (X = -0.5)
  glColor3f(0.0f, 1.0f, 1.0f);  // Cyan
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glVertex3f(-0.5f, -0.5f, 0.5f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glEnd();
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

template <size_t Size>
void Render(player p, std::vector<wall> walls, int w, int h, Maze<Size>& maze) {
  // Reset OpenGL state from menu
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_ALPHA_TEST);
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // 1. Perspective Setup
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
        glTranslatef((float)x, (float)y, 1.f);
        draw_cube();
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

void rendering_loop(SDL_GLContext ctx, Uint32* frames, SDL_Window* window,
                    player t, std::vector<wall> walls, NetworkClient* client) {
  bool done = 0;
  Game_State current_state = STATE_MENU;
  int w, h;

  // Network and local player data
  std::vector<Entity> otherPlayers;
  UDPpacket* inputPacket = SDLNet_AllocPacket(512);
  uint8_t myPlayerID = 255;
  float target_x = t.x;
  float target_y = t.y;

  // Load textures for menu
  if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) &
        (IMG_INIT_JPG | IMG_INIT_PNG))) {
    printf("IMG_Init Error: %s\n", IMG_GetError());
  }

  GLuint menuBG = LoadTextureSDL("Assets/menu_back.jpg");
  GLuint logo = LoadTextureSDL("Assets/doom_menu_buttons.png");
  bool sel = true;  // true = New Game, false = Quit

  // Maze will be initialized once we receive seed from server
  uint32_t mazeSeed = 0;
  bool mazeInitialized = false;
  Maze<10> maze;

  while (!done) {
    ++(*frames);
    SDL_GL_MakeCurrent(window, ctx);
    SDL_GetWindowSize(window, &w, &h);

    // Get input
    Game_State prev_state = current_state;
    uint8_t buttons = check_events(&done, &t, &current_state, &sel);

    // Initialize maze when starting game (if not already done)
    if (prev_state != STATE_PLAYING && current_state == STATE_PLAYING &&
        !mazeInitialized) {
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

    // Handle network communication (only when playing)
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

      uint32_t receivedSeed = 0;
      while (client->handleIncomingData() > 0) {
        UDPpacket* p = client->getPacket();
        PacketHandler::processClientPacket(p->data, p->len, otherPlayers,
                                           myPlayerID, receivedSeed);
      }

      // Initialize maze when we receive seed from server
      if (receivedSeed != 0 && !mazeInitialized) {
        mazeSeed = receivedSeed;
        set_maze_seed(mazeSeed);
        maze.n_shifts(10000);
        mazeInitialized = true;
        std::cout << "Maze initialisé avec seed du serveur: " << mazeSeed
                  << std::endl;
      }

      // Update player position from server
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

    // Smooth player movement (only when playing)
    if (current_state == STATE_PLAYING) {
      float smoothFactor = 0.2f;
      t.x = __lerp(t.x, target_x, smoothFactor);
      t.y = __lerp(t.y, target_y, smoothFactor);
    }

    glViewport(0, 0, w, h);

    // Render based on game state
    switch (current_state) {
      case STATE_MENU:
        glViewport(0, 0, w, h);
        Render_Menu(menuBG, logo, sel);
        break;

      case STATE_PLAYING:
        if (mazeInitialized) {
          Render(t, walls, w, h, maze);
          for (const auto& entity : otherPlayers) {
            if (entity.id != myPlayerID) {
              draw_enemy(entity.x, entity.y, entity.yaw);
            }
          }
        } else {
          // Afficher un écran de chargement en attendant la seed
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        break;

      case STATE_PAUSED:
        // Render game in background and pause overlay
        if (mazeInitialized) {
          Render(t, walls, w, h, maze);
        }
        break;
    }

    SDL_GL_SwapWindow(window);
  }

  if (inputPacket) SDLNet_FreePacket(inputPacket);
}