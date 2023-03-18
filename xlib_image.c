#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

const int kDepth = 24;
const int kPixelBits = 32;
const int kPixelBytes = 32 / 8;

void setSizeHint(Display *display, Window window, int minWidth, int minHeight, int maxWidth, int maxHeight) {
  XSizeHints hints = {};
  if (minWidth > 0 && minHeight > 0) {
    hints.flags |= PMinSize;
  }
  if (maxWidth > 0 && maxHeight > 0) {
    hints.flags |= PMaxSize;
  }
  hints.min_width = minWidth;
  hints.min_height = minHeight;
  hints.max_width = maxWidth;
  hints.max_height = maxHeight;
  XSetWMNormalHints(display, window, &hints);
}

Status toggleMaximize(Display *display, Window window) {
  XClientMessageEvent ev = {};
  Atom wmState = XInternAtom(display, "_NET_WM_STATE", False);
  Atom maxH = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
  Atom maxV = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_VERT", False);

  if (wmState == None) {
    return 0;
  }

  ev.type = ClientMessage;
  ev.format = 32;
  ev.window = window;
  ev.message_type = wmState;
  ev.data.l[0] = 2;
  ev.data.l[1] = maxH;
  ev.data.l[2] = maxV;
  ev.data.l[3] = 1;
  return XSendEvent(display, DefaultRootWindow(display), False, SubstructureNotifyMask, (XEvent *) &ev);
}

int main() {
  // Defines
  int width = 512;
  int height = 512;
  int windowBufferSize = width * height * kPixelBytes;
  Display *display = XOpenDisplay(NULL);
  assert(display);
  int root = DefaultRootWindow(display);
  int defaultScreen = DefaultScreen(display);
  XVisualInfo visinfo = {};
  assert(XMatchVisualInfo(display, defaultScreen, kDepth, TrueColor, &visinfo));
  XSetWindowAttributes windowAttr;
  windowAttr.bit_gravity = StaticGravity;
  windowAttr.background_pixel = 0;
  windowAttr.colormap = XCreateColormap(display, root, visinfo.visual, AllocNone);
  windowAttr.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask;
  unsigned long attributeMask = CWBitGravity | CWBackPixel | CWColormap | CWEventMask;
  Window window = XCreateWindow(display, root, 0, 0, width, height, 0, visinfo.depth, InputOutput, visinfo.visual, attributeMask, &windowAttr);
  assert(window);

  // Setup
  XStoreName(display, window, "Hello, Window!");
  // setSizeHint(display, window, width, height, width, height);
  XMapWindow(display, window);
  // printf("%d\n", toggleMaximize(display, window));
  XFlush(display);

  char *mem = malloc(windowBufferSize);
  XImage *xWindowBuffer = XCreateImage(display, visinfo.visual, visinfo.depth, ZPixmap, 0, mem, width, height, kPixelBits, 0);
  GC defaultGC = DefaultGC(display, defaultScreen);


  // Event loop
  Atom WM_DELETE_WINDOW = XInternAtom(display, "WM_DELETE_WINDOW", False);
  assert(XSetWMProtocols(display, window, &WM_DELETE_WINDOW, 1));
  int windowOpen = 1;
  int replot = 1;
  while (windowOpen) {
    XEvent ev;
    while (XPending(display) > 0) {
      XNextEvent(display, &ev);
      switch (ev.type) {
        case DestroyNotify:
          {
            printf("DestroyNotify\n");
            XDestroyWindowEvent *e = (XDestroyWindowEvent *) &ev;
            if (e->window == window) {
              windowOpen = 0;
            }
          }
          break;
        case ClientMessage:
          {
            printf("ClientMessage\n");
            XClientMessageEvent *e = (XClientMessageEvent *) &ev;
            if ((Atom) e->data.l[0] == WM_DELETE_WINDOW) {
              XDestroyWindow(display, window);
              windowOpen = 0;
            }
          }
          break;
        case ConfigureNotify:
          {
            XConfigureEvent *e = (XConfigureEvent *) &ev;
            width = e->width;
            height = e->height;
            XDestroyImage(xWindowBuffer);
            windowBufferSize = width * height * kPixelBytes;
            mem = malloc(windowBufferSize);
            xWindowBuffer = XCreateImage(display, visinfo.visual, visinfo.depth, ZPixmap, 0, mem, width, height, kPixelBits, 0);
            replot = 1;
          }
          break;
        case KeyPress:
          {
            XKeyPressedEvent *e = (XKeyPressedEvent *) &ev;
            printf("key %u pressed\n", e->keycode);
          }
          break;
        case KeyRelease:
          {
            XKeyReleasedEvent *e = (XKeyReleasedEvent *) &ev;
            printf("key %u released\n", e->keycode);
          }
          break;
      }
    }

    if (replot) {
      int pitch = width * kPixelBytes;
      for (int y = 0; y < height; y++) {
        char *row = mem + (y * pitch);
        for (int x = 0; x < width; x++) {
          unsigned int *p = (unsigned int *) (row + (x * kPixelBytes));
          if (x % 16 && y % 16) {
            *p = 0xffffffff;
          } else {
            *p = 0;
          }
        }
      }
      XPutImage(display, window, defaultGC, xWindowBuffer, 0, 0, 0, 0, width, height);
      replot = 0;
    }
  }

  XDestroyImage(xWindowBuffer);
  XCloseDisplay(display);

  return 0;
}
