
#include "utils.h"
#include "globals.h"
#include "umath.h"
#include "config.h"

// Not completely a traceline function but it does the job.
// Value of "to" changes if line intersects with any geometry.
void Utils_TraceLine(Vec3* from, Vec3* to) {
    DWORD fIntersectGeometry = (DWORD)(g_dwBaseAddress + m_oFIntersectGeometry);

    void* pFrom = (void*)from;
    void* pTo = (void*)to;

    __asm {
        mov ecx, pFrom
        mov edx, pTo
        call fIntersectGeometry
    }
}

bool Utils_IsVisible(CPlayer* target) {
    if (!target)
        return false;

    CPlayer* localPlayer = *(CPlayer**)(g_dwBaseAddress + m_oLocalPlayer);
    if (!localPlayer || target == localPlayer)
        return false;

    // Vec3 tmp = target->m_vecPosHead.x >= 0 ? target->m_vecPosHead : target->m_vecPosBody;
    Vec3 tmp = target->m_vecPosHead;

    Vec3 from = localPlayer->m_vecPosBody;
    Vec3 to = (Vec3){ tmp.x, tmp.y, tmp.z };

    Utils_TraceLine(&from, &to);
    return to.x == tmp.x && to.y == tmp.y && to.z == tmp.z;
}

bool Utils_IsEnemy(CPlayer* player) {
    CPlayer* localPlayer = *(CPlayer**)(g_dwBaseAddress + m_oLocalPlayer);
    if (!localPlayer || player == localPlayer)
        return false;

    int gameMode = *(int*)(g_dwBaseAddress + m_oGameMode);
    bool isTeamMode = gameMode == 0 || gameMode == 4 || gameMode == 5 || gameMode == 7
        || gameMode == 11 || gameMode == 13 || gameMode == 14 || gameMode == 16
        || gameMode == 17 || gameMode == 20 || gameMode == 21;

    return !isTeamMode || player->m_iTeam != localPlayer->m_iTeam;
}

CPlayer* Utils_GetClosestEnemy(bool visibleOnly) {
    CPlayer* localPlayer = *(CPlayer**)(g_dwBaseAddress + m_oLocalPlayer);
    if (!localPlayer)
        return NULL;

    uintptr_t entityList = *(uintptr_t*)(g_dwBaseAddress + m_oEntityList);
    if (!entityList)
        return NULL;

    CPlayer* target = NULL;
    int bestDist = 9999; // inf

    int entityCount = *(int*)(g_dwBaseAddress + m_oEntityCount);
    for (int i = 0; i < entityCount; i++) {
        CPlayer* player = *(CPlayer**)(entityList + i * 4);
        if (!player || player == localPlayer || player->m_iHealth <= 0) continue;

        if (!Utils_IsEnemy(player))
            continue;

        if (visibleOnly && !Utils_IsVisible(player))
            continue;

        Vec2 vScreen;
        if (!Math_WorldToScreen(player->m_vecPosHead, &vScreen))
            continue;

        int screenWidth = *(int*)(g_dwBaseAddress + m_oScreenWidth);
        int screenHeight = *(int*)(g_dwBaseAddress + m_oScreenHeight);

        float dist = Math_Distance((Vec2){ (float)screenWidth / 2, (float)screenHeight / 2 }, vScreen);
        if (dist < c_fAimbotFov && dist < bestDist) {
            target = player;
        }
    }

    return target;
}