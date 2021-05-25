'Public constants
'----------------

'General location of ViewObject on screen.
CONST VOBJ_LOC_UNDEFINED% = 0 'Position undefined
CONST VOBJ_LOC_TOP_LEFT% = 1 'Top left corner
CONST VOBJ_LOC_TOP_CENTER% = 2 'Top center
CONST VOBJ_LOC_TOP_RIGHT% = 3 'Top right corner
CONST VOBJ_LOC_CENTER_LEFT% = 4 'Center left
CONST VOBJ_LOC_CENTER_CENTER% = 5 'Center
CONST VOBJ_LOC_CENTER_RIGHT% = 6 'Center right
CONST VOBJ_LOC_BOTTOM_LEFT% = 7 'Bottom left corner
CONST VOBJ_LOC_BOTTOM_CENTER% = 8 'Bottom center
CONST VOBJ_LOC_BOTTOM_RIGHT% = 9 'Bottomr right corner

CONST VOBJ_MAX_NUM% = 100 'Max number of ViewObjects that can simultaneously exist in the game.

'Read-Only variables
'-------------------
DIM vobj_numAllocated%=0 'Used to track resource leaks.

'Private variables and constants
'-------------------------------

CONST _VOBJ_FLG_VAL_MSK% = &HFFFFFF 'value flag
CONST _VOBJ_FLG_VAL_SHFT% = 0
CONST _VOBJ_FLG_DRAW_VAL_MSK% = 1<<24 'draw value flag
CONST _VOBJ_FLG_DRAW_VAL_SHFT% = 24
CONST _VOBJ_FLG_BORDER_MSK% = 1<<25 'draw border flag
CONST _VOBJ_FLG_BORDER_SHFT% = 25
CONST _VOBJ_FLG_LOC_MSK% = 15 << 26 'location flag
CONST _VOBJ_FLG_LOC_SHFT% = 26
CONST _VOBJ_FLG_COL_MSK% = &HFFFFFFFF << 32 'color flag
CONST _VOBJ_FLG_COL_SHFT% = 32

DIM _vobj_viewString$(VOBJ_MAX_NUM%-1) LENGTH 40
DIM _vobj_flags%(VOBJ_MAX_NUM%-1)
DIM _vobj_flags2%(VOBJ_MAX_NUM%-1) 'Currently we only store the view event tag here.

DIM _vobj_allocIdx%=0

'View objects are rendered to their own page and from there blitted to the display page
'using dont-copy-black
DIM _vobj_pageId%

'Public subs and functions
'-------------------------

'Start up the ViewObject module
SUB VObj_startUp
  _vobj_pageId% = Sheet_create%()
END SUB

'Shut down the ViewObject module
SUB VObj_shutDown
  Sheet_destroy(_vobj_pageId%)
END SUB

'Allocate an view object from the heap and initialize it. 
'Returns an Object id. Note that ViewObject derives from Object.
FUNCTION VObj_create%()
  'First allocate a base Object
  LOCAL objId% = Obj_create%()
  'Set Type, solidness and altitude
  Obj_setType(objId%, OBJ_TYPE_VIEW_OBJECT%)
  Obj_setAlt(objId%, OBJ_MAX_ALTITUDE%)
  Obj_setPersist(objId%, 1) 'Make Object persistent so draw method is always called.
  
  'Set default methods
  obj_drawSub$(objId%) = "VObj_draw"
  obj_eventH$(objId%) = "VObj_eventHandler%"
  obj_destroySub$(objId%) = "VObj_destroy"
  'Now allocate a View Object
  LOCAL ii% = _vobj_allocIdx%
  
  DO
    IF _vobj_flags%(ii%) = 0 THEN 'Free entry found
      'Allocate, set default View Object attributes
      _vobj_flags%(ii%) = _VOBJ_FLG_DRAW_VAL_MSK% OR _VOBJ_FLG_BORDER_MSK% OR (RGB(WHITE)<<_VOBJ_FLG_COL_SHFT%)
      _vobj_flags2%(ii%) = VIEW_EVT_TAG_UNDEFINED%
      
      'Update index to next hopefully free entry
      _vobj_allocIdx% = ii% + 1
      IF _vobj_allocIdx% >= VOBJ_MAX_NUM% THEN
        _vobj_allocIdx%=0
      ENDIF
 
      INC vobj_numAllocated%  
      
      'Set ViewObject id as extra in base Obect
      Obj_setExtra(objId%, ii%)
      
      VObj_setLocation(objId%, VOBJ_LOC_TOP_CENTER%)

      'Register interest in view events.
      Obj_registerInterest(objId%, EVT_VIEW%)

      VObj_create% = objId%
      EXIT FUNCTION 
    ENDIF
    
    INC ii%
    IF ii% >= VOBJ_MAX_NUM% THEN
      ii%=0
    ENDIF
  LOOP UNTIL ii% = _vobj_allocIdx%
  
  ERROR "ViewObject Heap exhausted!"
