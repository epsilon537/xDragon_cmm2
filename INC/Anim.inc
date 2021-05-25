'Public constants
'----------------

#define "ANIM_MAX_NUM%","1000" 'Max. num. anim objects that can exist in the game.
#define "ANIM_MAX_NUM_FRAMES%","16" 'Max. number of frames that an animation can hold.

'Private variables and constants
'-------------------------------

'Anim flags decoding masks and shifts
#define "_ANIM_FLG_ALLOCATED_MSK%","(1<<61)"   'Anim Object is allocated flag
#define "_ANIM_FLG_ALLOCATED_SHFT%","61"
#define "_ANIM_FLG_WIDTH_MSK%","(65535<<21)"   'Anim width
#define "_ANIM_FLG_WIDTH_SHFT%","21"
#define "_ANIM_FLG_HEIGHT_MSK%","(65535<<5)"   'Anim height
#define "_ANIM_FLG_HEIGHT_SHFT%","5"
#define "_ANIM_FLG_FRAMES_MSK%","31"   'Number of frames
#define "_ANIM_FLG_FRAMES_SHFT%","0"

CONST _ANIM_CSV_HEADER$ = DQ$+"Delay(1/60)"+DQ$+","+DQ$+"File Name"+DQ$+","+DQ$+"X"+DQ$+","+DQ$+"Y"+DQ$+","+DQ$+"Width"+DQ$+","+DQ$+"Height"+DQ$

#define "_ANIM_SLOWDOWN_FIELD%","1"
#define "_ANIM_SHEETNAME_FIELD%","2"
#define "_ANIM_X_OFFSET_FIELD%","3"
#define "_ANIM_Y_OFFSET_FIELD%","4"
#define "_ANIM_WIDTH_FIELD%","5"
#define "_ANIM_HEIGHT_FIELD%","6"

'Various collision state values packed into an int. See mask and shift values above.
DIM _anim_flags%(ANIM_MAX_NUM%-1) 
DIM _anim_label$(ANIM_MAX_NUM%-1) LENGTH 16 'Text label to identify anim.
DIM _anim_frames%(ANIM_MAX_NUM_FRAMES%-1, ANIM_MAX_NUM%-1) 'Array of frames

'Index into tables above used to find the next available (unallocated) object 
DIM _anim_allocIdx%=0

DIM _anim_strings$(ANIM_MAX_NUM_FRAMES%)

'Read-Only variables
'-------------------

DIM anim_numAllocated%=0 'Used to track resource leaks
DIM anim_index%(ANIM_MAX_NUM%-1) 'Current index into the animation sequence.

'Read-Write variables
'--------------------

DIM anim_slowdownCount%(ANIM_MAX_NUM%-1) 'Slowdown count value, -1 means stop.
DIM anim_slowdownLimit%(ANIM_MAX_NUM%-1) 'Slowdown limit value
DIM anim_loopRewindIdx%(ANIM_MAX_NUM%-1) 'Frame index to which loop rewinds. Default: 0

'Public subs and functions
'-------------------------

'Initialize Anim module. Should be called once, at start-up of the game engine.
SUB Anim_startUp
  'Just for symmetry. Nothing to do currently.
END SUB

'Allocate a anim object from the heap and initialize it with default params.
FUNCTION Anim_create%()
  LOCAL ii% = _anim_allocIdx%
  
  DO
    IF _anim_flags%(ii%) = 0 THEN 'Free entry found
      'Allocate
      _anim_flags%(ii%) = _ANIM_FLG_ALLOCATED_MSK%
      'label
      _anim_label$(ii%) = ""
      
      'Update index to next hopefully free entry
      _anim_allocIdx% = ii% + 1
      IF _anim_allocIdx% >= ANIM_MAX_NUM% THEN
        _anim_allocIdx%=0
      ENDIF

      anim_index%(ii%)=0
      anim_slowdownCount%(ii%)=0
      anim_slowdownLimit%(ii%)=0
      anim_loopRewindIdx%(ii%)=0
      
      INC anim_numAllocated%  
      Anim_create% = ii%
      EXIT FUNCTION 
    ENDIF
    
    INC ii%
    IF ii% >= ANIM_MAX_NUM% THEN
      ii%=0
    ENDIF
  LOOP UNTIL ii% = _anim_allocIdx%
  
  ERROR "Anim Object Heap exhausted!"
END FUNCTION

'Destroy given anim object, releasing all resources used by this object.
SUB Anim_destroy(aid%)
  IF _anim_flags%(aid%) = 0 THEN
    ERROR "Attempt to free an Anim Object that was not allocated."
  ENDIF

  LOCAL ii%
  
  FOR ii%=0 TO Anim_getNumFrames%(aid%)-1
    'Unreference associated frame objects
    Frame_unref(Anim_getFrame%(aid%, ii%))
  NEXT ii%
  
  _anim_flags%(aid%) = 0
  
  INC anim_numAllocated%, -1  
END SUB

'Returns non-zero if anim object is allocated.
FUNCTION Anim_isAllocated%(aid%)
  Anim_isAllocated% = _anim_flags%(aid%)
