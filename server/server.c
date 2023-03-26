/* An example to put SHM image to X11 window */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/shm.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>

#include "../xdefs.h"

static Display *display;
static int root;
static int defaultScreen;
static XVisualInfo visinfo;
static XSetWindowAttributes windowAttr;
static unsigned long attributeMask;
static Window window;
static XImage *xWindowBuffer;
static GC defaultGC;
static Atom WM_DELETE_WINDOW;
static int windowOpen;

void sigint_handler(int signum) {
  if (signum == SIGINT) {
    puts("Caught SIGINT");
    windowOpen = 0;
  }
}

void setSizeHint(int minWidth, int minHeight, int maxWidth, int maxHeight) {
  XSizeHints hints;
  memset(&hints, 0, sizeof(XSizeHints));
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
  signal(SIGINT, sigint_handler);
  XShmSegmentInfo shminfo;
  key_t fbkey = ftok(SHM_VFBPATH, SHM_PROJID);
  assert(fbkey != -1);
  key_t kbdkey = ftok(SHM_KBDPATH, SHM_PROJID);
  assert(kbdkey != -1);
  printf("%llx %llx\n", (long long) fbkey, (long long) kbdkey);

  // Defines
  display = XOpenDisplay(NULL);
  assert(display);
  root = DefaultRootWindow(display);
  defaultScreen = DefaultScreen(display);
  assert(XMatchVisualInfo(display, defaultScreen, kDepth, TrueColor, &visinfo));
  windowAttr.bit_gravity = StaticGravity;  // avoid flicker
  windowAttr.background_pixel = 0;
  windowAttr.colormap = XCreateColormap(display, root, visinfo.visual, AllocNone);
  windowAttr.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask;
  attributeMask = CWBitGravity | CWBackPixel | CWColormap | CWEventMask;
  window = XCreateWindow(display, root, 0, 0, kWidth, kHeight, 0, visinfo.depth, InputOutput, visinfo.visual, attributeMask, &windowAttr);
  assert(window);

  // Setup the window
  XStoreName(display, window, "marsemu server");
  setSizeHint(kWidth, kHeight, kWidth, kHeight);
  XMapWindow(display, window);
  XFlush(display);
  defaultGC = DefaultGC(display, defaultScreen);
  WM_DELETE_WINDOW = XInternAtom(display, "WM_DELETE_WINDOW", false);
  assert(XSetWMProtocols(display, window, &WM_DELETE_WINDOW, 1));

  // Setup the framebuffer
  xWindowBuffer = XShmCreateImage(display, visinfo.visual, visinfo.depth, ZPixmap, 0, &shminfo, kWidth, kHeight);
  shminfo.shmid = shmget(fbkey, xWindowBuffer->bytes_per_line * xWindowBuffer->height, IPC_CREAT | 0660);
  assert(shminfo.shmid != -1);
  shminfo.shmaddr = xWindowBuffer->data = shmat(shminfo.shmid, 0, 0);
  assert(shminfo.shmaddr != (void *)-1);
  shminfo.readOnly = false;
  XShmAttach(display, &shminfo);

  // Setup the keyboard buffer
  int kbdshmid = shmget(kbdkey, 2 * sizeof(char), IPC_CREAT | 0660);
  assert(kbdshmid != -1);
  char *kbdbuf = shmat(kbdshmid, 0, 0);
  assert(kbdbuf != (void *)-1);
  kbdbuf[0] = 0;
  kbdbuf[1] = 0;

  // Event loop
  int shmWait = 0;
  struct timespec last, cur;
  if ((clock_gettime(CLOCK_REALTIME, &last)) == -1) {
    perror("clock_gettime");
    return 1;
  }
  struct timespec stattime;
  size_t statcount = 0;
  memcpy(&stattime, &last, sizeof(struct timespec));
  windowOpen = 1;
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
        case KeyPress:
          {
            XKeyPressedEvent *e = (XKeyPressedEvent *) &ev;
            printf("key %u pressed\n", e->keycode);
            kbdbuf[0] = 1;
            kbdbuf[1] = e->keycode;
          }
          break;
        case KeyRelease:
          {
            XKeyReleasedEvent *e = (XKeyReleasedEvent *) &ev;
            printf("key %u released\n", e->keycode);
            kbdbuf[0] = 0;
            kbdbuf[1] = e->keycode;
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
      statcount++;
      XShmPutImage(display, window, defaultGC, xWindowBuffer, 0, 0, 0, 0, kWidth, kHeight, false);
      shmWait = 1;
    }
    if (clock_gettime(CLOCK_REALTIME, &cur) == -1) {
      perror("clock_gettime");
      return 1;
    }
    if (shmWait == 1 && statcount % 100 == 0) {
      double fps = (double) statcount / (cur.tv_sec - stattime.tv_sec) + (double) (cur.tv_nsec - stattime.tv_nsec) / kBillion;
      printf("fps=%lf\n", fps);
    }
    memcpy(&last, &cur, sizeof(struct timespec));
    cur.tv_sec -= last.tv_sec;
    cur.tv_nsec -= last.tv_nsec;
    if (cur.tv_sec == 0 && cur.tv_nsec < kSleepNS) {
      cur.tv_nsec = kSleepNS - cur.tv_nsec;
      if (nanosleep(&cur, NULL) == -1) {
        perror("nanosleep");
      }
    }
  }

  printf("Shutting down\n");
  XShmDetach(display, &shminfo);
  XDestroyImage(xWindowBuffer);
  shmdt(kbdbuf);
  shmctl(kbdshmid, IPC_RMID, 0);
  shmdt(shminfo.shmaddr);
  shmctl(shminfo.shmid, IPC_RMID, 0);
  XCloseDisplay(display);
  return 0;
}
