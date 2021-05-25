#include "ARMCFunctions.h"

/*Extract a string of given length from a block of bytes, with offset.*/
void stringExtract(char *s, long long *from, long long *from_offset, long long *numBytesp) {
	char *fromptr = ((char*)from) + *from_offset;
	int ii=0, numBytes = (int)(*numBytesp);

	*s++ = (char)numBytes;
	
	while (ii++<numBytes)
		*s++ = *fromptr++;
}