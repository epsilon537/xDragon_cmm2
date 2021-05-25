'Public constants
'----------------
CONST BOX_MAX_NUM%=1000

'Read-Write variables
'--------------------
'.4 subpixel units
DIM box_cornerX%(BOX_MAX_NUM%-1) 'Upper left corner x-position
DIM box_cornerY%(BOX_MAX_NUM%-1) 'Upper left corner y-position
DIM box_width%(BOX_MAX_NUM%-1) 'Box width. A value of -1 means not allocated
SUB box_widthInit 'Initializer
  LOCAL ii%
  
  FOR ii%=0 TO BOX_MAX_NUM%-1
    box_width%(ii%) = -1
  NEXT ii%
END SUB
box_widthInit

DIM box_height%(BOX_MAX_NUM%-1) 'Box height.

DIM box_numAllocated%=0 'Used to track resource leaks

'Private variables and constants
'-------------------------------
'Index into table above used to find the next available (unallocated) box 
DIM _box_allocIdx%=0

'Public subs and functions
'-------------------------

'Allocate a box from the box heap and initialize it. Returns a box id.
FUNCTION Box_create%(xx%, yy%, ww%, hh%)
  LOCAL ii% = _box_allocIdx%
  
  DO
    IF box_width%(ii%) = -1 THEN 'Free entry found
      'Allocate and set attributes
      box_width%(ii%) = ww%
      box_height%(ii%) = hh%
      box_cornerX%(ii%) = xx%
      box_cornerY%(ii%) = yy%

      'Update index to next hopefully free entry
      _box_allocIdx% = ii% + 1
      IF _box_allocIdx% >= BOX_MAX_NUM% THEN
        _box_allocIdx%=0
      ENDIF
    
      INC box_numAllocated%  
      Box_create% = ii%
      EXIT FUNCTION 
    ENDIF
    
    INC ii%
    IF ii% >= BOX_MAX_NUM% THEN
      ii%=0
    ENDIF
  LOOP UNTIL ii% = _box_allocIdx%
  
  ERROR "Box Heap exhausted!"
END FUNCTION

'Destroy given box, releasing all resources.
SUB Box_destroy(box%)
  IF box_width%(box%) = -1 THEN
    ERROR "Attempt to free a Box that was not allocated."
  ENDIF
  
  box_width%(box%) = -1
  
  INC box_numAllocated%, -1  
END SUB

'Copy one box's attributes to another
SUB Box_copy(to_box%, from_box%)
  box_cornerX%(to_box%) = box_cornerX%(from_box%)
  box_cornerY%(to_box%) = box_cornerY%(from_box%)
  box_width%(to_box%) = box_width%(from_box%)
  box_height%(to_box%) = box_height%(from_box%)
END SUB

'Test two boxes for equality
FUNCTION Box_eq%(box1%, box2%)
  Box_eq% = (box_cornerX%(box1%) = box_cornerX%(box2%)) AND (box_cornerY%(box1%) = box_cornerY%(box2%))
  Box_eq% = Box_eq% AND (box_width%(box1%) = box_width%(box2%)) AND (box_height%(box1%) = box_height%(box2%))
END FUNCTION

'Scale a box relative to origin with given factor.
SUB Box_scale(box%, scaleFactor!)
  INC box_cornerX%(box%), box_width%(box%)*(1 - scaleFactor!)/2
  INC box_cornerY%(box%), box_height%(box%)*(1 - scaleFactor!)/2

  box_width%(box%) = box_width%(box%)*scaleFactor!
  box_height%(box%) = box_height%(box%)*scaleFactor!
END SUB

