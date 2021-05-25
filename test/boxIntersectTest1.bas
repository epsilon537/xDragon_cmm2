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

DIM obj1% = TestObj_create%(ASC("y"), ASC("*"), 0)
DIM obj2% = TestObj_create%(ASC("b"), ASC("X"), 1)

GM_run

TestObj_destroy%(obj1%)
TestObj_destroy%(obj2%)

LM_writeLog("Test Complete.")

GM_shutdown

END

FUNCTION TestObj_create%(colr%, c%, focus%)
  LOCAL obj_id% = Obj_create%()
  LOCAL extra% = (focus%<<31) OR (c%<<24) OR (colr% AND &HFFFFFF)
  LM_writeLog("focus "+HEX$(focus%))
  LM_writeLog("c "+HEX$(c%))
  LM_writeLog("colr "+HEX$(colr%))
  LM_writeLog("extra "+HEX$(extra%))
  Obj_setExtra(obj_id%, extra%)
  Obj_setType(obj_id%, OBJ_TYPE_TEST%)
  
  Obj_registerInterest(obj_id%, EVT_STEP%)
  Obj_registerInterest(obj_id%, EVT_KBD%)
  obj_drawSub$(obj_id%) = "TestObj_draw"
  obj_eventH$(obj_id%) = "TestObj_eventHandler%"  
  obj_destroySub$(obj_id%) = "TestObj_destroy"  
  TestObj_create% = obj_id%
END FUNCTION

SUB TestObj_destroy(obj_id%)
  PRINT "TestObject destroyed: "+STR$(obj_id%)
  Obj_defaultDestroy obj_id%
END SUB

FUNCTION TestObj_eventHandler%(obj_id%, ev%)
  TestObj_eventHandler% = 0
  
  LOCAL eType% = EVT_getType%(ev%)
  LOCAL res%
  
  IF eType% = EVT_STEP% THEN
    boxIntersectBox(Obj_getBox%(obj1%), Obj_getBox%(obj2%), res%)
    IF res% THEN
      LM_writeLog("X")
    ELSE
      LM_writeLog("O")
    ENDIF
  ENDIF
  
  IF eType% = EVT_KBD% THEN
    IF EVT_KBD_getAction%(ev%) = EVT_KBD_ACT_KEY_PRESSED% THEN
      LOCAL key% = EVT_KBD_getKey%(ev%) 
      LOCAL extra% = Obj_getExtra%(obj_id%)
      LOCAL focus% = extra%>>31
      LOCAL box_id% = Obj_getBox%(obj_id%)
      
      IF key% = F_KEY% THEN
        focus% = NOT focus%
        extra% = (focus%<<31) OR (extra% AND &H7FFFFFFF)
        Obj_setExtra(obj_id%, extra%)
      ENDIF

      IF NOT focus% THEN
        EXIT FUNCTION
      ENDIF
            
      SELECT CASE key%
      CASE LEFT_KEY%
        INC box_cornerX%(box_id%), -MM.INFO(FONTWIDTH)*16
      CASE RIGHT_KEY%
        INC box_cornerX%(box_id%), MM.INFO(FONTWIDTH)*16
      CASE UP_KEY%
        INC box_cornerY%(box_id%), -MM.INFO(FONTHEIGHT)*16
      CASE DOWN_KEY%
        INC box_cornerY%(box_id%), MM.INFO(FONTHEIGHT)*16
      CASE PLUS_KEY%
        INC box_width%(box_id%), MM.INFO(FONTWIDTH)*16
      CASE MINUS_KEY%
        INC box_width%(box_id%), -MM.INFO(FONTWIDTH)*16
      CASE SLASH_KEY%
        INC box_height%(box_id%), -MM.INFO(FONTHEIGHT)*16
      CASE COMMA_KEY%
        INC box_height%(box_id%), MM.INFO(FONTHEIGHT)*16
      END SELECT
    ENDIF
  ENDIF
END FUNCTION

SUB TestObj_draw(obj_id%)
  LOCAL x%=0, y%=0
  LOCAL box_id% = Obj_getBox%(obj_id%)
  LOCAL corner_x% = box_cornerX%(box_id%)
  LOCAL corner_y% = box_cornerY%(box_id%)  
  LOCAL extra% = Obj_getExtra%(obj_id%)
  LOCAL c$ = CHR$((extra%>>24) AND 127)
  LOCAL colr% = extra% AND &HFFFFFF
  
  IF colr% = ASC("y") THEN
    colr% = RGB(yellow)
  ELSE
    colr% = RGB(blue)
  ENDIF

  DO WHILE x% < box_width%(box_id%)
    y%=0
    DO WHILE y% < box_height%(box_id%)
      DM_drawCh(x%+corner_x%, y%+corner_y%, c$, colr%)

      INC y%, MM.INFO(FONTHEIGHT)*16
    LOOP
    INC x%, MM.INFO(FONTWIDTH)*16
  LOOP
END SUB

