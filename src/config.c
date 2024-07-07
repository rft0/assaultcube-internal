#include <Windows.h>
#include <stdio.h>
#include <ShlObj.h>
#include <io.h>

#include "thirdparty/cJSON.h"
#include "config.h"
#include "globals.h"

#define ASSERT(message) { MessageBoxA(NULL, message, "Error", MB_OK); CreateThread(NULL, 0, EjectThread, NULL, 0, NULL); }

extern DWORD WINAPI EjectThread(LPVOID lpParameter);

float c_fTriggerbotDelay = 0.0f;
float c_fAimbotFov = 100.0f;

Color4 c_ESPRectangleOutlineColorFriend = {0.0f, 255.0f, 0.0f, 240.0f};
Color4 c_ESPRectangleOutlineColorEnemy = {255.0f, 0.0f, 0.0f, 240.0f};
Color4 c_ESPRectangleFillColorFriend = {0.0f, 255.0f, 0.0f, 128.0f};
Color4 c_ESPRectangleFillColorEnemy = {255.0f, 0.0f, 0.0f, 128.0f};
Color4 c_ESPRectangleVisibleFillColorFriend = {0.0f, 255.0f, 0.0f, 64.0f};
Color4 c_ESPRectangleVisibleFillColorEnemy = {255.0f, 0.0f, 0.0f, 64.0f};
Color4 c_ESPRectangleNameColorFriend = {0.0f, 255.0f, 0.0f, 240.0f};
Color4 c_ESPRectangleNameColorEnemy = {255.0f, 0.0f, 0.0f, 240.0f};
Color4 c_ESPLineColorFriend = {0.0f, 255.0f, 0.0f, 240.0f};
Color4 c_ESPLineColorEnemy = {255.0f, 0.0f, 0.0f, 240.0f};

Color4 LoadColor4FromArray(cJSON* jsonArray) {
    Color4 color = {0.0f, 0.0f, 0.0f, 0.0f};

    if (!cJSON_IsArray(jsonArray)) {
        ASSERT("Expected an array for color (config.json)");
        return color;
    }

    if (cJSON_GetArraySize(jsonArray) != 4) {
        ASSERT("Color array must have 4 elements (config.json)");
        return color;
    }

    cJSON* r = cJSON_GetArrayItem(jsonArray, 0);
    cJSON* g = cJSON_GetArrayItem(jsonArray, 1);
    cJSON* b = cJSON_GetArrayItem(jsonArray, 2);
    cJSON* a = cJSON_GetArrayItem(jsonArray, 3);

    if (!r || !cJSON_IsNumber(r) || !g || !cJSON_IsNumber(g) || !b || !cJSON_IsNumber(b) || !a || !cJSON_IsNumber(a)) {
        ASSERT("Syntax error in color array");
        return color;
    }

    // rgba must be between 0 and 255
    if (r->valuedouble < 0 || r->valuedouble > 255 || g->valuedouble < 0 || g->valuedouble > 255 || b->valuedouble < 0 || b->valuedouble > 255 || a->valuedouble < 0 || a->valuedouble > 255) {
        ASSERT("Color values must be between 0 and 255");
        return color;
    }

    color.r = (float)r->valuedouble / 255.0f;
    color.g = (float)g->valuedouble / 255.0f;
    color.b = (float)b->valuedouble / 255.0f;
    color.a = (float)a->valuedouble / 255.0f;

    return color;
}

const char* defaultConfig = "{\n"
    "   \"Aimbot\": {\n"
    "       \"Enabled\": true,\n"
    "       \"FOV\": 100\n"
    "   },\n"
    "   \"Triggerbot\": {\n"
    "       \"Enabled\": true,\n"
    "       \"Delay\": 0\n"
    "   },\n"
    "   \"ESP\": {\n"
    "       \"Enabled\": true,\n"
    "       \"RectangleOutlineColorFriend\": [0, 255, 0, 240],\n"
    "       \"RectangleOutlineColorEnemy\": [255, 0, 0, 240],\n"
    "       \"RectangleFillColorFriend\": [0, 255, 0, 128],\n"
    "       \"RectangleFillColorEnemy\": [255, 0, 0, 128],\n"
    "       \"RectangleVisibleFillColorFriend\": [0, 255, 0, 64],\n"
    "       \"RectangleVisibleFillColorEnemy\": [255, 0, 0, 64],\n"
    "       \"RectangleNameColorFriend\": [0, 255, 0, 240],\n"
    "       \"RectangleNameColorEnemy\": [255, 0, 0, 240],\n"
    "       \"LineColorFriend\": [0, 255, 0, 240],\n"
    "       \"LineColorEnemy\": [255, 0, 0, 240]\n"
    "   }\n"
    "}";

void CreateConfigFile(const char* path) {
    FILE* file = NULL;
    errno_t err = fopen_s(&file, path, "w");
    if (err) {
        ASSERT("Failed to create config.json");
        return;
    }

    fwrite(defaultConfig, 1, strlen(defaultConfig), file);
    fclose(file);
}

