#include "ARMCFunctions.h"
#include "df_defines.h"

/*IMPORTANT: This function uses CFuncRAM entries 0-2*/

void boxIntersectBox(long long* box_a_p, long long *box_b_p, long long *resp) {
  int box_a = (int)(*box_a_p);
  int box_b = (int)(*box_b_p);

  long long *box_corner_xp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_CORNER_X];
  long long *box_corner_yp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_CORNER_Y];

  long long *box_horizontalp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_HORIZONTAL];
  long long *box_verticalp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_VERTICAL];

  int ax1 = (int)box_corner_xp[box_a];
  int bx1 = (int)box_corner_xp[box_b];
  int ay1 = (int)box_corner_yp[box_a];
  int by1 = (int)box_corner_yp[box_b];
    
  /*Test horizontal overlap*/
  int xres = ((bx1<=ax1) && (ax1 <= bx1 + (int)(box_horizontalp[box_b]))) || ((ax1<=bx1) && (bx1 <= ax1 + (int)(box_horizontalp[box_a])));
  /*Test vertical overlap*/
  int yres = ((by1<=ay1) && (ay1 <= by1 + (int)(box_verticalp[box_b]))) || ((ay1<=by1) && (by1 <= ay1 + (int)(box_verticalp[box_a])));

  *resp = xres && yres;
}

