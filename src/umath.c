#include "umath.h"
#include "offsets.h"
#include "globals.h"
#include <stdio.h>

bool Math_WorldToScreen(Vec3 pos, Vec2* screen) {
    float* matrix = (float*)(g_dwBaseAddress + m_oViewMatrix);

	Vec4 clipCoords;
	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
	clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
	clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
	clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;

    float mx = (float)*(int*)(g_dwBaseAddress + m_oScreenWidth) / 2.0f;
    int my = (float)*(int*)(g_dwBaseAddress + m_oScreenHeight) / 2.0f;

	screen->x = (mx * NDC.x) + (NDC.x + mx);
	screen->y = -(my * NDC.y) + (NDC.y + my);
	return true;
}

float Math_Distance(Vec2 pos1, Vec2 pos2) {
    Vec3 difference;
    difference.x = pos1.x - pos2.x;
    difference.y = pos1.y - pos2.y;

    return sqrtf(difference.x * difference.x + difference.y * difference.y);
}

float Math_Distance3D(Vec3 pos1, Vec3 pos2) {
    return sqrtf(powf(pos2.x - pos1.x, 2) + powf(pos2.y - pos1.y, 2) + powf(pos2.z - pos1.z, 2));
}

Angle Math_GetAngle(Vec3 pos1, Vec3 pos2) {
    Angle result;
    result.yaw = -atan2f(pos2.x - pos1.x, pos2.y - pos1.y) / M_PI * 180.f + 180.f;
    result.pitch = asinf((pos2.z - pos1.z) / Math_Distance3D(pos1, pos2)) * 180 / M_PI;

    return result;
}

// float Matrix_TransformX(float* matrix, Vec3 pos) {
//     return pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
// }

// float Matrix_TransformY(float* matrix, Vec3 pos) {
//     return pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
// }

// float Matrix_TransformZ(float* matrix, Vec3 pos) {
//     return pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
// }

// float Matrix_TransformW(float* matrix, Vec3 pos) {
//     return pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];
// }