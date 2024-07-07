#pragma once

#ifndef GL_UTILS_H
#define GL_UTILS_H

#include <Windows.h>
#include <stdbool.h>
#include <GL/gl.h>

#define NORM_RGB(r, g, b)           (float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f
#define NORM_RGBA(r, g, b, a)       (float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, (float)a / 255.0f

#define COLOR3(r, g, b)             (Color3){ r, g, b }
#define COLOR4(r, g, b, a)          (Color4){ r, g, b, a }

#define COLOR3_WHITE                COLOR3(1, 1, 1)
#define COLOR3_BLACK                COLOR3(0, 0, 0)
#define COLOR3_RED                  COLOR3(1, 0, 0)
#define COLOR3_GREEN                COLOR3(0, 1, 0)
#define COLOR3_BLUE                 COLOR3(0, 0, 1)

#define COLOR4_WHITE                COLOR4(1, 1, 1, 1)
#define COLOR4_BLACK                COLOR4(0, 0, 0, 1)
#define COLOR4_RED                  COLOR4(1, 0, 0, 1)
#define COLOR4_GREEN                COLOR4(0, 1, 0, 1)
#define COLOR4_BLUE                 COLOR4(0, 0, 1, 1)

typedef struct Color3 {
    float r, g, b;
} Color3;

typedef struct Color4 {
    float r, g, b, a;
} Color4;

void GL_SetupOrtho(void);
void GL_RestoreOrtho(void);

void GL_Print(int x, int y, Color4 color, const char *fmt, ...);

void GL_DrawRectangle(float x1, float y1, float x2, float y2, float thickness, Color4 color);
void GL_DrawFilledRectangle(float x1, float y1, float x2, float y2, float thickness, Color4 color);

void GL_DrawLine(float x1, float y1, float x2, float y2, Color4 color);

#endif