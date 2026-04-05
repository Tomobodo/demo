#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

#include "engine/demo.hpp"

extern "C" [[noreturn]] void sys_exit(int code);

extern "C" int sys_clock_gettime(long clock_id, timespec *ts);

long get_time_ns() {
	timespec ts;
	sys_clock_gettime(1, &ts);
	return ts.tv_sec * 1000000000L + ts.tv_nsec;
}

__attribute__((section(".text.startup._start"),
							 force_align_arg_pointer)) extern "C" void
_start() {
	Display *dpy = XOpenDisplay(nullptr);
	Window win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0,
																	 BUFFER_WIDTH, BUFFER_HEIGHT, 0, 0, 0);

	XSelectInput(dpy, win, KeyPressMask);
	XMapWindow(dpy, win);

	GC gc = XCreateGC(dpy, win, 0, nullptr);

	demo_init();

	XImage *img = XCreateImage(dpy, DefaultVisual(dpy, 0), DefaultDepth(dpy, 0),
														 ZPixmap, 0, (char *)demo_get_buffer(),
														 BUFFER_WIDTH, BUFFER_HEIGHT, 32, 0);

	bool loop = true;

	long t_start = get_time_ns();
	float time = 0.0f;

	while (loop) {
		long t_now = get_time_ns();
		float delta = (float)(t_now - t_start) / 1e9f;
		t_start = t_now;

		time += delta;

		XEvent ev;
		while (XPending(dpy)) {
			XNextEvent(dpy, &ev);
			if (ev.type == KeyPress) {
				KeySym sym = XLookupKeysym(&ev.xkey, 0);

				switch (sym) {
				case XK_Escape:
					loop = false;
					break;
				}
			}
		}

		demo_update(time);
		XPutImage(dpy, win, gc, img, 0, 0, 0, 0, BUFFER_WIDTH, BUFFER_HEIGHT);
		XFlush(dpy);
	}

	demo_deinit();

	sys_exit(0);
}
