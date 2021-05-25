'Public constants
'----------------
CONST OBJ_MAX_ALTITUDE% = 3 'Number of altitude levels -1, used to determine ordering object drawing.
CONST OBJ_MAX_COLLISION_GROUP% = 6 'Number of support collision groups -1.

CONST OBJ_MAX_NUM% = 1000 'Max. num. objects that can simultaneously exist in the game.

'Types of object solidness
CONST OBJ_HARD% = 0 'Hard objects can not move into other hard objects.
CONST OBJ_SOFT% = 1 'Soft objects can move into/through other hard or soft objects.

'Object flag bitfield definitions
CONST OBJ_FLG_ALLOCATED_MSK% = 1   'Object is allocated flag
CONST OBJ_FLG_ALLOCATED_SHFT% = 0
CONST OBJ_FLG_TYPE_MSK% = (255<<1) 'Object Type
CONST OBJ_FLG_TYPE_SHFT% = 1
CONST OBJ_FLG_ALT_MSK% = (15<<9) 'Object altitude
CONST OBJ_FLG_ALT_SHFT% = 9
CONST OBJ_FLG_SOLID_MSK% = (3<<13) 'Object solidness
CONST OBJ_FLG_SOLID_SHFT% = 13
CONST OBJ_FLG_BOX_MSK% = (1023<<15) 'Object bounding box
CONST OBJ_FLG_BOX_SHFT% = 15
CONST OBJ_FLG_CG_MSK% = (7<<25) 'Object collision group id
CONST OBJ_FLG_CG_SHFT% = 25
CONST OBJ_FLG_ACT_MSK% = (1<<28) 'Object active flag
CONST OBJ_FLG_ACT_SHFT% = 28
CONST OBJ_FLG_PERSIST_MSK% = (1<<29) 'Object persistent flag
CONST OBJ_FLG_PERSIST_SHFT% = 29

CONST OBJ_FLG_XTR_MSK% = ((&HFFFFFFFF)<<32) 'extra data for sub objects
CONST OBJ_FLG_XTR_SHFT% = 32

'Private variables and constants
'-------------------------------
DIM _obj_registeredEvents%(OBJ_MAX_NUM%-1) 'Bitmap of registered events.
'Index into table above used to find the next available (unallocated) object.
DIM _obj_allocIdx%=0

'Read-Write variables
'--------------------
DIM obj_destroySub$(OBJ_MAX_NUM%-1) LENGTH 32 'Derived Object virtual destructor sub name.
DIM obj_drawSub$(OBJ_MAX_NUM%-1) LENGTH 32 'Derived Object virtual draw sub name
DIM obj_eventH$(OBJ_MAX_NUM%-1) LENGTH 32 'Derived Object virtual eventHandler function name
DIM obj_velX%(OBJ_MAX_NUM%-1) 'Object horizontal velocity. Unit: .4 subpixels
DIM obj_velY%(OBJ_MAX_NUM%-1) 'Object vertical velocity. Unit: .4 subpixels
DIM obj_posX%(OBJ_MAX_NUM%-1) 'Object horizontal position. Unit: .4 subpixels
DIM obj_posY%(OBJ_MAX_NUM%-1) 'Object vertical position. Unit: .4 subpixels
DIM obj_flags%(OBJ_MAX_NUM%-1) 'Various object state values packed into an int. See mask and shift values above.

'Read-Only variables
'-------------------
DIM obj_anim%(OBJ_MAX_NUM%-1) 'Animation associated with object.

DIM obj_numAllocated%=0 'Used to track resource leaks

'Public subs and functions
'-------------------------

'Allocate an object from the object heap and initialize it. Returns an object id.
FUNCTION Obj_create%()
  LOCAL ii% = _obj_allocIdx%
  
  DO
    IF obj_flags%(ii%) = 0 THEN 'Free entry found
      'Allocate, set default type (OBJECT), altitude (1/2 of max), solidness (hard), col. group id 0 (spectral), active, not persistent
      obj_flags%(ii%) = ((OBJ_MAX_ALTITUDE%\2)<<OBJ_FLG_ALT_SHFT%) OR (OBJ_TYPE_OBJECT%<<OBJ_FLG_TYPE_SHFT%) OR OBJ_FLG_ACT_MSK% OR 1
      'Empty anim object
      obj_anim%(ii%) = Anim_create%()
      'Default position to (0,0)
      obj_posX%(ii%) = 0: obj_posY%(ii%) = 0
      'Default velocity is also (0,0)
      obj_velX%(ii%) = 0: obj_velY%(ii%) = 0
      'Default no registered events
      _obj_registeredEvents%(ii%) = 0      
      
      'Register the defaults methods
      obj_destroySub$(ii%) = "Obj_defaultDestroy"
      obj_drawSub$(ii%) = "Obj_defaultDraw"
      obj_eventH$(ii%) = "Dummy_fun_iii%"

      'Default bounding box 1x1 pixel in size, corner 0,0 (obj. relative)
      LOCAL box_id% = Box_create%(0, 0, 16, 16)
      setBitField obj_flags%(ii%), OBJ_FLG_BOX_MSK%, OBJ_FLG_BOX_SHFT%, box_id%
      
      'Update index to next hopefully free entry
      _obj_allocIdx% = ii% + 1
      IF _obj_allocIdx% >= OBJ_MAX_NUM% THEN
        _obj_allocIdx%=0
      ENDIF
    
      SG_insertObj(ii%) 'Add self to game world.
      
      INC obj_numAllocated%  
      Obj_create% = ii%
      EXIT FUNCTION 
    ENDIF
    
    INC ii%
    IF ii% >= OBJ_MAX_NUM% THEN
      ii%=0
    ENDIF
  LOOP UNTIL ii% = _obj_allocIdx%
  
  ERROR "Object Heap exhausted!"
