'Public constants
'----------------
CONST FRAME_MAX_NUM% = 200 'Maximum number of frames that can simultaneously exist game-wide.

'Read-Only variables
'-------------------
'Used to track resource leaks
DIM frame_numAllocated%=0

'Private variables and constants
'-------------------------------
DIM _frame_x%(FRAME_MAX_NUM%-1)
DIM _frame_y%(FRAME_MAX_NUM%-1)
DIM _frame_w%(FRAME_MAX_NUM%-1)
DIM _frame_h%(FRAME_MAX_NUM%-1)
DIM _frame_page%(FRAME_MAX_NUM%-1) 'A page_id of 0 indicates not allocated.
DIM _frame_ref%(FRAME_MAX_NUM%-1) 'A reference counter

'Index into tables above used to find the next available (unallocated) frame object
DIM _frame_allocIdx%=0

'Public subs and functions
'-------------------------

'Create a frame object of indicated width, height, and position on given page.
'Note that for consistency, .4 subpixel units are used, just like everywhere else in the engine.
'The assumption is that xx,yy,ww and hh don't have a fractional portion.
FUNCTION Frame_create%(xx%, yy%, ww%, hh%, pageId%)
  LOCAL ii% = _frame_allocIdx%
  
  DO
    IF _frame_page%(ii%) = 0 THEN 'Free entry found
    
      'Allocate and initialize.
      _frame_page%(ii%) = pageId%
      _frame_x%(ii%) = xx%
      _frame_y%(ii%) = yy%
      _frame_w%(ii%) = ww%
      _frame_h%(ii%) = hh%
      _frame_ref%(ii%) = 0
            
      'Update index to next hopefully free entry
      _frame_allocIdx% = ii% + 1
      IF _frame_allocIdx% >= FRAME_MAX_NUM% THEN
        _frame_allocIdx%=0
      ENDIF
      
      INC frame_numAllocated%
    
      'Also increment Sheet reference counter
      INC sheet_refCount%(pageId%)
      
      Frame_create% = ii%
      EXIT FUNCTION 
    ENDIF
    
    INC ii%
    IF ii% >= FRAME_MAX_NUM% THEN
      ii%=0
    ENDIF
  LOOP UNTIL ii% = _frame_allocIdx%
  
  ERROR "Frames Heap exhausted!"
END FUNCTION

'Destroy given frame object, releasing all resources used by this object.
SUB Frame_destroy(frid%)
  IF _frame_page%(frid%) = 0 THEN
    ERROR "Attempt to destroy an Frame Object that was not allocated."
  ENDIF

  IF _frame_ref%(frid%) <> 0 THEN
    ERROR "Attempt to destroy an Frame Object that's still being referenced."
  ENDIF
  
  'Decrement Sheet reference counter
  INC sheet_refCount%(_frame_page%(frid%)), -1

  _frame_page%(frid%) = 0  
  INC frame_numAllocated%, -1  
END SUB

'Indicate that this frame is being referenced. Bumps the reference count.
SUB Frame_ref(frid%)
  INC _frame_ref%(frid%)
END SUB

'Indicate that this frame is no longer referenced. If the reference drops to 0, destroy the frame
SUB Frame_unref(frid%)
  INC _frame_ref%(frid%), -1
  IF _frame_ref%(frid%)=0 THEN
    Frame_destroy(frid%)
  ENDIF
END SUB

'Draw frame, centered at position (unit: .4 subpixels)
SUB Frame_draw(frid%, xx%, yy%)
  LOCAL ww% = _frame_w%(frid%)
  LOCAL hh% = _frame_h%(frid%)
  LOCAL fromx% = _frame_x%(frid%)
  LOCAL fromy% = _frame_y%(frid%)
  LOCAL tox% = xx%, toy% = yy%
  INC tox%, -(ww%\2)-box_cornerX%(wm_view%))
  INC toy%, -(hh%\2)-box_cornerY%(wm_view%))

  'LM_writeLog("Frame draw at "+STR$(tox%)+" "+STR$(toy%))
  'LM_writeLog("Frame draw from "+STR$(fromx%)+" "+STR$(fromy%))
  'LM_writeLog("Frame draw on page "+STR$(DM_getWritePage%()))
  
  'Here we have to convert to real pixels
  BLIT fromx%\16, fromy%\16, tox%\16, toy%\16, ww%>>4, hh%>>4, _frame_page%(frid%), 4 'Don't copy transparent
END SUB  

