#include "ARMCFunctions.h"
#include "df_defines.h"

void TE_tilePageCSUB(long long *screenStartYp, long long *screenStartXp, 
  long long *startTileXp, long long *startTileYp,
  long long *endTileXp, long long *endTileYp,
  char *callout_sub, long long *callout_args) {

  int startTileY = (int)(*startTileYp);
  int startTileX = (int)(*startTileXp);
  int endTileY = (int)(*endTileYp);
  int endTileX = (int)(*endTileXp);
  
  unsigned long long *te_map = (unsigned long long*)CFuncRam[CFUNC_RAM_IDX_TE_MAP];
  long long *te_tbl = (long long*)CFuncRam[CFUNC_RAM_IDX_TE_TBL];
  int te_max_map_width = (int)(te_tbl[TE_TABLE_MAX_MAP_WIDTH]);
  unsigned te_mapXmask = (unsigned)(te_tbl[TE_TABLE_MAP_XMASK]);
  unsigned te_mapYmask = (unsigned)(te_tbl[TE_TABLE_MAP_YMASK]);
  
  int screenY = (int)(*screenStartYp);
  int screenStartX = (int)(*screenStartXp);
  
  for (int tileY = startTileY; tileY<=endTileY; tileY++) {
    int screenX = screenStartX;
    for (int tileX = startTileX; tileX<=endTileX; tileX++) {
      long long tileId = te_map[te_max_map_width*(tileY&te_mapYmask) + (tileX&te_mapXmask)];

      callout_args[0] = (tileId & 0xff)<<TE_TILE_PIXEL_W_SHIFT;
      callout_args[1] = (tileId & 0xff00)>>(8-TE_TILE_PIXEL_H_SHIFT);
      callout_args[2] = screenX;
      callout_args[3] = screenY;

      RunBasicSub(callout_sub+1);

      screenX += (TE_TILE_SUBPIXEL_W/16);
    }
  
    screenY += (TE_TILE_SUBPIXEL_H/16);
  }
}

