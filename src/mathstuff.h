#ifndef MATHSTUFF_H
#define MATHSTUFF_H

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))

#define abs(x) ((x) < 0 ? -(x) : (x))

#define log10_u8(x) ( \
  (x) >= 100 ? 2 :     \
  (x) >= 10 ? 1 :      \
  (x) >= 1 ? 0 :       \
  0                   \
)

#define log10_u16(x) ( \
  (x) >= 10000 ? 4 :    \
  (x) >= 1000 ? 3 :     \
  (x) >= 100 ? 2 :      \
  (x) >= 10 ? 1 :       \
  (x) >= 1 ? 0 :        \
  0                    \
)

#define log10_u32(x) (   \
  (x) >= 1000000000 ? 9 : \
  (x) >= 100000000 ? 8 :  \
  (x) >= 10000000 ? 7 :   \
  (x) >= 1000000 ? 6 :    \
  (x) >= 100000 ? 5 :     \
  (x) >= 10000 ? 4 :      \
  (x) >= 1000 ? 3 :       \
  (x) >= 100 ? 2 :        \
  (x) >= 10 ? 1 :         \
  (x) >= 1 ? 0 :          \
  0                      \
)

#define PI  3.14159265
#define TAU 6.28318520

u8 log10_u64(u64 x) {
  if(x < 10)
    return 0;
  
  u8 y = 0;
  while(x)
    ++y, x /= 10;
  
  return y - 1;
}

u64 exp2_u64(u64 x) {
  return 1 << x;
}

u64 exp10_u64(u64 x) {
  u64 z = 1;
  while(x) {
    z *= 10;
    --x;
  }
  
  return z;
}

#endif // MATHSTUFF_H
