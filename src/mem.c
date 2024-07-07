#include "mem.h"

void Mem_Patch(BYTE* dst, BYTE* src, int len) {
    DWORD oldProtection;
    VirtualProtect(dst, len, PAGE_EXECUTE_READWRITE, &oldProtection);

    memcpy(dst, src, len);

    VirtualProtect(dst, len, oldProtection, &oldProtection);
}

bool Mem_Detour(BYTE *src, BYTE *dst, const uintptr_t len) {
    if (len < 5) return false;

    DWORD oldProtection;
    VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProtection);

    uintptr_t relativeAddress = dst - src - 5;

    *src = 0xE9;
    *(uintptr_t*)(src + 1) = relativeAddress;

    VirtualProtect(src, len, oldProtection, &oldProtection);
    return true;
}

BYTE* Mem_TrampHook(BYTE *src, BYTE *dst, const uintptr_t len) {
    if (len < 5) return 0;

    BYTE *gateway = (BYTE *)VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    memcpy(gateway, src, len);

    uintptr_t gatewayRelativeAddr = src - gateway - 5;
    *(gateway + len) = 0xE9;
    *(uintptr_t*)(gateway + len + 1) = gatewayRelativeAddr;

    Mem_Detour(src, dst, len);
    return gateway;
}