#include "ARMCFunctions.h"
#include "df_defines.h"

void TE_checkColMapCSUB(long long *obj_idp, long long *xp, long long *yp, long long *res) {
  long long *obj_flagsp = (long long*)CFuncRam[CFUNC_RAM_IDX_OBJ_FLAGS];
  long long *box_corner_xp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_CORNER_X];
  long long *box_corner_yp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_CORNER_Y];
  long long *obj_position_xp = (long long *)CFuncRam[CFUNC_RAM_IDX_OBJ_POSITION_X];
  long long *obj_position_yp = (long long *)CFuncRam[CFUNC_RAM_IDX_OBJ_POSITION_Y];
  long long *box_horizontalp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_HORIZONTAL];
  long long *box_verticalp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_VERTICAL];
  unsigned long long *te_map = (unsigned long long*)CFuncRam[CFUNC_RAM_IDX_TE_MAP];
  long long *te_tbl = (long long*)CFuncRam[CFUNC_RAM_IDX_TE_TBL];

  int obj_id = (int)*obj_idp;
  int objBox = (int)(((obj_flagsp[obj_id]) & OBJ_FLG_BOX_MSK) >> OBJ_FLG_BOX_SHFT);
  int xs = (int)(box_corner_xp[objBox] + obj_position_xp[obj_id]);
  int ys = (int)(box_corner_yp[objBox] + obj_position_yp[obj_id]);
  int xe = (xs + (int)(box_horizontalp[objBox]) - 1)/TE_TILE_SUBPIXEL_W;
  int ye = (ys + (int)(box_verticalp[objBox]) - 1)/TE_TILE_SUBPIXEL_H;
  
  int te_max_map_width = (int)(te_tbl[TE_TABLE_MAX_MAP_WIDTH]);
  unsigned te_mapXmask = (unsigned)(te_tbl[TE_TABLE_MAP_XMASK]);
  unsigned te_mapYmask = (unsigned)(te_tbl[TE_TABLE_MAP_YMASK]);
  
  xs /= TE_TILE_SUBPIXEL_W;
  ys /= TE_TILE_SUBPIXEL_H;

  for (int y=ys; y<=ye; y++) {
    for (int x=xs; x<=xe; x++) {
      if ((te_map[te_max_map_width*(y&te_mapYmask) + (x&te_mapXmask)] & TILE_COL_MASK) 
          != TILE_COL_MASK) {
        *res = 1;
        return;
      } 
    }
  }
    
  *res = 0;  
}

