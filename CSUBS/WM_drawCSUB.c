#include "ARMCFunctions.h"
#include "df_defines.h"
#include "logger.h"

/*IMPORTANT: This function uses CFuncRAM entries 0-6*/

void WM_drawCSUB(long long *visible_objects_lids, long long *wm_viewp, char *callout_sub, long long *callout_arg) {
  long long *box_corner_xp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_CORNER_X];
  long long *box_corner_yp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_CORNER_Y];
  long long *box_horizontalp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_HORIZONTAL];
  long long *box_verticalp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_VERTICAL];
  long long *objList_numElemsp = (long long*)CFuncRam[CFUNC_RAM_IDX_OBJLIST_NUMELEMS];
  long long *objList_listp = (long long *)CFuncRam[CFUNC_RAM_IDX_OBJLIST_LIST];
  long long *obj_flagsp = (long long*)CFuncRam[CFUNC_RAM_IDX_OBJ_FLAGS];
  long long *obj_position_xp = (long long *)CFuncRam[CFUNC_RAM_IDX_OBJ_POSITION_X];
  long long *obj_position_yp = (long long *)CFuncRam[CFUNC_RAM_IDX_OBJ_POSITION_Y];

  int wm_view = (int)(*wm_viewp);
  int bx = (int)(box_corner_xp[wm_view]);
  int by = (int)(box_corner_yp[wm_view]);

  /*According to altitude*/
  for (int alt=0; alt<=MAX_ALTITUDE; alt++) {
    int visible_objects_lid = (int)(visible_objects_lids[alt]);
    long long *visibile_objListp = &objList_listp[MAX_OBJ_LIST_SIZE*visible_objects_lid];

    for (int index=0; index<objList_numElemsp[visible_objects_lid]; index++) {
      int current_object = (int)(visibile_objListp[index]);
      long long obj_flags = obj_flagsp[current_object];

      /*Bounding boxes are relative to object, so convert to world coorindates*/
      int current_box = (int)((obj_flags & OBJ_FLG_BOX_MSK) >> OBJ_FLG_BOX_SHFT);
      int ax = (int)(box_corner_xp[current_box] + obj_position_xp[current_object]);
      int ay = (int)(box_corner_yp[current_box] + obj_position_yp[current_object]);
        
      /*Only draw if Object would be visible on window*/ 
      /*Test horizontal overlap*/
      int xres = ((bx<=ax) && (ax <= bx + (int)(box_horizontalp[wm_view]))) || ((ax<=bx) && (bx <= ax + (int)(box_horizontalp[current_box])));
      /*Test vertical overlap*/
      int yres = ((by<=ay) && (ay <= by + (int)(box_verticalp[wm_view]))) || ((ay<=by) && (by <= ay + (int)(box_verticalp[current_box])));

      if ((xres && yres) || (obj_flags & OBJ_FLG_PERSIST_MSK)) {
        *callout_arg = current_object;
        RunBasicSub(callout_sub+1);
      }      
    }
  }
}

