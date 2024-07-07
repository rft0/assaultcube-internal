#pragma once

#ifndef OFFSETS_H
#define OFFSETS_H

#include <stdint.h>
#include "umath.h"

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

static const ptrdiff_t m_oFIntersectGeometry = 0xCCA80;
static const ptrdiff_t m_oGetCrossHairEnt = 0x5F573;

static const ptrdiff_t m_oViewMatrix = 0x17DFD0;

static const ptrdiff_t m_oLocalPlayer = 0x18AC00;

static const ptrdiff_t m_oEntityList = 0x18AC04;
static const ptrdiff_t m_oEntityCount = 0x18AC0C;
static const ptrdiff_t m_oGameMode = 0x18ABF8;

static const ptrdiff_t m_oScreenWidth = 0x191ED8;
static const ptrdiff_t m_oScreenHeight = 0x191EDC;

typedef enum WeaponID {
    WEAPON_KNIFE = 0,
    WEAPON_PISTOL = 1,
    WEAPON_CARBINE = 2,
    WEAPON_SHOTGUN = 3,
    WEAPON_SUBGUN = 4,
    WEAPON_SNIPER = 5,
    WEAPON_ASSAULT = 6,
    WEAPON_GRENADE = 7,
    WEAPON_AKIMBO = 8,
} WeaponID;

typedef struct CWeapon CWeapon;
typedef struct CPlayer CPlayer;

typedef struct CPlayer {
    union {
        DEFINE_MEMBER_N(Vec3, m_vecPosHead, 0x4);
        DEFINE_MEMBER_N(Vec3, m_vecPosBody, 0x28);
        DEFINE_MEMBER_N(Angle, m_vecAngles, 0x34);
        DEFINE_MEMBER_N(int32_t, m_iHealth, 0xEC);
        DEFINE_MEMBER_N(int32_t, m_iArmor, 0xF0);
        DEFINE_MEMBER_N(int32_t, m_iPistolReserve, 0x108);
        DEFINE_MEMBER_N(int32_t, m_iRifleReserve, 0x11C);
        DEFINE_MEMBER_N(int32_t, m_iPistolAmmo, 0x12C);
        DEFINE_MEMBER_N(int32_t, m_iRifleAmmo, 0x140);
        DEFINE_MEMBER_N(int32_t, m_iGrenadeCount, 0x144);
        DEFINE_MEMBER_N(bool, m_bAttacking, 0x204);
        DEFINE_MEMBER_N(char, m_cName[16], 0x205);
        DEFINE_MEMBER_N(unsigned char, m_iTeam, 0x30C);
        DEFINE_MEMBER_N(CWeapon*, m_pCurrentWeapon, 0x364); // 0x364 = animation end, 0x368 = animation start
    };
} CPlayer;

typedef struct CWeapon{
    union {
        DEFINE_MEMBER_N(WeaponID, m_iWeaponID, 0x4);
        DEFINE_MEMBER_N(CPlayer*, m_pOwner, 0x10);
        DEFINE_MEMBER_N(uintptr_t, m_pSecondaryAmmoPtr, 0x10);
        DEFINE_MEMBER_N(uintptr_t, m_pPrimaryAmmoPtr, 0x14);
        DEFINE_MEMBER_N(int32_t, m_iBoredBullets, 0x1C);
        DEFINE_MEMBER_N(bool, m_bReloading, 0x20); // Suspicious
    };
} CWeapon;

#endif