END FUNCTION

'Set width of anim. .4 subpixel units.
SUB Anim_setWidth(aid%, width%)
  setBitField _anim_flags%(aid%), _ANIM_FLG_WIDTH_MSK%, _ANIM_FLG_WIDTH_SHFT%, width%
END SUB

'Get width of anim. .4 subpixel units.
FUNCTION Anim_getWidth%(aid%)
  getBitField _anim_flags%(aid%), _ANIM_FLG_WIDTH_MSK%, _ANIM_FLG_WIDTH_SHFT%, Anim_getWidth%
END FUNCTION

'Set height of anim. .4 subpixel units.
SUB Anim_setHeight(aid%, height%)
  setBitField _anim_flags%(aid%), _ANIM_FLG_HEIGHT_MSK%, _ANIM_FLG_HEIGHT_SHFT%, height%
END SUB

'Get height of anim. .4 subpixel units.
FUNCTION Anim_getHeight%(aid%)
  getBitField _anim_flags%(aid%), _ANIM_FLG_HEIGHT_MSK%, _ANIM_FLG_HEIGHT_SHFT%, Anim_getHeight%
END FUNCTION

'Get number of frames in anim.
FUNCTION Anim_getNumFrames%(aid%)
  getBitField _anim_flags%(aid%), _ANIM_FLG_FRAMES_MSK%, _ANIM_FLG_FRAMES_SHFT%, Anim_getNumFrames%
END FUNCTION

'Add a frame to anim. object.
SUB Anim_addFrame(aid%, frid%)
  LOCAL frameCount% = Anim_getNumFrames%(aid%)
  
  IF frameCount% >= ANIM_MAX_NUM_FRAMES% THEN
    Error "Can't add frame. Anim full."
  ENDIF
  
  _anim_frames%(frameCount%, aid%) = frid%
  Frame_ref(frid%)
  
  INC frameCount%
  setBitField _anim_flags%(aid%), _ANIM_FLG_FRAMES_MSK%, _ANIM_FLG_FRAMES_SHFT%, frameCount%
END SUB

'Get frame indicated by number
FUNCTION Anim_getFrame%(aid%, frameNumber%)
  Anim_getFrame% = _anim_frames%(frameNumber%, aid%)
END FUNCTION

'Set label associated with anim object.
SUB Anim_setLabel(aid%, label$)
  _anim_label$(aid%) = label$
END SUB

'Get label associated with anim object.
FUNCTION Anim_getLabel$(aid%)
  Anim_getLabel$ = _anim_label$(aid%) 
END FUNCTION

'Draw a specific frame centered at given position (.4 subpixel units).
SUB Anim_drawFrame(aid%, index%, xx%, yy%)  
  Frame_draw _anim_frames%(index%, aid%), xx%, yy%
END SUB

'Draw frame centered at given position (.4 subpixel units).
'Account for slowdown and advance anim frame.
SUB Anim_draw(aid%, xx%, yy%)  
  Frame_draw _anim_frames%(anim_index%(aid%), aid%), xx%, yy%
  
  IF anim_slowdownCount%(aid%) = -1 THEN: EXIT SUB: ENDIF 
  INC anim_slowdownCount%(aid%)

  IF anim_slowdownCount%(aid%) >= anim_slowdownLimit%(aid%) THEN
    anim_slowdownCount%(aid%) = 0
    INC anim_index%(aid%)
    'If at last frame, loop to rewind index
    IF anim_index%(aid%) >= Anim_getNumFrames%(aid%) THEN: anim_index%(aid%) = anim_loopRewindIdx%(aid%): ENDIF
  ENDIF
END SUB

'Find anim object by label. Returns anim id, -1 if not found.
FUNCTION Anim_find%(label$)
  LOCAL ii%

  FOR ii%=0 TO ANIM_MAX_NUM%-1
    IF (_anim_flags%(ii%)<>0) AND (UCASE$(_anim_label$(ii%)) = UCASE$(label$)) THEN
      Anim_find% = ii%
      EXIT FUNCTION
    ENDIF
  NEXT ii%
  
  Anim_find% = -1
END FUNCTION

'Shut down ANIM module. Done once at exit of the game engine.
SUB Anim_shutDown
  LOCAL aid%
  
  FOR aid%=0 TO ANIM_MAX_NUM%-1
    IF Anim_isAllocated%(aid%) THEN
      Anim_destroy(aid%)
    ENDIF
  NEXT aid%
END SUB

