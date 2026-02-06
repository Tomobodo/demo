#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dwmapi.h>

#include "scene.hpp"
#include "scene_a.hpp"
#include "frame_infos.h"

//constexpr int BUFFER_WIDTH = 850;
//constexpr int BUFFER_HEIGHT = 480;

constexpr int BUFFER_WIDTH = 1920;
constexpr int BUFFER_HEIGHT = 1080;

unsigned int pixel_buffer[BUFFER_WIDTH * BUFFER_HEIGHT];

#ifdef DEBUG
extern "C" int _fltused = 0;
#endif

void* operator new(size_t size)
{
	return HeapAlloc(GetProcessHeap(), 0, size);
}

void operator delete(void* p, size_t size) noexcept
{
	if (p) HeapFree(GetProcessHeap(), 0, p);
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
}

extern "C" void entry()
{
	HWND hWnd = CreateWindowEx(0,
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

	auto frequency = static_cast<unsigned int>(freq.LowPart);
	auto smooth_fps_buffer_index = 0;

	FrameInfos frame_infos{
		.frame = 0,
		.delta_time = 0.0f,
		.time = 0.0f,
		.fps = 0.0f,
		.pixel_buffer = pixel_buffer,
		.pixel_buffer_width = BUFFER_WIDTH,
		.pixel_buffer_height = BUFFER_HEIGHT
	};

	Scene* current_scene = new SceneA();

	MSG msg;

	bool loop = true;
	RECT rc;
	GetClientRect(hWnd, &rc);

	while (loop)
	{
		QueryPerformanceCounter(&t_end);

		const unsigned int elapsed = t_end.LowPart - t_start.LowPart;
		frame_infos.frame++;
		frame_infos.delta_time = static_cast<float>(elapsed) / static_cast<float>(frequency);
		frame_infos.time += frame_infos.delta_time;
		frame_infos.fps = 1.0f / frame_infos.delta_time;
		frame_infos.smooth_fps_buffer[smooth_fps_buffer_index++ % SMOOTH_FPS_BUFFER_SIZE] = frame_infos.fps;

		frame_infos.smooth_fps = 0;
		for (const float fps : frame_infos.smooth_fps_buffer)
			frame_infos.smooth_fps += fps;
		frame_infos.smooth_fps /= SMOOTH_FPS_BUFFER_SIZE;

		t_start = t_end;

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
		// update current scene
		current_scene->update(frame_infos);

		// blit bitmap


		StretchDIBits(hdc, 0, 0, rc.right, rc.bottom, 0, 0, BUFFER_WIDTH, BUFFER_HEIGHT, pixel_buffer, &bmi, DIB_RGB_COLORS,
		              SRCCOPY);

#ifdef DEBUG
		char fps_text[32];
		wsprintf(fps_text, "FPS: %d", static_cast<int>(frame_infos.smooth_fps));

		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkMode(hdc, TRANSPARENT);

		TextOut(hdc, 10, 10, fps_text, lstrlen(fps_text));
#endif

		//DwmFlush();
	}

	ExitProcess(0);
}
