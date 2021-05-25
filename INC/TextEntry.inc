'Text Entry is a singleton ViewObject extension.

'Public constants
'----------------
CONST TEXT_ENTRY_DEFAULT_BLINK_RATE% = 15 'Cursor default blink rate in TextEntry object.

'Read/Write variables
'--------------------
DIM textEntry_numbersOnly% = 0 'True if only numbers are accepted.
DIM textEntry_cursorChar$ = "_" 'Cursor character
DIM textEntry_blinkRate% = TEXT_ENTRY_DEFAULT_BLINK_RATE% 'Cursor blink rate
DIM textEntry_callback$ 'Set to SUB to CALL back when enter is pressed.

'Private variables and constants
'-------------------------------

DIM _textEntry_text$ 'Text entered
DIM _textEntry_prefix$ 'Prefix going before text entry section
DIM _textEntry_limit% = 0 'Character limit in text
DIM _textEntry_cursor% = 0 'Cursor location
DIM _textEntry_objId% = -1

'Public subs and functions
'-------------------------

'Create a TextEntry Object. Call this before using any other TextEntry functions.
'Defaults: prefix="", limit=0, numbers_only=false, cursor_char=_, blink_rate=default
'Returns ViewObject id.
FUNCTION TextEntry_create%()
  IF _textEntry_objId% <> -1 THEN
    ERROR "A TextEntry object already exists."
  ENDIF
  
  _textEntry_objId% = VObj_create%()
  Obj_setType(_textEntry_objId%, OBJ_TYPE_TEXT_ENTRY%)
  
  _textEntry_text$ = ""
  _textEntry_prefix$ = ""
  _textEntry_limit% = 0
  textEntry_numbersOnly% = 0
  _textEntry_cursor% = 0
  textEntry_cursorChar$ = "_"
  textEntry_blinkRate% = TEXT_ENTRY_DEFAULT_BLINK_RATE%
  textEntry_callback$=""
  
  Obj_registerInterest(_textEntry_objId%, EVT_STEP%)
  Obj_registerInterest(_textEntry_objId%, EVT_KBD%)
  obj_eventH$(_textEntry_objId%) = "TextEntry_eventHandler%"  
  obj_destroySub$(_textEntry_objId%) = "TextEntry_destroy"
  TextEntry_create% = _textEntry_objId%
END FUNCTION

'Destroy TextEntry Object
SUB TextEntry_destroy(objId%)
  VObj_destroy objId%
  _textEntry_objId% = -1
END SUB

'Set text entry prefix
SUB TextEntry_setPrefix(objId%, prefix$)
  _textEntry_prefix$ = prefix$
  PRINT "prefx = "+_textEntry_prefix$+" LEN: "+STR$(LEN(_textEntry_prefix$))
  PRINT "txt = "+_textEntry_text$+" LEN: "+STR$(LEN(_textEntry_text$))

  VObj_setViewString(objId%, _textEntry_prefix$+_textEntry_text$)
END SUB

'Set text entered
SUB TextEntry_setText(objId%, txt$)
  _textEntry_text$ = LEFT$(txt$, _textEntry_limit%)
  VObj_setViewString(objId%, _textEntry_prefix$+_textEntry_text$)
END SUB

'TextEntry object eventHandler, used to handle keyboard input.
FUNCTION TextEntry_eventHandler%(objId%, ev%)    
  'If step event, blink cursor
  IF EVT_getType%(ev%) = EVT_STEP% THEN
    'Blink on or off based on rate.
    STATIC blink%=-1*textEntry_blinkRate%

    IF blink% >= 0 THEN
      MID$(_textEntry_text$, _textEntry_cursor%+1, 1) = textEntry_cursorChar$
    ELSE
      MID$(_textEntry_text$, _textEntry_cursor%+1, 1) = " "
    ENDIF

    INC blink%
    
    IF blink% = textEntry_blinkRate% THEN
      blink% = -1*textEntry_blinkRate%
    ENDIF

    VObj_setViewString(objId%, _textEntry_prefix$+_textEntry_text$)
    TextEntry_eventHandler% = 1
    EXIT FUNCTION
  ENDIF
  
  'If keyboard event, handle
  IF EVT_getType%(ev%) = EVT_KBD% THEN
    LOCAL action% = EVT_KBD_getAction%(ev%)

    IF action% = EVT_KBD_ACT_KEY_PRESSED% THEN
      LOCAL key% = EVT_KBD_getKey%(ev%)

      'If return key pressed, then callback  
      IF key% = ENTER_KEY% THEN
        IF textEntry_callback$ <> "" THEN
          LM_writeLog("TextEntry invoking callback.")
          CALL(textEntry_callback$)
        ENDIF
        
        TextEntry_eventHandler% = 1
        EXIT FUNCTION
      ENDIF
 
      IF key% = BCKSPC_KEY% THEN
        IF _textEntry_cursor% > 0 THEN
          MID$(_textEntry_text$, _textEntry_cursor%+1, 1) = " "
          INC _textEntry_cursor%, -1
          MID$(_textEntry_text$, _textEntry_cursor%+1, 1) = " "
          VObj_setViewString(objId%, _textEntry_prefix$+_textEntry_text$)
          VObj_commit(objId%)        
        ENDIF
        
        TextEntry_eventHandler% = 1
        EXIT FUNCTION
      ENDIF
      
      'If no room, cannot add characters
      IF _textEntry_cursor% >= _textEntry_limit%-1 THEN
        TextEntry_eventHandler% = 1
        EXIT FUNCTION
      ENDIF
      
      'IF entry should be number, confirm
      IF textEntry_numbersOnly% AND NOT ((key%>= NUM0_KEY%) AND (key% <= NUM9_KEY%)) THEN
        TextEntry_eventHandler% = 1
        EXIT FUNCTION
      ENDIF

      'Replace with printable character
      IF (key% >= SPC_KEY%) AND (key% <= Z_KEY%) THEN
        MID$(_textEntry_text$, _textEntry_cursor%+1, 1) = CHR$(key%)
        VObj_setViewString(objId%, _textEntry_prefix$+_textEntry_text$)              
        VObj_commit(objId%)        
        INC _textEntry_cursor%
      ENDIF
      
      TextEntry_eventHandler% = 1
      
      EXIT FUNCTION
    ENDIF      
  ENDIF

  TextEntry_eventHandler% = 0
  EXIT FUNCTION  
END FUNCTION

'Set limit of number of characters allowed.
SUB TextEntry_setLimit(objId%, limit%)
  _textEntry_cursor% = 0
  _textEntry_limit% = limit%
  _textEntry_text$ = ""

  LOCAL ii%
  FOR ii%=0 TO limit%-1
    _textEntry_text$ = _textEntry_text$ + " "
  NEXT ii%

  VObj_setViewString(objId%, _textEntry_prefix$+_textEntry_text$)                
END SUB

'Commit changes since creation or since previous commmit.
SUB TextEntry_commit(objId%)
  VObj_commit(objId%)
END SUB