void Config_Load() {
    char configPath[MAX_PATH];
    if (SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, 0, configPath) != S_OK) {
        ASSERT("Failed to retrieve Documents folder path for config.json");
        return;
    }

    strcat_s(configPath, MAX_PATH, "\\AssaultCubeInternal");

    if (_access(configPath, 0) != 0) {
        if (!CreateDirectoryA(configPath, NULL)) {
            ASSERT("Failed to create directory for config.json");
            return;
        }
    }
    
    strcat_s(configPath, MAX_PATH, "\\config.json");
    if (_access(configPath, 0) != 0)
        CreateConfigFile(configPath);

    FILE* file = NULL;
    errno_t err = fopen_s(&file, configPath, "r");
    if (err) {
        ASSERT("Failed to open config.json");
        return;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = malloc(size + 1);
    fread(buffer, 1, size, file);
    buffer[size] = 0;

    cJSON* root = cJSON_Parse(buffer);
    if (!root) {
        ASSERT("Failed to parse config.json");
        return;
    }

    cJSON* aimbot = cJSON_GetObjectItem(root, "Aimbot");
    if (aimbot) {
        cJSON* enabled = cJSON_GetObjectItem(aimbot, "Enabled");
        if (enabled)
            g_bAimbotEnabled = cJSON_IsTrue(enabled);

        cJSON* fov = cJSON_GetObjectItem(aimbot, "FOV");
        if (fov || cJSON_IsNumber(fov))
            c_fAimbotFov = fov->valuedouble;
        else
            ASSERT("Syntax error in Aimbot.FOV (config.json)");
    }

    cJSON* triggerbot = cJSON_GetObjectItem(root, "Triggerbot");
    if (triggerbot) {
        cJSON* enabled = cJSON_GetObjectItem(triggerbot, "Enabled");
        if (enabled)
            g_bTriggerbotEnabled = cJSON_IsTrue(enabled);

        cJSON* delay = cJSON_GetObjectItem(triggerbot, "Delay");
        if (delay || cJSON_IsNumber(delay))
            c_fTriggerbotDelay = delay->valuedouble;
        else
            ASSERT("Syntax error in TriggerBot.Delay (config.json)");
    }

    cJSON* esp = cJSON_GetObjectItem(root, "ESP");
    if (esp) {
        cJSON* enabled = cJSON_GetObjectItem(esp, "Enabled");
        if (enabled)
            g_bEspEnabled = cJSON_IsTrue(enabled);

        cJSON* rectangleOutlineColorFriend = cJSON_GetObjectItem(esp, "RectangleOutlineColorFriend");
        if (rectangleOutlineColorFriend)
            c_ESPRectangleOutlineColorFriend = LoadColor4FromArray(rectangleOutlineColorFriend);

        cJSON* rectangleOutlineColorEnemy = cJSON_GetObjectItem(esp, "RectangleOutlineColorEnemy");
        if (rectangleOutlineColorEnemy)
            c_ESPRectangleOutlineColorEnemy = LoadColor4FromArray(rectangleOutlineColorEnemy);

        cJSON* rectangleFillColorFriend = cJSON_GetObjectItem(esp, "RectangleFillColorFriend");
        if (rectangleFillColorFriend)
            c_ESPRectangleFillColorFriend = LoadColor4FromArray(rectangleFillColorFriend);
        
        cJSON* rectangleFillColorEnemy = cJSON_GetObjectItem(esp, "RectangleFillColorEnemy");
        if (rectangleFillColorEnemy)
            c_ESPRectangleFillColorEnemy = LoadColor4FromArray(rectangleFillColorEnemy);

        cJSON* rectangleVisibleFillColorFriend = cJSON_GetObjectItem(esp, "RectangleVisibleFillColorFriend");
        if (rectangleVisibleFillColorFriend)
            c_ESPRectangleVisibleFillColorFriend = LoadColor4FromArray(rectangleVisibleFillColorFriend);

        cJSON* rectangleVisibleFillColorEnemy = cJSON_GetObjectItem(esp, "RectangleVisibleFillColorEnemy");
        if (rectangleVisibleFillColorEnemy)
            c_ESPRectangleVisibleFillColorEnemy = LoadColor4FromArray(rectangleVisibleFillColorEnemy);

        cJSON* rectangleNameColorFriend = cJSON_GetObjectItem(esp, "RectangleNameColorFriend");
        if (rectangleNameColorFriend)
            c_ESPRectangleNameColorFriend = LoadColor4FromArray(rectangleNameColorFriend);

        cJSON* rectangleNameColorEnemy = cJSON_GetObjectItem(esp, "RectangleNameColorEnemy");
        if (rectangleNameColorEnemy)
            c_ESPRectangleNameColorEnemy = LoadColor4FromArray(rectangleNameColorEnemy);

        cJSON* lineColorFriend = cJSON_GetObjectItem(esp, "LineColorFriend");
        if (lineColorFriend)
            c_ESPLineColorFriend = LoadColor4FromArray(lineColorFriend);

        cJSON* lineColorEnemy = cJSON_GetObjectItem(esp, "LineColorEnemy");
        if (lineColorEnemy)
            c_ESPLineColorEnemy = LoadColor4FromArray(lineColorEnemy);
    }

    cJSON_Delete(root);
    free(buffer);
    fclose(file);
}



