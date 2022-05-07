#pragma once 

#define GAME_NAME "GAME_ALPHA"
#define GAME_RES_WIDTH 384
#define GAME_RES_HEIGHT 216
#define GAME_BPP 32
#define GAME_DRAWING_AREA_MEMORY_SIZE (GAME_RES_WIDTH * GAME_RES_HEIGHT * (GAME_BPP/8))

typedef struct GAMEBITMAP
{
    BITMAPINFO BitMapInfo;
    
    void* Memory;
} GAMEBITMAP;

// Handles windows events
LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMSG, WPARAM wParam, LPARAM lParam);

DWORD CreateMainGameWindow(void);

BOOL GameIsAlreadyRunning(void);

void ProcessPlayerInput(void);

void RenderFrameGraphics(void);


