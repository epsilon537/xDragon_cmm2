#include "ARMCFunctions.h"

/*Copy a block of bytes from one block with offset to another with offset.*/
void blockCopyBytes(long long *to, long long *to_offset, long long *from, long long *from_offset, long long *numBytes) {
  char *toptr = ((char*)to) + *to_offset;
  char *fromptr = ((char*)from) + *from_offset;
  int ii;

  for (ii=0; ii<*numBytes; ii++)
    *(toptr + ii) = *(fromptr + ii);
}