END FUNCTION

'Destroy given view object, releasing all resources used by this object.
'Called through Obj_destroy virtual destructor.
SUB VObj_destroy(objId%)
  LOCAL vo_id% = Obj_getExtra%(objId%)

  LM_writeLog("Deleting view obj "+STR$(vo_id%))
  IF _vobj_flags%(vo_id%) = 0 THEN
    ERROR "Attempt to free a ViewObject that was not allocated. id: "+STR$(vo_id%))
  ENDIF
  
  _vobj_flags%(vo_id%) = 0
      
  INC vobj_numAllocated%, -1  
  
  Obj_defaultDestroy objId% 'Invoke base class destructor, non-virtual.
END SUB

'Virtual Object::draw method
SUB VObj_draw(objId%)
  LOCAL objBox%
  getBitField obj_flags%(objId%), OBJ_FLG_BOX_MSK%, OBJ_FLG_BOX_SHFT%, objBox%
  BLIT box_cornerX%(objBox%), box_cornerY%(objBox%), box_cornerX%(objBox%), box_cornerY%(objBox%), box_width%(objBox%), box_height%(objBox%), _vobj_pageId%, 4
END SUB

'Commit changes since creation or since previous commmit.
SUB VObj_commit(objId%)
  'Set page write to the ViewObject page
  DM_setWritePage _vobj_pageId%

  LOCAL vo_id% = Obj_getExtra%(objId%)
  LOCAL colr% = _vobj_flags%(vo_id%) >> _VOBJ_FLG_COL_SHFT%
  LOCAL world_pos_x% = obj_posX%(objId%)
  LOCAL world_pos_y% = obj_posY%(objId%)

  viewToWorld(world_pos_x%, world_pos_y%)

  LOCAL s$
  IF _vobj_flags%(vo_id%) AND _VOBJ_FLG_DRAW_VAL_MSK% THEN 'Display view string + value
    s$ = " "+_vobj_viewString$(vo_id%)+" "+STR$(_vobj_flags%(vo_id%) AND _VOBJ_FLG_VAL_MSK%)+" "
  ELSE
    s$ = " "+_vobj_viewString$(vo_id%)+" "
  ENDIF

  'Abuse the Object Box property to store the box's position and size
  LOCAL objBox%
  getBitField obj_flags%(objId%), OBJ_FLG_BOX_MSK%, OBJ_FLG_BOX_SHFT%, objBox%
  box_cornerX%(objBox%) = (obj_posX%(objId%)>>>4) - LEN(s$)*MM.INFO(FONTWIDTH)\2
  box_cornerY%(objBox%) = (obj_posY%(objId%)>>>4) - MM.INFO(FONTHEIGHT)
  box_width%(objBox%) = LEN(s$)*MM.INFO(FONTWIDTH)
  box_height%(objBox%) = MM.INFO(FONTHEIGHT)*2

  'Clear this portion of the page
  BOX box_cornerX%(objBox%), box_cornerY%(objBox%), box_width%(objBox%), box_height%(objBox%), 0, 0, 0
  
  DM_drawString(world_pos_x%, world_pos_y%, s$, colr%)

  'Draw border, if requested
  IF _vobj_flags%(vo_id%) AND _VOBJ_FLG_BORDER_MSK% THEN
    BOX box_cornerX%(objBox%), box_cornerY%(objBox%), box_width%(objBox%), box_height%(objBox%), 1, colr%
  ENDIF
    
  'Restore write page
  DM_restoreDefWritePage  
