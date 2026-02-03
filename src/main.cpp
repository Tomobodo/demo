#define WIN32_LEAN_AND_MEAN
#include <windows.h>
//#include <intrin.h>

constexpr int WIDTH = 640;
constexpr int HEIGHT = 480;

constexpr int PIXEL_SIZE = 1;
constexpr int BUFFER_WIDTH = WIDTH / PIXEL_SIZE, BUFFER_HEIGHT = HEIGHT / PIXEL_SIZE;

constexpr int BALL_RADIUS = 20;

unsigned int pixel_buffer[BUFFER_WIDTH * BUFFER_HEIGHT];

int freq = 440;

//float sqrt(float x)
//{
//	float res;
//	__m128 in = _mm_set_ss(x);
//	__m128 out = _mm_sqrt_ss(in);
//	_mm_store_ss(&res, out);
//	return res;
//}

//DWORD WINAPI PlayBeep(LPVOID lpParam)
//{
//	Beep(++freq,50);
//	return 0;
//}

extern "C" void entry()
{
	HWND hWnd = CreateWindowEx(8,
		"static", nullptr,
		WS_POPUP | WS_VISIBLE | WS_MAXIMIZE,
		0, 0, 0, 0,
		0, 0, 0, 0
	);

	HDC hdc = GetDC(hWnd);

	static BITMAPINFO bmi;

	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = BUFFER_WIDTH;
	bmi.bmiHeader.biHeight = -BUFFER_HEIGHT;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;

	int ball_pos_x = 0, ball_pos_y = 0, ball_vit_x = 2, ball_vit_y = 2;

	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		static unsigned char frame = 0;
		frame++;

		ball_pos_x += ball_vit_x;
		ball_pos_y += ball_vit_y;

		if (ball_pos_x + BALL_RADIUS > BUFFER_WIDTH)
		{
			ball_pos_x = BUFFER_WIDTH - BALL_RADIUS;
			ball_vit_x *= -1;

			//CreateThread(0, 0, PlayBeep, 0, 0, 0); creating a thread is so simple with windows api omg
		}

		if (ball_pos_x - BALL_RADIUS < 0)
		{
			ball_pos_x = BALL_RADIUS;
			ball_vit_x *= -1;

			//CreateThread(0, 0, PlayBeep, 0, 0, 0);
		}

		if (ball_pos_y + BALL_RADIUS > BUFFER_HEIGHT)
		{
			ball_pos_y = BUFFER_HEIGHT - BALL_RADIUS;
			ball_vit_y *= -1;

			//CreateThread(0, 0, PlayBeep, 0, 0, 0);
		}

		if (ball_pos_y - BALL_RADIUS < 0)
		{
			ball_pos_y = BALL_RADIUS;
			ball_vit_y *= -1;

			//CreateThread(0, 0, PlayBeep, 0, 0, 0);
		}

		unsigned int* ptr = pixel_buffer;
		for (int y = 0; y < BUFFER_HEIGHT; y++)
		{
			int diff_y = ball_pos_y - y;
			int dy2 = diff_y * diff_y;

			for (int x = 0; x < BUFFER_WIDTH; x++)
			{
				int diff_x = ball_pos_x - x;
				if (diff_x * diff_x + dy2 <= (BALL_RADIUS * BALL_RADIUS))
					*ptr++ = 0xffffffff;
				else
					*ptr++ = (frame << 16) | ((y + x) & 0xFF);
			}
		}

		// blit bitmap

		RECT rc;
		GetClientRect(hWnd, &rc);

		StretchDIBits(hdc, 0, 0, rc.right, rc.bottom, 0, 0, BUFFER_WIDTH, BUFFER_HEIGHT, pixel_buffer, &bmi, DIB_RGB_COLORS, SRCCOPY);

		MSG msg;

		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			DispatchMessage(&msg);
	}

	ExitProcess(0);
}