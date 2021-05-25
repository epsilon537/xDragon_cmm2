#include "ARMCFunctions.h"
#include "df_defines.h"

void CFUNC_RAM_initEntry(long long *entry, long long *entryIndexp) {
    CFuncRam[*entryIndexp] = (int)entry;
}
