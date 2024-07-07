#pragma once

#ifndef MATH_H
#define MATH_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdbool.h>

typedef struct ViewMatrix {
	float m[16];
} ViewMatrix;

typedef struct Vec2 {
	float x, y;
} Vec2;

typedef struct Vec3 {
	float x, y, z;
} Vec3;

typedef struct Vec4 {
	float x, y, z, w;
} Vec4;

typedef struct Angle {
	float yaw, pitch;
} Angle;

bool Math_WorldToScreen(Vec3 pos, Vec2* screen);
float Math_Distance(Vec2 pos1, Vec2 pos2);
float Math_Distance3D(Vec3 pos1, Vec3 pos2);
Angle Math_GetAngle(Vec3 pos1, Vec3 pos2);

#endif
