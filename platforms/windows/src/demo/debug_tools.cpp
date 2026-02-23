
#include "debug_tools.hpp"

#include <windows.h>
#include <commctrl.h>

HWND debug_windows_handle;
HWND seekbar_handle;

float* demo_time;

LRESULT CALLBACK DebugWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_HSCROLL:
        if (reinterpret_cast<HWND>(lParam) == seekbar_handle)
        {
            int pos = SendMessage(seekbar_handle, TBM_GETPOS, 0, 0);
            *demo_time = pos / 100.0f;
        }
        break;
    case WM_CLOSE:
        ShowWindow(hwnd, SW_HIDE);
        return 0;

    case WM_SIZE:
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);
        MoveWindow(seekbar_handle, 0, 0, width, height, TRUE);
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void init_debug_tool(HINSTANCE hinstance, const float demo_duration, float* time)
{
    demo_time = time;

    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_BAR_CLASSES;
    InitCommonControlsEx(&icex);

    WNDCLASS wc = {};
    wc.lpfnWndProc = DebugWindowProc;
    wc.hInstance = nullptr;
    wc.lpszClassName = "DebugWindowClass";
    wc.hbrBackground = reinterpret_cast<HBRUSH>((BLACK_BRUSH));
    RegisterClass(&wc);

    debug_windows_handle = CreateWindowEx(
        WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
        "DebugWindowClass", "Debug Tools",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 640, 100,
        nullptr, nullptr, hinstance, nullptr
    );

    seekbar_handle = CreateWindowEx(
        0,
        TRACKBAR_CLASS,
        "Trackbar Control",
        WS_CHILD | WS_VISIBLE | TBS_ENABLESELRANGE,
        0, 0,
        640, 30,
        debug_windows_handle,
        nullptr,
        nullptr,
        nullptr
    );

    SendMessage(seekbar_handle, TBM_SETRANGE, TRUE, MAKELONG(0, static_cast<int>(demo_duration * 100)));
    SendMessage(seekbar_handle, TBM_SETPAGESIZE, 0, 10);

    SendMessage(seekbar_handle, TBM_SETPOS, TRUE, 0);
}

void update_debug_tool(float time)
{
    SendMessage(seekbar_handle, TBM_SETPOS, TRUE, static_cast<int>(time * 100));
}