END FUNCTION

'Destroy given object, releasing all resources used by this object.
SUB Obj_destroy(objId%)
  LM_writeLog("Deleting obj: "+STR$(objId%))
  IF obj_flags%(objId%) = 0 THEN
    ERROR "Attempt to free an Object that was not allocated. Obj.id: "+STR$(objId%))
  ENDIF

  CALL obj_destroySub$(objId%), objId%  
END SUB

'Object default destructor sub. Call this from derived object virtual destructor.
SUB Obj_defaultDestroy(objId%)
  SG_removeObj objId%

  LOCAL box_id%
    
  getBitField obj_flags%(objId%), OBJ_FLG_BOX_MSK%, OBJ_FLG_BOX_SHFT%, box_id%

  Box_destroy(box_id%)
    
  obj_flags%(objId%) = 0
  
  'Remove event registration.
  LOCAL ii% = (MAX_NUM_EVT_TYPES%-1)
  DO WHILE ii% <> 0
    IF (_obj_registeredEvents%(objId%) AND (1<<ii%)) <> 0 THEN
      dummy_i% = EVT_MGR_unregisterInterest%(objId%, ii%)
    ENDIF
    
    INC ii%, -1
  LOOP
    
  INC obj_numAllocated%, -1  
END SUB

'Sets object type. See ObjectTypes.inc.
SUB Obj_setType(objId%, type%)
  setBitField obj_flags%(objId%), OBJ_FLG_TYPE_MSK%, OBJ_FLG_TYPE_SHFT%, type%
END SUB

'Gets object type. See ObjectTypes.inc.
FUNCTION Obj_getType%(objId%)
  getBitField obj_flags%(objId%), OBJ_FLG_TYPE_MSK%, OBJ_FLG_TYPE_SHFT%, Obj_getType%
END FUNCTION

'Set object altitude, used to determine drawing order of objects (does not affect collisions detection).
SUB Obj_setAlt(objId%, alt%)
  SG_updateAlt objId%, alt%
  setBitField obj_flags%(objId%), OBJ_FLG_ALT_MSK%, OBJ_FLG_ALT_SHFT%, alt%
END SUB

'Gets object altitude.
FUNCTION Obj_getAlt%(objId%)
  getBitField obj_flags%(objId%), OBJ_FLG_ALT_MSK%, OBJ_FLG_ALT_SHFT%, Obj_getAlt%
END FUNCTION

'Set object's collision group id. A collission group id. refers to a specific cg_map bitmap which specifies
'with which other collision groups this object can collide.
SUB Obj_setColGroup(objId%, cgId%)
  SG_updateColGroup objId%, cgId%
  setBitField obj_flags%(objId%), OBJ_FLG_CG_MSK%, OBJ_FLG_CG_SHFT%, cgId%  
END SUB

'Get object collision group id.
FUNCTION Obj_getColGroup%(objId%)
  getBitField obj_flags%(objId%), OBJ_FLG_CG_MSK%, OBJ_FLG_CG_SHFT%, Obj_getColGroup%
END FUNCTION

'Set object extra data
SUB Obj_setExtra(objId%, xtr%)
  setBitField obj_flags%(objId%), OBJ_FLG_XTR_MSK%, OBJ_FLG_XTR_SHFT%, xtr%  
END SUB

'Get object extra data.
FUNCTION Obj_getExtra%(objId%)
  getBitField obj_flags%(objId%), OBJ_FLG_XTR_MSK%, OBJ_FLG_XTR_SHFT%, Obj_getExtra%
END FUNCTION

'Set object active state. 1=active, 0=inactive.
SUB Obj_setActive(objId%, activeVal%)
  'Update scene graph
  SG_updateActive objId%, activeVal%
  
  'Set active value
  setBitField obj_flags%(objId%), OBJ_FLG_ACT_MSK%, OBJ_FLG_ACT_SHFT%, activeVal%  
