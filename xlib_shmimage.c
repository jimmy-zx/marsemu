#include <assert.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>

int main() {
  Display *d;
  int s;
  XImage *image;
  XShmSegmentInfo shminfo;
  d = XOpenDisplay(NULL);
  s = DefaultScreen(d);

  image = XShmCreateImage(d, 
      DefaultVisual(d,0), // Use a correct visual. Omitted for brevity     
      24,   // Determine correct depth from the visual. Omitted for brevity
      ZPixmap, NULL, &shminfo, 100, 100); 

  shminfo.shmid = shmget(IPC_PRIVATE,
      image->bytes_per_line * image->height,
      IPC_CREAT|0777);

  shminfo.shmaddr = image->data = shmat(shminfo.shmid, 0, 0);
  shminfo.readOnly = False;

  XShmAttach(d, &shminfo);

  XShmGetImage(d,
      RootWindow(d,0),
      image,
      50,
      50,
      AllPlanes);

  sleep(10);
}
