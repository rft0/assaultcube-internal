#include <stdio.h>
#include <Windows.h>
#include <TlHelp32.h>

#include "cheats/cheats.h"
#include "globals.h"
#include "offsets.h"
#include "config.h"
#include "mem.h"
#include "gui.h"

#include "gl_utils.h"

HMODULE dllModule = NULL;
FILE* fp = NULL;

typedef BOOL(__stdcall* wglSwapBuffers_t)(HDC hDc);
wglSwapBuffers_t oWglSwapBuffers = NULL;
wglSwapBuffers_t tWglSwapBuffers = NULL;
BYTE oWglSwapBuffersBytes[5];

DWORD WINAPI EjectThread(LPVOID lpParameter) {
    Mem_Patch((BYTE*)oWglSwapBuffers, oWglSwapBuffersBytes, 5);
    FreeConsole();
    fclose(fp);

    Sleep(100);
    FreeLibraryAndExitThread(dllModule, 0);
	return 0;
}

void InitializeProgram() {
    g_dwBaseAddress = (uintptr_t)GetModuleHandle(NULL);

    Config_Load();
}

void HandleInput() {
    // Hooking wndproc may be better but this is enough for now.
    if (GetAsyncKeyState(VK_NUMPAD0) & 1)
        Config_Load();
    else if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        g_bTriggerbotEnabled = !g_bTriggerbotEnabled;
    else if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        g_bAimbotEnabled = !g_bAimbotEnabled;
    else if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        g_bEspEnabled = !g_bEspEnabled;
}

BOOL __stdcall wglSwapBuffersHook(HDC hDc) {
    static bool bInsertPressed = false;
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        InitializeProgram();
    }

    if (GetAsyncKeyState(VK_INSERT) & !bInsertPressed) {
        bInsertPressed = true;
        CreateThread(NULL, 0, EjectThread, NULL, 0, NULL);
    }
    
    GL_SetupOrtho();

    HandleInput();

    GUI_Draw();

    DoTriggerbot();
    DoAimbot();
    DoEsp();

    GL_RestoreOrtho();
    
    return tWglSwapBuffers(hDc);
}

bool Hook(void* HookAddr, void* tFunc, int len) {
 
    if (len < 5) return FALSE;
 
    DWORD oldProtection;
    VirtualProtect(HookAddr, len, PAGE_EXECUTE_READWRITE, &oldProtection);
 
    memset(HookAddr, 0x90, len);
 
    DWORD rAddr = ((DWORD)tFunc - (DWORD)HookAddr) - 5;
 
    *(BYTE*)HookAddr = 0xE9;
 
    *(DWORD*)((DWORD)HookAddr + 1) = rAddr;
 
    DWORD tProtection;
    VirtualProtect(HookAddr, len, oldProtection, &tProtection);
 
    return TRUE;
 
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    dllModule = hModule;

    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        AllocConsole();
        freopen_s(&fp, "CONOUT$", "w", stdout);

        oWglSwapBuffers = (wglSwapBuffers_t)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers");
        memcpy(oWglSwapBuffersBytes, (BYTE*)oWglSwapBuffers, 5);
        tWglSwapBuffers = (wglSwapBuffers_t)Mem_TrampHook((BYTE*)oWglSwapBuffers, (BYTE*)wglSwapBuffersHook, 5);
    }

    return TRUE;
}