#include "ARMCFunctions.h"

void delBytesFromBlock(long long *block, long long *blockOffset, long long *blockSizeInBytes, long long *numBytesToDelp) {
	char *tptr = ((char*)block) + *blockOffset;
	long long numBytesToDel = *numBytesToDelp;
	char *endPtr = ((char*)block) + *blockSizeInBytes - numBytesToDel;
	
	/*byte-by-byte*/
	while (tptr < endPtr) {
		*tptr = *(tptr+numBytesToDel);
		++tptr;
	}
}
