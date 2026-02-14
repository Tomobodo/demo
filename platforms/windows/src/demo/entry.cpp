#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dwmapi.h>

#include "demo/demo.hpp"

#ifndef BUFFER_WIDTH
#define BUFFER_WIDTH 640
#endif

#ifndef BUFFER_HEIGHT
#define BUFFER_HEIGHT 480
#endif

constexpr float ASPECT_RATIO = (float)BUFFER_WIDTH / (float)BUFFER_HEIGHT;

extern "C" {
#pragma function(memset)
void* __cdecl memset(void* dest, int c, size_t count)
{
	auto p = static_cast<unsigned char*>(dest);
	while (count--)
		*p++ = static_cast<unsigned char>(c);
	return dest;
}

// need to be set for the compiler to know we use float
int _fltused = 0;

#ifdef HOTRELOAD
typedef void (*DemoVoidFunc)();
typedef void (*DemoFloatFunc)(float);
typedef unsigned int* (*DemoRetIntPtrFunc)();

decltype(demo_init)* init = nullptr;

decltype(demo_update)* update = nullptr;

decltype(demo_get_buffer)* get_buffer = nullptr;

HINSTANCE lib_handle = nullptr;
bool need_to_reload_lib = true;

void load_lib()
{
	if (lib_handle)
		FreeLibrary(lib_handle);

	CopyFile(HOTRELOAD_LIB_NAME, HOTRELOAD_LIB_LOCKED_NAME, false);

	lib_handle = LoadLibrary(TEXT(HOTRELOAD_LIB_LOCKED_NAME));
	if (lib_handle != nullptr)
	{
		init = reinterpret_cast<DemoVoidFunc>(GetProcAddress(lib_handle, "demo_init"));
		update = reinterpret_cast<DemoFloatFunc>(GetProcAddress(lib_handle, "demo_update"));
		get_buffer = reinterpret_cast<DemoRetIntPtrFunc>(GetProcAddress(lib_handle, "demo_get_buffer"));
		need_to_reload_lib = false;
	}
	else
	{
		ExitProcess(1);
	}
}

DWORD WINAPI dll_watcher(LPVOID param)
{
	HANDLE dir_handle = CreateFile(
		".",
		FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		nullptr,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		nullptr
	);

	if (dir_handle == INVALID_HANDLE_VALUE) return 1;
	char buffer[1024];
	DWORD bytes_returned;

	while (true)
	{
		if (ReadDirectoryChangesW(
			dir_handle,
			buffer,
			sizeof(buffer),
			false,
			FILE_NOTIFY_CHANGE_LAST_WRITE,
			&bytes_returned,
			nullptr,
			nullptr
		))
		{
			Sleep(100);

			need_to_reload_lib = true;
		}
	}

	return 0;
}

void start_dll_watcher()
{
	DWORD thread_id;
	HANDLE thread_handle = CreateThread(
		nullptr,
		0,
		dll_watcher,
		nullptr,
		0,
		&thread_id
	);
}

#else
constexpr auto& init = demo_init;
constexpr auto& update = demo_update;
constexpr auto& get_buffer = demo_get_buffer;
#endif

void entry()
{
#ifdef HOTRELOAD
	load_lib();

	start_dll_watcher();
#endif

	const HWND hWnd = CreateWindowEx(0,
	                                 reinterpret_cast<LPCSTR>(0x8000), nullptr,
#ifdef FULLSCREEN
	                                 WS_POPUP | WS_VISIBLE | WS_MAXIMIZE,
	                                 0, 0, 0, 0,
#else
	                                 WS_POPUP | WS_VISIBLE,
	                                 0, 0, BUFFER_WIDTH, BUFFER_HEIGHT,
#endif
	                                 nullptr, nullptr, nullptr, nullptr
	);

	HDC hdc = GetDC(hWnd);

	static BITMAPINFO bmi;

	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = BUFFER_WIDTH;
	bmi.bmiHeader.biHeight = -BUFFER_HEIGHT;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;

	LARGE_INTEGER freq, t_start, t_end;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&t_start);

#if defined(DEBUG) || defined(SHOW_FPS)
	LARGE_INTEGER t_start_dbg_fps;
	QueryPerformanceCounter(&t_start_dbg_fps);
	float delta_samples = 0;
	int fps_samples = 0;
	int fps = 0;
#endif

	const auto frequency = static_cast<unsigned int>(freq.LowPart);

	MSG msg;

	bool loop = true;
	RECT rc;
	GetClientRect(hWnd, &rc);

	const auto dest_width = static_cast<int>(static_cast<float>(rc.bottom) * ASPECT_RATIO);
	const int blit_x = (rc.right - dest_width) / 2;

	FillRect(hdc, &rc, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));

	init();

	while (loop)
	{
		if (need_to_reload_lib)
		{
			load_lib();
			init();
		}

		QueryPerformanceCounter(&t_end);

		const float time = static_cast<float>(t_end.LowPart - t_start.LowPart) / static_cast<float>(frequency);

#if defined(DEBUG) || defined(SHOW_FPS)
		delta_samples += static_cast<float>(t_end.LowPart - t_start_dbg_fps.LowPart) / static_cast<float>(
			frequency);
		fps_samples++;
		if (delta_samples >= 1.0f)
		{
			fps = static_cast<int>(1.0f / (delta_samples / static_cast<float>(fps_samples)));
			fps_samples = 0;
			delta_samples -= 1.0f;
		}
		t_start_dbg_fps = t_end;
#endif

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (
				msg.message == WM_QUIT ||
				msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)
			{
				loop = false;
				break;
			}

			if (msg.message == WM_SIZE)
				GetClientRect(hWnd, &rc);

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		update(time);

		// blit bitmap
		StretchDIBits(hdc,
		              blit_x, 0,
		              dest_width, rc.bottom,
		              0, 0, BUFFER_WIDTH, BUFFER_HEIGHT,
		              get_buffer(),
		              &bmi, DIB_RGB_COLORS,
		              SRCCOPY);

#if defined(DEBUG) || defined(SHOW_FPS)
		char fps_text[16];
		wsprintf(fps_text, "FPS: %d", fps);
		SetTextColor(hdc, RGB(0xFF, 0xFF, 0xFF));
		SetBkMode(hdc, TRANSPARENT);

		TextOut(hdc, blit_x + 10, 10, fps_text, lstrlen(fps_text));
#endif
		//DwmFlush();
	}

	ExitProcess(0);
}
}
