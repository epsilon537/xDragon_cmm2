#include "ARMCFunctions.h"

void delWordsFromBlock(long long *block, long long *blockOffset, long long *blockSizeInWords, long long *numWordsToDelp) {
  long long *tptr = block + *blockOffset;
  long long numWordsToDel = *numWordsToDelp;
  long long *endPtr = block + *blockSizeInWords - numWordsToDel;
  
  while (tptr < endPtr) {
    *tptr = *(tptr+numWordsToDel);
    ++tptr;
  }
}

