#include "ARMCFunctions.h"
#include "df_defines.h"

//Internal rendering function. Using pixel units!
void BG_renderCSUB(long long *srcXp, long long *srcYp, long long *dstXp, long long *dstYp, 
  long long *wp, long long *hp, char *callout_sub, long long *callout_args) {
  long long *bg_tbl = (long long*)CFuncRam[CFUNC_RAM_IDX_BG_TBL];
  int bg_width = (int)(bg_tbl[BG_TABLE_WIDTH])>>4;
  int bg_height = (int)(bg_tbl[BG_TABLE_HEIGHT])>>4;
  int srcX = (int)(*srcXp);
  int srcY = (int)(*srcYp);
  int dstX = (int)(*dstXp);
  int dstY = (int)(*dstYp);
  int w = (int)(*wp);
  int h = (int)(*hp);
  int srcXend = (srcX + w) % bg_width;
  int srcYend = (srcY + h) % bg_height;
  
  int y = srcY;

  while (y!=srcYend) {  
    int hh = (y>srcYend ? bg_height : srcYend) - y;
    int x = srcX;      

    callout_args[1] = y;
    callout_args[3] = dstY;
    callout_args[5] = hh;
    
    while (x!=srcXend) {
      int ww = (x>srcXend ? bg_width : srcXend) - x;
      
      callout_args[0] = x;
      callout_args[2] = dstX;
      callout_args[4] = ww;

      RunBasicSub(callout_sub+1);

      dstX += ww;      
      x += ww;
      x %= bg_width;
    }
    
    dstY += hh;
    y += hh;
    y %= bg_height;
  }
}

