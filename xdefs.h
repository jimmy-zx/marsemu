#ifndef XDEFS_H_
#define XDEFS_H_

const int kDepth = 24;
const int kPixelBits = 32;
const int kPixelBytes = 32 / 8;
const int kWidth = 512;
const int kHeight = 512;
const int kWindowBufferSize = kWidth * kHeight * kPixelBytes;
const long kBillion = 1000000000L;
const long kSleepNS = 10000000L;

#define MEM_KEYPATH "marsemu"
#define MEM_KEYID 65

#endif  // XDEFS_H_