END SUB

'Set general location of ViewObject on screen
SUB VObj_setLocation(objId%, location%)
  LOCAL vo_id% = Obj_getExtra%(objId%)
  
  'Set position based on location
  SELECT CASE location%
    CASE VOBJ_LOC_TOP_LEFT%
      obj_posX%(objId%) = MM.HRES*16\6
      obj_posY%(objId%) = MM.INFO(FONTHEIGHT)*16
    CASE VOBJ_LOC_TOP_CENTER%
      obj_posX%(objId%) = MM.HRES*3*16\6
      obj_posY%(objId%) = MM.INFO(FONTHEIGHT)*16
    CASE VOBJ_LOC_TOP_RIGHT%
      obj_posX%(objId%) = MM.HRES*5*16\6
      obj_posY%(objId%) = MM.INFO(FONTHEIGHT)*16
    CASE VOBJ_LOC_CENTER_LEFT%
      obj_posX%(objId%) = MM.HRES*16\6
      obj_posY%(objId%) = MM.VRES*16\2
    CASE VOBJ_LOC_CENTER_CENTER%
      obj_posX%(objId%) = MM.HRES*3*16\6
      obj_posY%(objId%) = MM.VRES*16\2    
    CASE VOBJ_LOC_CENTER_RIGHT%
      obj_posX%(objId%) = MM.HRES*5*16\6
      obj_posY%(objId%) = MM.VRES*16\2        
    CASE VOBJ_LOC_BOTTOM_LEFT%
      obj_posX%(objId%) = MM.HRES*16\6
      obj_posY%(objId%) = (MM.VRES-MM.INFO(FONTHEIGHT))*16
    CASE VOBJ_LOC_BOTTOM_CENTER%
      obj_posX%(objId%) = MM.HRES*3*16\6
      obj_posY%(objId%) = (MM.VRES-MM.INFO(FONTHEIGHT))*16
    CASE VOBJ_LOC_BOTTOM_RIGHT%
      obj_posX%(objId%) = MM.HRES*5*16\6
      obj_posY%(objId%) = (MM.VRES-MM.INFO(FONTHEIGHT))*16
  END SELECT  
  
  _vobj_flags%(vo_id%) = (_vobj_flags%(vo_id%) AND (INV _VOBJ_FLG_LOC_MSK%)) OR (location% << _VOBJ_FLG_LOC_SHFT%)
END SUB

'Get general location of ViewObject on screen
FUNCTION VObj_getLocation%(objId%)
  LOCAL vo_id% = Obj_getExtra%(objId%)
  getBitField _vobj_flags%(vo_id%), _VOBJ_FLG_LOC_MSK%, _VOBJ_FLG_LOC_SHFT%, VObj_getLocation%
END FUNCTION

'Set view value
SUB VObj_setValue(objId%, value%)
  LOCAL vo_id% = Obj_getExtra%(objId%)
  setBitField(_vobj_flags%(vo_id%), _VOBJ_FLG_VAL_MSK%, _VOBJ_FLG_VAL_SHFT%, value%)
END SUB

'Get view value
FUNCTION VObj_getValue%(objId%)
  LOCAL vo_id% = Obj_getExtra%(objId%)
  getBitField _vobj_flags%(vo_id%), _VOBJ_FLG_VAL_MSK%, _VOBJ_FLG_VAL_SHFT%, VObj_getValue%
END FUNCTION

'Set view border
SUB VObj_setBorder(objId%, border%)
  LOCAL vo_id% = Obj_getExtra%(objId%)
  setBitField(_vobj_flags%(vo_id%), _VOBJ_FLG_BORDER_MSK%, _VOBJ_FLG_BORDER_SHFT%, border%)

  'Reset location to account for border setting
  VObj_setLocation(objId%, VObj_getLocation%(objId%))
