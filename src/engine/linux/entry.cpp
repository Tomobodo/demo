#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include <time.h>

#include "engine/demo.hpp"

static long get_time_ns() {
  timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec * 1000000000L + ts.tv_nsec;
}

__attribute__((force_align_arg_pointer)) extern "C" void entry() {
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
  unsigned long frame = 0;

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

    ++frame;
  }

  demo_deinit();

  exit(0);
}
