#include "../../../include/engine/debug_tools.hpp"

#include <windows.h>
#include <commctrl.h>

namespace engine
{
    HWND debug_windows_handle;
    HWND seekbar_handle;
    HWND play_pause_button_handle;

    float* demo_time;
    bool* demo_paused;

    constexpr int SEEKBAR_HEIGHT = 30;
    constexpr int DEBUG_WINDOW_WIDTH = 640;
    constexpr int DEBUG_WINDOW_HEIGHT = SEEKBAR_HEIGHT + 40;
    constexpr int BUTTON_WIDTH = 60;
    constexpr int ID_PLAY_PAUSE = 1;

    void update_seekbar_size(int width, int height)
    {
        if (play_pause_button_handle)
            MoveWindow(play_pause_button_handle, 0, 0, BUTTON_WIDTH, SEEKBAR_HEIGHT, TRUE);
        MoveWindow(seekbar_handle, BUTTON_WIDTH, 0, width - BUTTON_WIDTH, SEEKBAR_HEIGHT, TRUE);
    }

    LRESULT CALLBACK DebugWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        case WM_COMMAND:
            if (LOWORD(wParam) == ID_PLAY_PAUSE)
            {
                *demo_paused = !*demo_paused;
                SetWindowText(play_pause_button_handle, *demo_paused ? ">" : "||");
                return 0;
            }
            break;
        case WM_GETMINMAXINFO:
            {
                auto lpMMI = reinterpret_cast<LPMINMAXINFO>(lParam);
                lpMMI->ptMinTrackSize.y = DEBUG_WINDOW_HEIGHT;
                lpMMI->ptMaxTrackSize.y = DEBUG_WINDOW_HEIGHT;
                return 0;
            }
        case WM_HSCROLL:
            if (reinterpret_cast<HWND>(lParam) == seekbar_handle)
            {
                int pos = SendMessage(seekbar_handle, TBM_GETPOS, 0, 0);
                *demo_time = pos / 100.0f;
                return 0;
            }
            break;
        case WM_CLOSE:
            ShowWindow(hwnd, SW_HIDE);
            return 0;

        case WM_SIZE:
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            update_seekbar_size(width, height);
            return 0;
        }

        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    void init_debug_tool(HINSTANCE hinstance, const float demo_duration, float* time, bool* paused)
    {
        demo_time = time;
        demo_paused = paused;

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
            CW_USEDEFAULT, CW_USEDEFAULT, DEBUG_WINDOW_WIDTH, DEBUG_WINDOW_HEIGHT,
            nullptr, nullptr, hinstance, nullptr
        );

        play_pause_button_handle = CreateWindowEx(
            0,
            "BUTTON",
            "||",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            0, 0,
            BUTTON_WIDTH, SEEKBAR_HEIGHT,
            debug_windows_handle,
            reinterpret_cast<HMENU>(static_cast<INT_PTR>(ID_PLAY_PAUSE)),
            nullptr,
            nullptr
        );

        seekbar_handle = CreateWindowEx(
            0,
            TRACKBAR_CLASS,
            "Trackbar Control",
            WS_CHILD | WS_VISIBLE | TBS_ENABLESELRANGE,
            0, 0,
            DEBUG_WINDOW_WIDTH, SEEKBAR_HEIGHT,
            debug_windows_handle,
            nullptr,
            nullptr,
            nullptr
        );

        RECT rect = {};
        GetClientRect(debug_windows_handle, &rect);

        update_seekbar_size(rect.right, rect.bottom);

        SendMessage(seekbar_handle, TBM_SETRANGE, TRUE, MAKELONG(0, static_cast<int>(demo_duration * 100)));
        SendMessage(seekbar_handle, TBM_SETPAGESIZE, 0, 10);

        SendMessage(seekbar_handle, TBM_SETPOS, TRUE, 0);
    }

    void update_debug_tool(float time)
    {
        SendMessage(seekbar_handle, TBM_SETPOS, TRUE, static_cast<int>(time * 100));
    }
}
