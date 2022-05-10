// Standard library for input and output
#include <stdio.h>
// Windows standard lib?
#include <windows.h>
// My own header file
#include "main.h"

HANDLE gGameWindow;
BOOL gGameIsRunning;
GAMEBITMAP gDrawingSurface;

// Main function that creates a window instead of int main() for a console app
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
    // Tells the compiler we're not using these variables on purpose
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    if(GameIsAlreadyRunning() == TRUE) {
        MessageBoxA(NULL, "Game is already running!", GAME_NAME, MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    if(CreateMainGameWindow() != ERROR_SUCCESS) {
        goto Exit;
    }
    gDrawingSurface.BitMapInfo.bmiHeader.biSize = sizeof(gDrawingSurface.BitMapInfo.bmiHeader);
    gDrawingSurface.BitMapInfo.bmiHeader.biWidth = GAME_RES_WIDTH;
    gDrawingSurface.BitMapInfo.bmiHeader.biHeight = GAME_RES_HEIGHT;
    gDrawingSurface.BitMapInfo.bmiHeader.biBitCount = GAME_BPP;
    gDrawingSurface.BitMapInfo.bmiHeader.biCompression = BI_RGB;
    gDrawingSurface.BitMapInfo.bmiHeader.biPlanes = 1;
    if((gDrawingSurface.Memory = VirtualAlloc(NULL, GAME_DRAWING_AREA_MEMORY_SIZE,
    MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)) == NULL) {
        MessageBoxA(NULL, "Failed to allocate memory for drawing suface!",
        "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    } 

    MSG msg = {0};
    gGameIsRunning = TRUE;

    // Game frame loop
    while(gGameIsRunning) 
    {
        // Will dispatch all new messages then continue on to the next while loop
        // Like GetMesasgeA but wont lock up the while loop
        while(PeekMessageA(&msg, gGameWindow, 0, 0, PM_REMOVE)) 
        {
            DispatchMessage(&msg);
        }
        ProcessPlayerInput();
        RenderFrameGraphics();

        // Will allow another program to run on thread before continuing, but how long the program runs is unknown
        Sleep(1);
    }
  
    Exit:
        return 0;
}


DWORD CreateMainGameWindow(void) {
    // Initalize datastructure with all zeros  - "{ 0 }"
    WNDCLASSEXA WindowClass = {0};
    DWORD Result = ERROR_SUCCESS;

    WindowClass.cbSize = sizeof(WNDCLASSEXA);
    WindowClass.style = 0;
    WindowClass.lpfnWndProc = MainWindowProc;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = GetModuleHandleA(NULL);
    WindowClass.hIcon = LoadIconA(NULL, IDI_APPLICATION);
    WindowClass.hIconSm = LoadIconA(NULL, IDI_APPLICATION);
    WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    WindowClass.lpszMenuName = NULL;
    WindowClass.lpszClassName =  GAME_NAME "_WINDOWCLASS";

    if (!RegisterClassExA(&WindowClass))
    {
        Result = GetLastError();
        MessageBoxA(NULL, "Window Registration Failed!", "ERROR!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    gGameWindow = CreateWindowExA(WS_EX_CLIENTEDGE, WindowClass.lpszClassName, GAME_NAME,
                                   WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, WindowClass.hInstance, NULL);

    if (!gGameWindow)
    {
        Result = GetLastError();
        MessageBoxA(NULL, "Window Creation Failed!", "ERROR!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }
    
    Exit:
        return Result;
}

// Handles all the windows events
LRESULT CALLBACK MainWindowProc(
    HWND hwnd,     // Handle to window
    UINT uMSG,     // Messasge identifier
    WPARAM wParam, // First message param
    LPARAM lParam) // Second message param
{
    LRESULT Result = 0;

    switch (uMSG)
    {
        case WM_CLOSE:
        gGameIsRunning = FALSE;
        PostQuitMessage(0);
        break;
    
    default:
        Result = DefWindowProcA(hwnd, uMSG, wParam, lParam);
    }
    return Result;
}

BOOL GameIsAlreadyRunning(void)
{
    HANDLE Mutex = NULL;

    Mutex = CreateMutexA(NULL, FALSE, GAME_NAME "_GameMutex");

    if(GetLastError() == ERROR_ALREADY_EXISTS) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void ProcessPlayerInput(void) 
{
    // GetAsyncKeyState will return true if key is pressed
    short EscapeKeyIsDown = GetAsyncKeyState(VK_ESCAPE);

    if(EscapeKeyIsDown) {
        SendMessageA(gGameWindow, WM_CLOSE, 0, 0);
    }
}

void RenderFrameGraphics(void) 
{

}
