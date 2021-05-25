#include "ARMCFunctions.h"
#include "df_defines.h"

void WM_updateCSUB(long long *active_objects_lidp, char *callout_sub, long long *callout_arg1, long long *callout_arg2, long long *callout_arg3) {
  long long *objList_numElemsp = (long long*)CFuncRam[CFUNC_RAM_IDX_OBJLIST_NUMELEMS];
  long long *objList_listp = (long long *)CFuncRam[CFUNC_RAM_IDX_OBJLIST_LIST];
  long long *obj_position_xp = (long long *)CFuncRam[CFUNC_RAM_IDX_OBJ_POSITION_X];
  long long *obj_position_yp = (long long *)CFuncRam[CFUNC_RAM_IDX_OBJ_POSITION_Y];

  long long *obj_velocity_xp = (long long *)CFuncRam[CFUNC_RAM_IDX_OBJ_VELOCITY_X];
  long long *obj_velocity_yp = (long long *)CFuncRam[CFUNC_RAM_IDX_OBJ_VELOCITY_Y];

  int active_objects_lid = (int)(*active_objects_lidp);
  long long *active_objListp = (long long*)&objList_listp[MAX_OBJ_LIST_SIZE*active_objects_lid];

  for (int index=0; index<objList_numElemsp[active_objects_lid]; index++) {
    int current_object = (int)(active_objListp[index]);
    //If position changed, move
    if (obj_velocity_xp[current_object] || obj_velocity_yp[current_object]) {
      *callout_arg1 = current_object;
      *callout_arg2 = obj_position_xp[current_object] + obj_velocity_xp[current_object];
      *callout_arg3 = obj_position_yp[current_object] + obj_velocity_yp[current_object];

      RunBasicSub(callout_sub+1);
    }
  }
}

