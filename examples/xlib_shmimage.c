#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/shm.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>

#include "xdefs.h"

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

int main() {
  // Defines
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
  Window window = XCreateWindow(display, root, 0, 0, kWidth, kHeight, 0, visinfo.depth, InputOutput, visinfo.visual, attributeMask, &windowAttr);
  assert(window);

  // Setup
  XStoreName(display, window, "Hello, Window!");
  setSizeHint(display, window, kWidth, kHeight, kWidth, kHeight);
  XMapWindow(display, window);
  XFlush(display);

  XShmSegmentInfo shminfo;
  XImage *xWindowBuffer = XShmCreateImage(display, visinfo.visual, visinfo.depth, ZPixmap, 0, &shminfo, kWidth, kHeight);
  key_t key = ftok("shmfile", 65);
  shminfo.shmid = shmget(key, xWindowBuffer->bytes_per_line * xWindowBuffer->height, IPC_CREAT | 0660);
  shminfo.shmaddr = xWindowBuffer->data = shmat(shminfo.shmid, 0, 0);
  shminfo.readOnly = false;
  XShmAttach(display, &shminfo);

  GC defaultGC = DefaultGC(display, defaultScreen);


  // Event loop
  Atom WM_DELETE_WINDOW = XInternAtom(display, "WM_DELETE_WINDOW", false);
  assert(XSetWMProtocols(display, window, &WM_DELETE_WINDOW, 1));
  int windowOpen = 1;
  int shmWait = 0;
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
        // case ConfigureNotify:
        //   {
        //     XConfigureEvent *e = (XConfigureEvent *) &ev;
        //     width = e->width;
        //     height = e->height;
        //     XDestroyImage(xWindowBuffer);
        //     windowBufferSize = width * height * kPixelBytes;
        //     mem = malloc(windowBufferSize);
        //     xWindowBuffer = XCreateImage(display, visinfo.visual, visinfo.depth, ZPixmap, 0, mem, width, height, kPixelBits, 0);
        //     replot = 1;
        //   }
        //   break;
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
        case ShmCompletion:
          {
            shmWait = 0;
          }
          break;
      }
    }

    if (windowOpen || !shmWait) {
      XShmPutImage(display, window, defaultGC, xWindowBuffer, 0, 0, 0, 0, kWidth, kHeight, false);
      shmWait = 1;
    }
  }

  XShmDetach(display, &shminfo);
  XDestroyImage(xWindowBuffer);
  shmdt(shminfo.shmaddr);
  shmctl(shminfo.shmid, IPC_RMID, 0);
  XCloseDisplay(display);
  return 0;
}
