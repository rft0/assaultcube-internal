#pragma once

#ifndef MEM_H
#define MEM_H

#include <Windows.h>
#include <stdbool.h>

void Mem_Patch(BYTE* dst, BYTE* src, int len);

bool Mem_Detour(BYTE* src, BYTE* dst, const uintptr_t len);
BYTE* Mem_TrampHook(BYTE* src, BYTE* dst, const uintptr_t len);

#endif
