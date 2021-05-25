#ifndef LOGGER_H
#define LOGGER_H

#define writeLog(a,b,c) \
  ((long long*)(CFuncRam[CFUNC_RAM_IDX_LM_LOG_TBL]))[0] = (a); \
  ((long long*)(CFuncRam[CFUNC_RAM_IDX_LM_LOG_TBL]))[1] = (b); \
  ((long long*)(CFuncRam[CFUNC_RAM_IDX_LM_LOG_TBL]))[2] = (c); \
  RunBasicSub(((char*)(CFuncRam[CFUNC_RAM_IDX_LM_LOG_CALLOUT]))+1);

#endif /*LOGGER*/
