#include "cheats.h"

#include "../gl_utils.h"
#include "../offsets.h"
#include "../globals.h"
#include "../config.h"
#include "../utils.h"
#include "../umath.h"

void DoEsp() {
    if (!g_bEspEnabled) return;

    CPlayer* localPlayer = *(CPlayer**)(g_dwBaseAddress + m_oLocalPlayer);
    if (!localPlayer) return;

    uintptr_t entityList = *(uintptr_t*)(g_dwBaseAddress + m_oEntityList);
    if (!entityList) return;

    int entityCount = *(int*)(g_dwBaseAddress + m_oEntityCount);
    for (int i = 0; i < entityCount; i++) {
        CPlayer* player = *(CPlayer**)(entityList + i * 4);
        if (!player || player == localPlayer || player->m_iHealth <= 0) continue;

        bool isEnemy = Utils_IsEnemy(player);
        bool isVisible = Utils_IsVisible(player);

        Vec3 vBody = player->m_vecPosBody;
        Vec3 vHead = player->m_vecPosHead;

        Vec2 vBodyScreen;
        Vec2 vHeadScreen;

        float sx = (float)*(int*)(g_dwBaseAddress + m_oScreenWidth);
        float sy = (float)*(int*)(g_dwBaseAddress + m_oScreenHeight);
        if (Math_WorldToScreen(vBody, &vBodyScreen) && Math_WorldToScreen(vHead, &vHeadScreen)) {
            float h = vBodyScreen.y - vHeadScreen.y;
            float w = h / 3.0f;
            float d = h / 8.0f;

            // Print player name
            GL_Print(vHeadScreen.x - w, vHeadScreen.y - h / 5.0f, isEnemy? c_ESPRectangleNameColorEnemy : c_ESPRectangleNameColorFriend, player->m_cName);

            // Outline of rectangle, RED = enemy, GREEN = friendly
            GL_DrawRectangle(vHeadScreen.x - w, vHeadScreen.y - h / 6.0f, vBodyScreen.x + w, vBodyScreen.y + d, 4.0f, isEnemy? c_ESPRectangleOutlineColorEnemy : c_ESPRectangleOutlineColorFriend);

            GL_DrawFilledRectangle(vHeadScreen.x - w, vHeadScreen.y - h / 6.0f, vBodyScreen.x + w, vBodyScreen.y + d, 1.0f, isEnemy? (isVisible? c_ESPRectangleVisibleFillColorEnemy : c_ESPRectangleFillColorEnemy) : (isVisible? c_ESPRectangleVisibleFillColorFriend : c_ESPRectangleFillColorFriend));

            GL_DrawLine(sx / 2, sy, vBodyScreen.x, vBodyScreen.y + d, isEnemy? c_ESPLineColorEnemy : c_ESPLineColorFriend);
        }
    }
}