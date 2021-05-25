#include "ARMCFunctions.h"

/*Copy a block of 8-byte words from one block with offset to another with offset.*/
void blockCopyWords(long long *to, long long *to_offset, long long *from, long long *from_offset, long long *numWords) {
  long long *toptr = to + *to_offset;
  long long *fromptr = from + *from_offset;
  long long *toEndPtr = toptr + *numWords;

  while (toptr < toEndPtr)
    *(toptr++) = *(fromptr++);
}