END SUB

'Get object active state. 1=active, 0=inactive.
FUNCTION Obj_isActive%(objId%)
  getBitField obj_flags%(objId%), OBJ_FLG_ACT_MSK%, OBJ_FLG_ACT_SHFT%, Obj_isActive%
END FUNCTION

'Set object persistent state. Persistent objects are drawn without doing a bounds check
'(E.g. ViewObjects).
SUB Obj_setPersist(objId%, persistVal%)  
  setBitField obj_flags%(objId%), OBJ_FLG_PERSIST_MSK%, OBJ_FLG_PERSIST_SHFT%, persistVal%  
END SUB

'Get object persistent state
FUNCTION Obj_isPersist%(objId%)
  getBitField obj_flags%(objId%), OBJ_FLG_PERSIST_MSK%, OBJ_FLG_PERSIST_SHFT%, Obj_isPersist%
END FUNCTION

'Returns non-zero if object is allocated.
FUNCTION Obj_isAllocated%(objId%)
  Obj_isAllocated% = obj_flags%(objId%)
END FUNCTION

'Object event handler function. Dispatching to derived object event handler.
'Returns 0 if event ignored, 1 if handled.
FUNCTION Obj_eventHandler%(objId%, ev%)
  Obj_eventHandler% = CALL(obj_eventH$(objId%), objId%, ev%)
END FUNCTION

'Object default draw method.
SUB Obj_defaultDraw(objId%)
  'LM_writeLog("Default object draw for obj: "+STR$(objId%))
  'LM_writeLog("x = "+HEX$(obj_posX%(objId%))+" y = "+HEX$(obj_posY%(objId%)))
  'default draw method
  Anim_draw(obj_anim%(objId%), obj_posX%(objId%), obj_posY%(objId%))  
END SUB

'Draw self at position. Dispatched to derived object draw method.
SUB Obj_draw(objId%)
  CALL obj_drawSub$(objId%), objId%
END SUB

'Set object solidness (OBJ_HARD or OBJ_SOFT).
SUB Obj_setSolidness(objId%, solidness%)
  setBitField obj_flags%(objId%), OBJ_FLG_SOLID_MSK%, OBJ_FLG_SOLID_SHFT%, solidness%  
END SUB

'Get object solidness.
FUNCTION Obj_getSolidness%(objId%)
  getBitField obj_flags%(objId%), OBJ_FLG_SOLID_MSK%, OBJ_FLG_SOLID_SHFT%, Obj_getSolidness%
END FUNCTION

'Set animation to use for this object.
SUB Obj_setAnim(objId%, label$)
  LOCAL aid% = Anim_find%(label$)
  IF aid% = -1 THEN
    Error "Anim "+label$+" not found."
  ENDIF
  
  obj_anim%(objId%) = aid%

  LOCAL o_box%
  getBitField obj_flags%(objId%), OBJ_FLG_BOX_MSK%, OBJ_FLG_BOX_SHFT%, o_box%

  'Set bounding box to match anim dimensions  
  box_width%(o_box%) = Anim_getWidth%(aid%)
  box_height%(o_box%) = Anim_getHeight%(aid%)
  box_cornerX%(o_box%) = -0.5*box_width%(o_box%)
  box_cornerY%(o_box%) = -0.5*box_height%(o_box%)
END SUB

'Set a variant of animation specified by previous function. All properties remain
'the same, except the Animation ID (aid%)
SUB Obj_setAnimVariant(objId%, aid%)
  obj_anim%(objId%) = aid%
  Anim_reset aid%
END SUB

'Set Object's bounding box, using given box's attributes. Note: Box coordinates are object position relative.
SUB Obj_setBox(objId%, box_id%)
  LOCAL o_box%
  getBitField obj_flags%(objId%), OBJ_FLG_BOX_MSK%, OBJ_FLG_BOX_SHFT%, o_box%

  Box_copy(o_box%, box_id%)
END SUB

'Get Object's bounding box. Note: Box coordinates are object position relative.
FUNCTION Obj_getBox%(objId%)
  getBitField obj_flags%(objId%), OBJ_FLG_BOX_MSK%, OBJ_FLG_BOX_SHFT%, Obj_getBox%
END FUNCTION

'Indicate interest in event.
SUB Obj_registerInterest(objId%, evType%)
  _obj_registeredEvents%(objId%) = _obj_registeredEvents%(objId%) OR (1<<evType%)
  EVT_MGR_registerInterest(objId%, evType%)
END SUB

'Indicate no longer interested in event
'Returns 0 if success, -1 if object not found
FUNCTION Obj_unregisterInterest%(objId%, evType%)
  _obj_registeredEvents%(objId%) = _obj_registeredEvents%(objId%) AND (INV (1<<evType%))
  Obj_unregisterInterest% = EVT_MGR_unregisterInterest%(objId%, evType%)
END FUNCTION

