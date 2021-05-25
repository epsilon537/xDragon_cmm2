#include "ARMCFunctions.h"
#include "df_defines.h"
/*IMPORTANT: This function uses CFuncRAM entries 0-7*/

/*collisions_lid will be set to -1 on error*/
void WM_getCollisionsCSUB(long long *obj_idp, long long *where_xp, long long *where_yp, long long *collidable_objects_lids, long long *collisions_lidp) {
  int collisions_lid = (int)(*collisions_lidp);
  long long *box_corner_xp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_CORNER_X];
  long long *box_corner_yp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_CORNER_Y];
  long long *box_horizontalp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_HORIZONTAL];
  long long *box_verticalp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_VERTICAL];
  long long *objList_numElemsp = (long long*)CFuncRam[CFUNC_RAM_IDX_OBJLIST_NUMELEMS];
  long long *objList_listp = (long long *)CFuncRam[CFUNC_RAM_IDX_OBJLIST_LIST];
  long long *obj_flagsp = (long long*)CFuncRam[CFUNC_RAM_IDX_OBJ_FLAGS];
  long long *obj_position_xp = (long long *)CFuncRam[CFUNC_RAM_IDX_OBJ_POSITION_X];
  long long *obj_position_yp = (long long *)CFuncRam[CFUNC_RAM_IDX_OBJ_POSITION_Y];
  long long *cgmaps = (long long *)CFuncRam[CFUNC_RAM_IDX_CG_MAPS];
  int obj_id = (int)(*obj_idp);
  long long *collisions_objListp = (long long*)&objList_listp[MAX_OBJ_LIST_SIZE*collisions_lid];
  
  int wbox = (int)(((obj_flagsp[obj_id]) & OBJ_FLG_BOX_MSK) >> OBJ_FLG_BOX_SHFT);
  int ax1 = (int)(box_corner_xp[wbox] + *where_xp);
  int ay1 = (int)(box_corner_yp[wbox] + *where_yp);
  int ax2 = ax1 + (int)box_horizontalp[wbox];
  int ay2 = ay1 + (int)box_verticalp[wbox];
  
  int cg = (int)(((obj_flagsp[obj_id]) & OBJ_FLG_CG_MSK) >> OBJ_FLG_CG_SHFT);
  int cgmap = (int)cgmaps[cg];

  /*Iterate through all collision groups in cgmap. Skip entry 0, the spectral group.*/
  for (cg=1; cg<7; cg++) {
    /*Is this collision group selected?*/
    if (cgmap & (1<<cg)) {      
      int col_objects_lid = (int)(collidable_objects_lids[cg]);
      long long *col_objListp = &objList_listp[MAX_OBJ_LIST_SIZE*col_objects_lid];
      int ol_numElems = (int)objList_numElemsp[col_objects_lid];

      /*Iterate through all objects*/
      for (int index=0; index < ol_numElems; ++index) {
        int other_obj_id = (int)col_objListp[index];

        /*Do not consider self*/
        if (other_obj_id == obj_id)
          continue;
        
        int other_wbox_id = 
          (int)(((obj_flagsp[other_obj_id]) & OBJ_FLG_BOX_MSK) >> OBJ_FLG_BOX_SHFT);
        int bx1 = (int)(box_corner_xp[other_wbox_id] + obj_position_xp[other_obj_id]);
        int by1 = (int)(box_corner_yp[other_wbox_id] + obj_position_yp[other_obj_id]);
                
        /*Test horizontal overlap*/
        int xres = 
          ((bx1<=ax1) && (ax1 <= bx1 + (int)(box_horizontalp[other_wbox_id]))) || ((ax1<=bx1) && (bx1<=ax2));
        /*Test vertical overlap*/
        int yres = 
          ((by1<=ay1) && (ay1 <= by1 + (int)(box_verticalp[other_wbox_id]))) || ((ay1<=by1) && (by1<=ay2));
    
        /*Overlapping location*/
        if (xres && yres) {
          int cl_numElems = (int)(objList_numElemsp[collisions_lid]);
          if (cl_numElems >= MAX_OBJ_LIST_SIZE) {
            *collisions_lidp = -1;
            return;
          }
                  
          collisions_objListp[cl_numElems] = other_obj_id;
          ++cl_numElems;      
          objList_numElemsp[collisions_lid] = cl_numElems;
        }
      }
    }
  }
}

