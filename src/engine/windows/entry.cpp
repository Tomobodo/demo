#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "engine/demo.hpp"

#ifdef DEBUG
#include "../../../include/engine/debug_tools.hpp"
#endif

constexpr float ASPECT_RATIO = static_cast<float>(BUFFER_WIDTH) / static_cast<float>(BUFFER_HEIGHT);

void update_window_size(const HWND& window_handle, bool fullscreen)
{
	MONITORINFO mi = {sizeof(mi)};
	GetMonitorInfo(MonitorFromWindow(window_handle, MONITOR_DEFAULTTOPRIMARY), &mi);

	const HDC hdc = GetDC(window_handle);

	if (fullscreen)
		SetWindowPos(window_handle, HWND_TOP,
		             mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left,
		             mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	else
		SetWindowPos(
			window_handle, HWND_TOP,
			(mi.rcMonitor.right - BUFFER_WIDTH) / 2, (mi.rcMonitor.bottom - BUFFER_HEIGHT) / 2,
			BUFFER_WIDTH, BUFFER_HEIGHT, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

	RECT rc;
	GetClientRect(window_handle, &rc);
	FillRect(hdc, &rc, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));
}

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

decltype(&demo_init) init = nullptr;
decltype(&demo_deinit) deinit = nullptr;
decltype(&demo_update) update = nullptr;
decltype(&demo_get_buffer) get_buffer = nullptr;
decltype(&demo_get_duration) get_duration = nullptr;

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
		init = reinterpret_cast<decltype(init)>(GetProcAddress(lib_handle, "demo_init"));
		deinit = reinterpret_cast<decltype(deinit)>(GetProcAddress(lib_handle, "demo_deinit"));
		update = reinterpret_cast<decltype(update)>(GetProcAddress(lib_handle, "demo_update"));
		get_buffer = reinterpret_cast<decltype(get_buffer)>(GetProcAddress(lib_handle, "demo_get_buffer"));
		get_duration = reinterpret_cast<decltype(get_duration)>(GetProcAddress(lib_handle, "demo_get_duration"));
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
constexpr auto& deinit = demo_deinit;
constexpr auto& update = demo_update;
constexpr auto& get_buffer = demo_get_buffer;
constexpr auto& get_duration = demo_get_duration;
#endif

void entry()
{
	HINSTANCE hinstance = GetModuleHandle(nullptr);
#ifdef HOTRELOAD
	load_lib();

	start_dll_watcher();
#endif

	HWND window_handle = CreateWindowEx(0,
	                                    reinterpret_cast<LPCSTR>(0x8000), nullptr,
	                                    WS_POPUP | WS_VISIBLE,
	                                    0, 0, BUFFER_WIDTH, BUFFER_HEIGHT,
	                                    nullptr, nullptr, hinstance, nullptr
	);

	HDC hdc = GetDC(window_handle);

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
	float delta_samples = 0;
	int fps_samples = 0;
	int fps = 0;
#endif

	const auto frequency = static_cast<unsigned int>(freq.LowPart);

	MSG msg;

	bool loop = true;
	bool fullscreen = false;
	init();

	float time = 0;

#if defined(DEBUG)
	bool paused = false;
	engine::init_debug_tool(hinstance, get_duration(), &time, &paused);
#endif

	RECT rc;

	update_window_size(window_handle, fullscreen);

	while (loop)
	{
#ifdef HOTRELOAD
		if (need_to_reload_lib)
		{
			deinit();
			load_lib();
			init();
		}
#endif

		QueryPerformanceCounter(&t_end);
		float delta = static_cast<float>(t_end.LowPart - t_start.LowPart) / static_cast<float>(
			frequency);

#ifdef DEBUG
		if (!paused)
#endif

		time += delta;
		t_start = t_end;

#if defined(DEBUG) || defined(SHOW_FPS)
		delta_samples += delta;
		fps_samples++;
		if (delta_samples >= 1.0f)
		{
			fps = static_cast<int>(1.0f / (delta_samples / static_cast<float>(fps_samples)));
			fps_samples = 0;
			delta_samples -= 1.0f;
		}
#endif

#ifdef DEBUG
		engine::update_debug_tool(time);
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

			if (msg.message == WM_KEYDOWN && msg.wParam == VK_F11)
			{
				fullscreen = !fullscreen;
				update_window_size(window_handle, fullscreen);
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		GetClientRect(window_handle, &rc);
		auto dest_width = static_cast<int>(static_cast<float>(rc.bottom) * ASPECT_RATIO);
		auto blit_x = (rc.right - dest_width) / 2;

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

	deinit();

	ExitProcess(0);
}
}
