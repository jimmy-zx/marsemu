/* An example of using Xlib */

#include <assert.h>
#include <unistd.h>
#include <stddef.h>
#include <X11/Xlib.h>

int main() {
  Display *dpy = XOpenDisplay(NULL);
  assert(dpy);
  int blackColor = BlackPixel(dpy, DefaultScreen(dpy));
  int whiteColor = WhitePixel(dpy, DefaultScreen(dpy));
  Window w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 256, 256, 0, blackColor, blackColor);
  XSelectInput(dpy, w, StructureNotifyMask);
  XMapWindow(dpy, w);
  GC gc = XCreateGC(dpy, w, 0, NULL);
  XSetForeground(dpy, gc, whiteColor);
  for (;;) {
    XEvent e;
    XNextEvent(dpy, &e);
    if (e.type == MapNotify) {
      break;
    }
  }
  XDrawLine(dpy, w, gc, 10, 60, 180, 20);
  XFlush(dpy);
  sleep(10);
}
