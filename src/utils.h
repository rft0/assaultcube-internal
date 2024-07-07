#pragma once

#ifndef UTILS_H
#define UTILS_H

// template <size_t Index, typename TRet = void, typename... ArgList>
// __forceinline TRet CallVirtual(void* instance, ArgList... args)
// {
//     using Fn = TRet(__thiscall*)(void*, ArgList...);
    
//     auto function = (*reinterpret_cast<Fn**>(instance))[Index];
//     return function(instance, args...);
// }

#include "offsets.h"

#define CALL_VIRTUAL_METHOD(type, this, index, ...) (((type)(((PUINT*)(this))[0][index]))(this, 0, __VA_ARGS__));

void Utils_GetCrossHairEnt();
void Utils_TraceLine(Vec3* from, Vec3* to);
bool Utils_IsVisible(CPlayer* target);
bool Utils_IsEnemy(CPlayer* player);
CPlayer* Utils_GetClosestEnemy(bool visibleOnly);

#endif