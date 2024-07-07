#include "gl_utils.h"
#include <stdio.h>
#include <stdarg.h>

static bool bFontsBuild = 0;
static GLuint uFontBase = 0;

void GL_SetupOrtho() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(0, 0, viewport[2], viewport[3]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
}

void GL_RestoreOrtho() {
    glPopMatrix();
    glPopAttrib();
}

void GL_BuildFonts()
{
    HDC hDC = wglGetCurrentDC();

    uFontBase = glGenLists(96);

    HFONT hFont = CreateFont(
        -16,               // Height of font
        0,                 // Width of font
        0,                 // Angle of escapement
        0,                 // Orientation angle
        FW_BOLD,           // Font weight
        FALSE,             // Italic
        FALSE,             // Underline
        FALSE,             // Strikeout
        ANSI_CHARSET,      // Character set identifier
        OUT_TT_PRECIS,     // Output precision
        CLIP_DEFAULT_PRECIS,// Clipping precision
        ANTIALIASED_QUALITY,// Output quality
        FF_DONTCARE | DEFAULT_PITCH, // Family and pitch
        L"Courier New");    // Font face

    HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

    wglUseFontBitmaps(hDC, 32, 96, uFontBase);

    SelectObject(hDC, hOldFont);

    DeleteObject(hFont);

    bFontsBuild = true;
}

void GL_Print(int x, int y, Color4 color, const char *fmt, ...)
{
    if (!bFontsBuild) GL_BuildFonts();

    if (fmt == NULL) return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(color.r, color.g, color.b, color.a);
    glRasterPos2i(x, y);

    char text[256];
    va_list ap;

    va_start(ap, fmt);
    vsprintf_s(text, sizeof(text), fmt, ap); // Ensure buffer size
    va_end(ap);

    glPushAttrib(GL_LIST_BIT);
    glListBase(uFontBase - 32);
    glCallLists((GLsizei)strlen(text), GL_UNSIGNED_BYTE, (GLvoid*)text);
    glPopAttrib();

    glDisable(GL_BLEND);
}

void GL_DrawRectangle(float x1, float y1, float x2, float y2, float thickness, Color4 color) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(color.r, color.g, color.b, color.a);

    glLineWidth(thickness);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();

    glDisable(GL_BLEND);
}

void GL_DrawFilledRectangle(float x1, float y1, float x2, float y2, float thickness, Color4 color) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(color.r, color.g, color.b, color.a);

    glLineWidth(thickness);
    glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();

    glDisable(GL_BLEND);
}

void GL_DrawLine(float x1, float y1, float x2, float y2, Color4 color) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glLineWidth(1.0f);
    glColor4f(color.r, color.g, color.b, color.a);

    glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    glEnd();

    glDisable(GL_BLEND);
}