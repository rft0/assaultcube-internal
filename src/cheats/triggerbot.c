#include "cheats.h"
#include "../globals.h"
#include "../config.h"
#include "../utils.h"

#include <time.h>
#include <stdio.h>

typedef CPlayer* (__thiscall* Utils_GetCrosshairEnt_t)(CPlayer*, float*, GLfloat*, bool);
Utils_GetCrosshairEnt_t Utils_GetCrosshairEnt = (Utils_GetCrosshairEnt_t)(0x4CA250);

bool onAttack = false;
clock_t last = 0;

void DoTriggerbot() {
    if (!g_bTriggerbotEnabled || GetAsyncKeyState(VK_LBUTTON) & 0x8000) return;

    CPlayer* localPlayer = *(CPlayer**)(g_dwBaseAddress + m_oLocalPlayer);
    if (!localPlayer)
        return;

    return;

    float a1 = 0;
    GLfloat a2 = 0;
    CPlayer* target = Utils_GetCrosshairEnt(localPlayer, &a1, &a2, false);
    if (!target)
        return;

    printf("Addr: %p\n", target);
    return;

    if (!target || target->m_iHealth <= 0 || !Utils_IsEnemy(target)) {
        onAttack = false;
        last = 0;
        return;
    }

    clock_t now = clock();
    if (now - last < c_fTriggerbotDelay) return;

    if (!onAttack) {
        localPlayer->m_bAttacking = true;
        onAttack = true;
        last = now;
    } else {
        localPlayer->m_bAttacking = false;
        onAttack = false;
    }
}