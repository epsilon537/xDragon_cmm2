#include "ARMCFunctions.h"
#include "df_defines.h"

void CFUNC_RAM_initStrEntry(char *entry, long long *entryIndexp) {
    CFuncRam[*entryIndexp] = (int)entry;
}
