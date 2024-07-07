#include "cheats.h"

#include "../offsets.h"
#include "../globals.h"
#include "../utils.h"
#include "../umath.h"

void DoAimbot() {
    if (!g_bAimbotEnabled) return;

    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
        CPlayer* localPlayer = *(CPlayer**)(g_dwBaseAddress + m_oLocalPlayer);
        if (!localPlayer)
            return;

        CPlayer* target = Utils_GetClosestEnemy(true);
        if (!target)
            return;

        Angle angles = Math_GetAngle(localPlayer->m_vecPosHead, target->m_vecPosHead);
        localPlayer->m_vecAngles.pitch = angles.pitch;
        localPlayer->m_vecAngles.yaw = angles.yaw;
    }
}