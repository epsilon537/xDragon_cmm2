'Public constants
'----------------
#define "EVT_COL_MAX_NUM%","1000" 'Max. num. objects that can exist in the game.
CONST EVT_COL_OBJ2_IS_TILE% = OBJ_MAX_NUM% 'Obj.2 is set to this value in case of a tile collision.

'Read-Only variables
'-------------------
DIM evt_col_numAllocated%=0 'Used to track resource leaks

'Private variables and constants
'-------------------------------
'Event flags decoding masks and shifts
#define "_EVT_COL_FLG_ALLOCATED_MSK%","1"   'Collision Event Object is allocated flag
#define "_EVT_COL_FLG_ALLOCATED_SHFT%","0"
#define "_EVT_COL_FLG_OBJ1_MSK%","(1023<<1)"   'Object 1 mask and shift
#define "_EVT_COL_FLG_OBJ1_SHFT%","1"
#define "_EVT_COL_FLG_OBJ2_MSK%","(1023<<11)"   'Object 2 mask and shift, if applicable
#define "_EVT_COL_FLG_OBJ2_SHFT%","11"

'Various collision state values packed into an int. See mask and shift values above.
DIM _evt_col_flags%(EVT_COL_MAX_NUM%-1) 
DIM _evt_col_posX%(EVT_COL_MAX_NUM%-1) 'Collision position
DIM _evt_col_posY%(EVT_COL_MAX_NUM%-1) 'Collision position

'Index into table above used to find the next available (unallocated) object 
DIM _evt_col_allocIdx%=0

'Public subs and functions
'-------------------------

'Allocate a collision event object from the heap and initialize it with given params. 
'Returns an Event object.
FUNCTION EVT_COL_create%(objId1%, objId2%, xx%, yy%)
  LOCAL ii% = _evt_col_allocIdx%
  
  DO
    IF _evt_col_flags%(ii%) = 0 THEN 'Free entry found
      'Allocate, set obj. ids.
      _evt_col_flags%(ii%) = (objId1%<<_EVT_COL_FLG_OBJ1_SHFT%) OR (objId2%<<_EVT_COL_FLG_OBJ2_SHFT%) OR 1
      'Position
      _evt_col_posX%(ii%) = xx%
      _evt_col_posY%(ii%) = yy%

      'Update index to next hopefully free entry
      _evt_col_allocIdx% = ii% + 1
      IF _evt_col_allocIdx% >= EVT_COL_MAX_NUM% THEN
        _evt_col_allocIdx%=0
      ENDIF
    
      'Create base event object
      LOCAL ev% = EVT_create%()      
      EVT_setType(ev%, EVT_COL%)
      'Set collision event obj.id as event data.
      EVT_setData(ev%, ii%)

      INC evt_col_numAllocated%  
      EVT_COL_create% = ev%
      EXIT FUNCTION 
    ENDIF
    
    INC ii%
    IF ii% >= EVT_COL_MAX_NUM% THEN
      ii%=0
    ENDIF
  LOOP UNTIL ii% = _evt_col_allocIdx%
  
  ERROR "Collision Event Object Heap exhausted!"
END FUNCTION

'Destroy given collision event object, releasing all resources used by this object.
SUB EVT_COL_destroy(ev%)
  'Collision event obj id is stored in base event data
  LOCAL eoId% = EVT_getData%(ev%)
  
  IF _evt_col_flags%(eoId%) = 0 THEN
    ERROR "Attempt to free an Collision Event Object that was not allocated."
  ENDIF
  
  _evt_col_flags%(eoId%) = 0
  
  INC evt_col_numAllocated%, -1  
END SUB

'Returns non-zero if event object is allocated.
FUNCTION EVT_COL_isAllocated%(ev%)
  'Collision event obj id is stored in base event data
  LOCAL eoId% = EVT_getData%(ev%)

  EVT_COL_isAllocated% = _evt_col_flags%(eoId%)
END FUNCTION

'Set object that caused collision.
SUB EVT_COL_setObj1(ev%, objId%)
  'Collision event obj id is stored in base event data
  LOCAL eoId% = EVT_getData%(ev%)
  setBitField _evt_col_flags%(eoId%), _EVT_COL_FLG_OBJ1_MSK%, _EVT_COL_FLG_OBJ1_SHFT%, objId%
END SUB

'Return object that cause collision.
FUNCTION EVT_COL_getObj1%(ev%)
  'Collision event obj id is stored in base event data
  LOCAL eoId% = EVT_getData%(ev%)
  getBitField _evt_col_flags%(eoId%), _EVT_COL_FLG_OBJ1_MSK%, _EVT_COL_FLG_OBJ1_SHFT%, EVT_COL_getObj1%
END FUNCTION

'Set object that was collided with.
SUB EVT_COL_setObj2(ev%, objId%)
  'Collision event obj id is stored in base event data
  LOCAL eoId% = EVT_getData%(ev%)
  setBitField _evt_col_flags%(eoId%), _EVT_COL_FLG_OBJ2_MSK%, _EVT_COL_FLG_OBJ2_SHFT%, objId%
END SUB

'Return object that was collided with.
FUNCTION EVT_COL_getObj2%(ev%)
  'Collision event obj id is stored in base event data
  LOCAL eoId% = EVT_getData%(ev%)
  getBitField _evt_col_flags%(eoId%), _EVT_COL_FLG_OBJ2_MSK%, _EVT_COL_FLG_OBJ2_SHFT%, EVT_COL_getObj2%
END FUNCTION

'Set position of collision
SUB EVT_COL_setPos(ev%, xx%, yy%)
  'Collision event obj id is stored in base event data
  LOCAL eoId% = EVT_getData%(ev%)
  _evt_col_posX%(eoId%) = xx%
  _evt_col_posY%(eoId%) = yy%
END SUB

'Get x-position of collision
FUNCTION EVT_COL_getPosX%(ev%)
  'Collision event obj id is stored in base event data
  LOCAL eoId% = EVT_getData%(ev%)
  EVT_COL_getPosX% = _evt_col_posX%(eoId%)
END FUNCTION

'Get y-position of collision
FUNCTION EVT_COL_getPosY%(ev%)
  'Collision event obj id is stored in base event data
  LOCAL eoId% = EVT_getData%(ev%)
  EVT_COL_getPosY% = _evt_col_posY%(eoId%)
END FUNCTION

'Get x/y-position of collision
SUB EVT_COL_getPos(ev%, xx%, yy%)
  LOCAL eoId% = EVT_getData%(ev%)
  xx% = _evt_col_posX%(eoId%)  
  yy% = _evt_col_posY%(eoId%)  
END SUB

