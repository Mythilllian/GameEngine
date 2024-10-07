#pragma once

#include <windows.h>

// Window procedure function declaration
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/// <summary>
/// Opens a window of a certain width and height and names it
/// </summary>
/// <param name="width">The width of the window</param>
/// <param name="height">The height of the window</param>
/// <param name="windowTitle">The title of the window</param>
/// <param name="windowClass">The class of the window</param>
/// <returns></returns>
HWND CreateCustomWindow(int width, int height, const char* windowTitle, const char* windowClass) {
    HINSTANCE hInstance = GetModuleHandle(NULL);

    // Define window class properties
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;  // Window procedure function
    wc.hInstance = hInstance;     // Handle to the instance
    wc.lpszClassName = windowClass;

    // Register the window class
    RegisterClass(&wc);

    // Create the window with specified dimensions
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles
        windowClass,                     // Window class
        windowTitle,                    // Window title
        WS_OVERLAPPEDWINDOW,            // Window style

        // Position and size
        CW_USEDEFAULT, CW_USEDEFAULT,   // Position (x, y)
        width, height,                  // Width and height

        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        return NULL; // Failed to create window
    }

    // Show the window
    ShowWindow(hwnd, SW_SHOW);

    return hwnd;
}

HWND CreateCustomWindow(int width, int height, const char* windowTitle[]) {
    return CreateCustomWindow(width, height, *windowTitle, "DefaultClass");
}

HWND CreateCustomWindow(int width, int height) {
    return CreateCustomWindow(width, height, "Default Title", "DefaultClass");
}

// Window procedure function definition
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}