END SUB

'Get view border
FUNCTION VObj_getBorder%(objId%, border%)
  LOCAL vo_id% = Obj_getExtra%(objId%)
  getBitField _vobj_flags%(vo_id%), _VOBJ_FLG_BORDER_MSK%, _VOBJ_FLG_BORDER_SHFT%, VObj_getBorder%
END FUNCTION

'Set view color
SUB VObj_setColor(objId%, colr%)
  LOCAL vo_id% = Obj_getExtra%(objId%)
  setBitField(_vobj_flags%(vo_id%), _VOBJ_FLG_COL_MSK%, _VOBJ_FLG_COL_SHFT%, colr%)
END SUB

'Get view color
FUNCTION VObj_getColor%(objId%, colr%)
  LOCAL vo_id% = Obj_getExtra%(objId%)
  getBitField _vobj_flags%(vo_id%), _VOBJ_FLG_COL_MSK%, _VOBJ_FLG_COL_SHFT%, VObj_getColor%
END FUNCTION

'Set view display string
SUB VObj_setViewString(objId%, ss$)
  LOCAL vo_id% = Obj_getExtra%(objId%)
  _vobj_viewString$(vo_id%) = ss$
END SUB

'Get view string
FUNCTION VObj_getViewString$(objId%)
  LOCAL vo_id% = Obj_getExtra%(objId%)
  VObj_getViewString$ = _vobj_viewString$(vo_id%)
END FUNCTION

'Set to draw value with display string
SUB VObj_setDrawValue(objId%, drawVal%)
  LOCAL vo_id% = Obj_getExtra%(objId%)
  setBitField(_vobj_flags%(vo_id%), _VOBJ_FLG_DRAW_VAL_MSK%, _VOBJ_FLG_DRAW_VAL_SHFT%, drawVal%)
END SUB

'Get draw value flag
FUNCTION VObj_getDrawValue%(objId%)
  LOCAL vo_id% = Obj_getExtra%(objId%)
  getBitField _vobj_flags%(vo_id%), _VOBJ_FLG_DRAW_VAL_MSK%, _VOBJ_FLG_DRAW_VAL_SHFT%, VObj_getDrawValue%
END FUNCTION

'Set the view event tag that this object responds to. See ViewEventTags.inc.
SUB VObj_setEventTag(objId%, tag%)
  LOCAL vo_id% = Obj_getExtra%(objId%)
  _vobj_flags2%(vo_id%) = tag%
END SUB

'Get the view event tag that this object responds to.
FUNCTION VObj_getEventTag%(objId%)
  LOCAL vo_id% = Obj_getExtra%(objId%)
  VObj_getEventTag% = _vobj_flags2%(vo_id%)
END FUNCTION

'Handle view event if tag matches the view string.
'Return 0 if ignored, 1 if handled.
FUNCTION VObj_eventHandler%(objId%, ev%)
  LOCAL vo_id% = Obj_getExtra%(objId%)

  'See if this is a view event
  IF EVT_getType%(ev%) = EVT_VIEW% THEN
    'See if this event is meant for this object.
    IF EVT_VIEW_getTag%(ev%) = _vobj_flags2%(vo_id%) THEN
      IF EVT_VIEW_getDelta%(ev%) THEN
        LOCAL new_val% = (_vobj_flags%(vo_id%) + EVT_VIEW_getValue%(ev%)) AND _VOBJ_FLG_VAL_MSK%
        _vobj_flags%(vo_id%) = (_vobj_flags%(vo_id%) AND (INV _VOBJ_FLG_VAL_MSK%)) OR new_val%
      ELSE
        _vobj_flags%(vo_id%) = (_vobj_flags%(vo_id%) AND (INV _VOBJ_FLG_VAL_MSK%)) OR (EVT_VIEW_getValue%(ev%)<<_VOBJ_FLG_VAL_SHFT%)
      ENDIF
      
      VObj_commit(objId%)
      
      'Event is handled
      VObj_eventHandler% = 1
    ENDIF
  ENDIF
  
  VObj_eventHandler% = 0
END FUNCTION

