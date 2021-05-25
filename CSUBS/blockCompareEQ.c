#include "ARMCFunctions.h"

/*Byte compare two memory blocks. Returns non-zero in res if equal.*/
void blockCompareEQ(long long *lhs, long long *rhs, long long *numBytes, long long *res) {
	int numWords = (*numBytes)/8;
	int remainder = (*numBytes)&7;
	char *lptr = (char*)(lhs+numWords);
	char *rptr = (char*)(rhs+numWords);
	int ii;

	for (ii=0; ii<numWords; ii++) {
		if (lhs[ii] != rhs[ii]) {
			*res = 0;
			return;
		}
	}
	
	for (ii=0; ii<remainder; ii++) {
		if (lptr[ii] != rptr[ii]) {
			*res = 0;
			return;
		}
	}

	*res = 1;
	return;
}