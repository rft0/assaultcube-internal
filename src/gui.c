#include "gl_utils.h"
#include "globals.h"

#include "config.h"

void GUI_Draw() {
    // I am too lazy to make a proper GUI so I will just print some text, just use cfg for customization.
    GL_Print(16, 24, COLOR4_WHITE, "[0] Reload Config");
    GL_Print(16, 48, g_bTriggerbotEnabled? COLOR4_GREEN : COLOR4_RED ,"[1] Trigger Bot: %s", g_bTriggerbotEnabled ? "ON" : "OFF");
    GL_Print(16, 72, g_bAimbotEnabled? COLOR4_GREEN : COLOR4_RED,"[2] Aim Bot: %s", g_bAimbotEnabled ? "ON" : "OFF");
    GL_Print(16, 96, g_bEspEnabled? COLOR4_GREEN : COLOR4_RED,"[3] ESP: %s", g_bEspEnabled ? "ON" : "OFF");
}