'Load anim object description from .csv file
'Assign indicated label to anim.
SUB Anim_load(filename$, label$)
  IF Anim_find%(label$) <> -1 THEN
    ERROR "Anim label already in use: "+label$
  ENDIF
  
  LM_writeLog("Loading anim file: "+filename$)
  
  OPEN filename$ FOR INPUT AS #1
  
  'Read and first line which should be a header line
  LINE INPUT #1, _anim_strings$(0)
  IF _anim_strings$(0) <> _ANIM_CSV_HEADER$ THEN
    ERROR "Invalid Anim .CSV file."
  ENDIF
  
  'Now read contents, one row per frame.
  LOCAL frames%=0
  DO WHILE NOT EOF(#1)
    LINE INPUT #1, _anim_strings$(frames%)
    INC frames%
  LOOP
  
  'Get slowdown, frame height and width out of the first line
  'Convert to engine standard .4 subpixel units.
  LOCAL w% = _Anim_csv_getWidth%(_anim_strings$(0))<<4
  LOCAL h% = _Anim_csv_getHeight%(_anim_strings$(0))<<4
  LOCAL slowdown% = _anim_csv_getSlowdown%(_anim_strings$(0))
  
  'LM_writeLog("Frame width: "+STR$(w%))
  'LM_writeLog("Frame height: "+STR$(h%))
  'LM_writeLog("Frame slowdown: "+STR$(slowdown%))

  LOCAL aid% = Anim_create%()
  LOCAL ii%, frid%, x%, y%, page_id%
  
  FOR ii%=0 TO frames%-1
    'Get x-offset, y-offset and page-id per frame.
    x% = _Anim_csv_getXoffset%(_anim_strings$(ii%))<<4
    y% = _Anim_csv_getYoffset%(_anim_strings$(ii%))<<4
    page_id% = Sheet_find%(_Anim_csv_getSheetName$(_anim_strings$(ii%)))

    'LM_writeLog("Frame "+STR$(ii%)+" x: "+STR$(x%))
    'LM_writeLog("Frame "+STR$(ii%)+" y: "+STR$(y%))
    'LM_writeLog("Frame "+STR$(ii%)+" sheet: "+_Anim_csv_getSheetName$(_anim_strings$(ii%)))

    IF page_id% = -1 THEN
      ERROR "Sheet not found: "+_Anim_csv_getSheetName$(_anim_strings$(ii%))
    ENDIF
    
    frid% = Frame_create%(x%, y%, w%, h%, page_id%)
    
    Anim_addFrame(aid%, frid%)
  NEXT ii%
  
  Anim_setHeight(aid%, h%)
  Anim_setWidth(aid%, w%)
  anim_slowdownLimit%(aid%) = slowdown%
  Anim_setLabel(aid%, label$)

  CLOSE #1
END SUB

'Unload animation object with indicated label.
SUB Anim_unload(label$)
  LOCAL aid% = Anim_find%(label$)
  
  IF aid% = -1 THEN
    Error "Anim label "+label$+" not found."
  ENDIF
  
  Anim_destroy aid%
END SUB

'Copy all properties except the frames/numFrames from one anim obj to another
SUB Anim_copyProps(fromAid%, toAid%)
  Anim_setWidth toAid%, Anim_getWidth%(fromAid%)
  Anim_setHeight toAid%, Anim_getHeight%(fromAid%)
  anim_slowdownCount%(toAid%) = anim_slowdownCount%(fromAid%)
  anim_slowdownLimit%(toAid%) = anim_slowdownLimit%(fromAid%)  
END SUB

'Reset animation object counters so animation restarts from the beginning
SUB Anim_reset(aid%)
  anim_index%(aid%) = 0
  anim_slowdownCount%(aid%) = 0
END SUB

'Private subs and functions
'-------------------------

'Lift field out of a anim csv line
FUNCTION _Anim_csv_getFieldStr$(s$, fieldId%)
  LOCAL fld$ = FIELD$(s$, fieldId%, ",")
  'Skip leading and trailing quotes
  _Anim_csv_getFieldStr$=MID$(fld$, 2, LEN(fld$)-2)
END FUNCTION

'Lift slowdown value out of a anim csv line
FUNCTION _Anim_csv_getSlowDown%(s$)
  _Anim_csv_getSlowDown% = VAL(_Anim_csv_getFieldStr$(s$, _ANIM_SLOWDOWN_FIELD%))
END FUNCTION

'Lift sheet name out of a anim csv line
FUNCTION _Anim_csv_getSheetName$(s$)
  _Anim_csv_getSheetName$ = fileToBasename$(_Anim_csv_getFieldStr$(s$, _ANIM_SHEETNAME_FIELD%))
END FUNCTION

'Lift x-offset value out of a anim csv line
FUNCTION _Anim_csv_getXoffset%(s$)
  _Anim_csv_getXoffset% = VAL(_Anim_csv_getFieldStr$(s$, _ANIM_X_OFFSET_FIELD%))
END FUNCTION

'Lift y-offset value out of a anim csv line
FUNCTION _Anim_csv_getYoffset%(s$)
  _Anim_csv_getYoffset% = VAL(_Anim_csv_getFieldStr$(s$, _ANIM_Y_OFFSET_FIELD%))
END FUNCTION

'Lift width value out of a anim csv line
FUNCTION _Anim_csv_getWidth%(s$)
  _Anim_csv_getWidth% = VAL(_Anim_csv_getFieldStr$(s$, _ANIM_WIDTH_FIELD%))
END FUNCTION

'Lift width value out of a anim csv line
FUNCTION _Anim_csv_getHeight%(s$)
  _Anim_csv_getHeight% = VAL(_Anim_csv_getFieldStr$(s$, _ANIM_HEIGHT_FIELD%))
END FUNCTION

