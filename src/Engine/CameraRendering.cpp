#include "Engine/CameraRendering.hpp"

#include <GL/glu.h>
#include <SDL2/SDL_opengl.h>

#include <vector>

#include "Engine/Controls.hpp"
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
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3f(-0.5f, -0.5f, 0.5f);
  glVertex3f(0.5f, -0.5f, 0.5f);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  // Back Face (Z = -0.5)
  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glVertex3f(0.5f, 0.5f, -0.5f);
  glVertex3f(0.5f, -0.5f, -0.5f);
  // Top Face (Y = 0.5)
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glVertex3f(0.5f, 0.5f, -0.5f);
  // Bottom Face (Y = -0.5)
  glColor3f(1.0f, 1.0f, 0.0f);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glVertex3f(0.5f, -0.5f, -0.5f);
  glVertex3f(0.5f, -0.5f, 0.5f);
  glVertex3f(-0.5f, -0.5f, 0.5f);
  // Right face (X = 0.5)
  glColor3f(1.0f, 0.0f, 1.0f);
  glVertex3f(0.5f, -0.5f, -0.5f);
  glVertex3f(0.5f, 0.5f, -0.5f);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glVertex3f(0.5f, -0.5f, 0.5f);
  // Left Face (X = -0.5)
  glColor3f(0.0f, 1.0f, 1.0f);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glVertex3f(-0.5f, -0.5f, 0.5f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glEnd();
}

void rendering_settings() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glShadeModel(GL_SMOOTH);
}

void Render(player p, std::vector<wall> walls, int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float aspect = (float)w / h;
  gluPerspective(45.0f, aspect, 0.1f, 100.0f);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(-p.angle, 0.0f, 0.0f, 1.0f);
  glTranslatef(-p.x, -p.y, -2.0f);

  glPushMatrix();
  glTranslatef(2.0f, 5.0f, 0.5f);
  draw_cube();
  glPopMatrix();
}

void rendering_loop(SDL_GLContext ctx, Uint32* frames, SDL_Window* window,
                    player t, std::vector<wall> walls, NetworkClient* client) {
  bool done = 0;
  std::vector<Entity> otherPlayers;
  UDPpacket* inputPacket = SDLNet_AllocPacket(512);

  uint8_t myPlayerID = 255;
  float target_x = t.x;
  float target_y = t.y;

  while (!done) {
    ++(*frames);
    uint8_t buttons = check_events(&done, &t);

    if (client) {
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
                                           myPlayerID);
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
    float smoothFactor = 0.2f;
    t.x = __lerp(t.x, target_x, smoothFactor);
    t.y = __lerp(t.y, target_y, smoothFactor);

    int w, h;
    SDL_GL_MakeCurrent(window, ctx);
    SDL_GetWindowSize(window, &w, &h);
    glViewport(0, 0, w, h);

    Render(t, walls, w, h);

    for (const auto& entity : otherPlayers) {
      if (entity.id != myPlayerID) {
        draw_enemy(entity.x, entity.y, entity.yaw);
      }
    }
    SDL_GL_SwapWindow(window);
  }

  if (inputPacket) SDLNet_FreePacket(inputPacket);
}