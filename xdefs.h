#ifndef XDEFS_H_
#define XDEFS_H_

#define kDepth 24
#define kPixelBits 32
#define kPixelBytes (32 / 8)
#define kWidth 256
#define kHeight 256
#define kWindowBufferSize (kWidth * kHeight * kPixelBytes)
#define kBillion 1000000000L
#define kSleepNS 10000000L

#define SHM_PROJID 65
#define SHM_KBDPATH "marsemu_kbd"
#define SHM_VFBPATH "marsemu_vfb"

#endif  // XDEFS_H_
