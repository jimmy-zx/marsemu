#ifndef XDEFS_H_
#define XDEFS_H_

const int kDepth = 24;
const int kPixelBits = 32;
const int kPixelBytes = 32 / 8;
const int kWidth = 256;
const int kHeight = 256;
const int kWindowBufferSize = kWidth * kHeight * kPixelBytes;
const long kBillion = 1000000000L;
const long kSleepNS = 10000000L;

#define SHM_KEYPATH "marsemu"
#define SHM_FBKEYID 65
#define SHM_KBDKEYID 66

#endif  // XDEFS_H_
