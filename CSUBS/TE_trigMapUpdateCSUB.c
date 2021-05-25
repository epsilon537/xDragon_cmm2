#include "ARMCFunctions.h"
#include "df_defines.h"

/*Internal function called by TE_tileMap.
 *Update view position in trigger map to tileX%/Y% (tile units).
 *Wraps as needed. Generates EVT_TE_TRIG events when 'trigger tiles' scroll into view*/
void TE_trigMapUpdateCSUB(long long *tileXp, long long *tileYp, 
  char *callout_sub, long long *callout_args) {
  long long *box_horizontalp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_HORIZONTAL];
  long long *box_verticalp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_VERTICAL];
  unsigned long long *te_map = (unsigned long long*)CFuncRam[CFUNC_RAM_IDX_TE_MAP];
  long long *te_tbl = (long long*)CFuncRam[CFUNC_RAM_IDX_TE_TBL];
  int trigMapView = (int)(te_tbl[TE_TABLE_TRIGMAP_VIEW]);
  int trigMapViewVer = (int)(box_verticalp[trigMapView]);
  int trigMapViewHor = (int)(box_horizontalp[trigMapView]);

  int te_max_map_width = (int)(te_tbl[TE_TABLE_MAX_MAP_WIDTH]);
  unsigned te_mapXmask = (unsigned)(te_tbl[TE_TABLE_MAP_XMASK]);
  unsigned te_mapYmask = (unsigned)(te_tbl[TE_TABLE_MAP_YMASK]);

  int tileX = (int)(*tileXp);
  int tileY = (int)(*tileYp);
  int prevTileX = (int)(te_tbl[TE_TABLE_TRIGMAP_PREV_TILEX]);
  int prevTileY = (int)(te_tbl[TE_TABLE_TRIGMAP_PREV_TILEY]);
  int deltaX = tileX - prevTileX;
  int deltaY = tileY - prevTileY;
  
  int startTx, startTy, endTx, endTy;
  
  if ((!deltaX) && (!deltaY))
    return;
  
  if ((ABS(deltaX)<=1) && (ABS(deltaY)<=1)) {
    //We're transitioning from one tile to the next on the right side of the viewport.
    if (deltaX == 1) {
      startTx = tileX + trigMapViewHor-1;
      endTx = startTx;
      startTy = tileY;
      endTy = tileY + trigMapViewVer-1;
    }
    else if (deltaX == -1) {
      //We're transitioning from one tile to the next on the left side of the viewport.
      startTx = tileX;
      endTx = startTx;
      startTy = tileY;
      endTy = tileY + trigMapViewVer-1;
    }
    else if (deltaY == 1) {
      //We're transitioning from one tile to the next on the bottom of the viewport.
      startTx = tileX;
      endTx = tileX + trigMapViewHor-1;
      startTy = tileY + trigMapViewVer-1;
      endTy = startTy;
    }
    else /* (deltaY == -1) */ {
      //We're transitioning from one tile to the next on the top of the viewport.
      startTx = tileX;
      endTx = tileX+trigMapViewHor-1;
      startTy = tileY;
      endTy = startTy;
    }
    
    for (int ty=startTy; ty<=endTy; ty++) {
      for (int tx=startTx; tx<=endTx; tx++) {
        unsigned long long trigId = 
          te_map[te_max_map_width*(ty&te_mapYmask) + (tx&te_mapXmask)] & TILE_TRIG_MASK;
        if (trigId != TILE_TRIG_MASK) { //a non-empty tile?
          trigId >>= TILE_TRIG_SHIFT;
          callout_args[0] = tx;
          callout_args[1] = ty;
          callout_args[2] = trigId;

          RunBasicSub(callout_sub+1);
        }
      }
    }
  }
  else {
    for (long long ty=tileY; ty < (tileY + box_verticalp[trigMapView]); ty++) {
      for (long long tx=tileX; tx< (tileX + box_horizontalp[trigMapView]); tx++) {
        unsigned long long trigId = 
          te_map[te_max_map_width*(ty&te_mapYmask) + (tx&te_mapXmask)] & TILE_TRIG_MASK;
        if (trigId != TILE_TRIG_MASK) { //a non-empty tile?
          //Was if off-screen before?
          long long tmp = (tx & te_mapXmask) - prevTileX;
          if ((tmp>=0) && (tmp < box_horizontalp[trigMapView]))
            continue;
          
          tmp = (ty & te_mapYmask) - prevTileY;
          if ((tmp>=0) && (tmp < box_verticalp[trigMapView]))
            continue;
    
          trigId>>= TILE_TRIG_SHIFT;
          callout_args[0] = tx;
          callout_args[1] = ty;
          callout_args[2] = trigId;

          RunBasicSub(callout_sub+1);
        }
      }
    }
  }

  te_tbl[TE_TABLE_TRIGMAP_PREV_TILEX] = tileX;
  te_tbl[TE_TABLE_TRIGMAP_PREV_TILEY] = tileY;
}

