OPTION EXPLICIT
OPTION DEFAULT NONE
OPTION BASE 0
OPTION CONSOLE SERIAL

#INCLUDE "../INC/Dummy.inc"
#INCLUDE "../INC/Utility.inc"
#INCLUDE "../INC/Palette.inc"
#INCLUDE "../INC/Sheet.inc"
#INCLUDE "../INC/Box.inc"
#INCLUDE "../INC/Background.inc"
#INCLUDE "../INC/TileEngine.inc"
#INCLUDE "../INC/Frame.inc"
#INCLUDE "../INC/Anim.inc"
#INCLUDE "../INC/EventTypes.inc"
#INCLUDE "../INC/Event.inc"
#INCLUDE "../INC/EventTEtrigger.inc"
#INCLUDE "../INC/EventStep.inc"
#INCLUDE "../INC/ObjectTypes.inc"
#INCLUDE "../INC/ViewEventTags.inc"
#INCLUDE "../INC/EventView.inc"
#INCLUDE "../INC/ViewObject.inc"
#INCLUDE "../INC/TextEntry.inc"
#INCLUDE "../INC/Object.inc"
#INCLUDE "../INC/ObjectList.inc"
#INCLUDE "../INC/EventCollision.inc"
#INCLUDE "../INC/EventManager.inc"
#INCLUDE "../INC/LogManager.inc"
#INCLUDE "../INC/DisplayManager.inc"
#INCLUDE "../INC/EventOut.inc"
#INCLUDE "../INC/SceneGraph.inc"
#INCLUDE "../INC/WorldManager.inc"
#INCLUDE "../INC/EventKeyboard.inc"
#INCLUDE "../INC/EventJoystick.inc"
#INCLUDE "../INC/InputManager.inc"
#INCLUDE "../INC/Sound.inc"
#INCLUDE "../INC/GameManager.inc"
#INCLUDE "../CSUBS/cFuncRAM.inc"

GM_startUp

DIM obj_id% = textEntry_create%()

IF _textEntry_text$ <> "" THEN
  ERROR "textEntry create error."
ENDIF

IF _textEntry_prefix$ <> "" THEN
  ERROR "textEntry create error."
ENDIF

IF _textEntry_limit% <> 0 THEN
  ERROR "textEntry create error."
ENDIF

IF _textEntry_cursor% <> 0 THEN
  ERROR "textEntry create error."
ENDIF

IF textEntry_numbersOnly% <> 0 THEN
  ERROR "textEntry create error."
ENDIF

IF textEntry_cursorChar$ <> "_" THEN
  ERROR "textEntry create error."
ENDIF

IF textEntry_blinkRate% <> TEXT_ENTRY_DEFAULT_BLINK_RATE% THEN
  ERROR "textEntry create error."
ENDIF

IF textEntry_callback$ <> "" THEN
  ERROR "textEntry create error."
ENDIF

IF _textEntry_objId% = -1 THEN
  ERROR "textEntry create error"
ENDIF

TextEntry_destroy(obj_id%)
IF _textEntry_objId% <> -1 THEN
  ERROR "textEntry destroy error"
ENDIF

obj_id% = textEntry_create%()
VObj_setLocation(obj_id%, VOBJ_LOC_CENTER_CENTER%)
VObj_setBorder(obj_id%, 1)
VObj_setColor(obj_id%, RGB(yellow))
VObj_setDrawValue(obj_id%, 0)

TextEntry_setLimit(obj_id%, 16)
textEntry_callback$ = "enterPressed"

TextEntry_commit(obj_id%)

GM_run
GM_shutDown

PRINT "Test Done."
END

SUB enterPressed
  LM_writeLog("Enter Pressed")
  
  IF LEN(_textEntry_text$) <> 16 THEN
    ERROR "textEntry error."
  ENDIF
  
  IF _textEntry_prefix$ <> "" THEN
    ERROR "textEntry error."
  ENDIF
  
  IF _textEntry_limit% <> 16 THEN
    ERROR "textEntry error."
  ENDIF
  
  IF textEntry_numbersOnly% <> 0 THEN
    ERROR "textEntry error."
  ENDIF
  
  IF textEntry_cursorChar$ <> "_" THEN
    ERROR "textEntry error."
  ENDIF
  
  IF textEntry_blinkRate% <> TEXT_ENTRY_DEFAULT_BLINK_RATE% THEN
    ERROR "textEntry error."
  ENDIF

  TextEntry_setPrefix(obj_id%, "Prefix: ")
  textEntry_numbersOnly% = 1
  textEntry_blinkRate% = 5
  TextEntry_setLimit(obj_id%, 4)
  textEntry_cursorChar$ = "*"
  
  textEntry_callback$ = ""
  
  TextEntry_commit(obj_id%)
END SUB

