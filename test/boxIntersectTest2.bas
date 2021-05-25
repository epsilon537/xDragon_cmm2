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

Sheet_load("../resources/spritesheet.BMP")

Anim_load("../resources/enemy.csv", "Enemy")
Anim_load("../resources/ship.csv", "Ship")

DIM obj1% = TestObj_create%(ASC("y"), ASC("*"), 0)
DIM obj2% = TestObj_create%(ASC("b"), ASC("X"), 1)

Obj_setAnim(obj1%, "Enemy")
Obj_setAnim(obj2%, "Ship")

Obj_setSolidness(obj1%, OBJ_SOFT%)
Obj_setSolidness(obj2%, OBJ_SOFT%)

DIM vx% = MM.HRES*0.25*16
DIM vy% = MM.VRES*0.25*16

obj_posX%(obj1%) = vx%
obj_posY%(obj1%) = vy%

obj_posX%(obj2%) = MM.HRES*0.75*16
obj_posY%(obj2%) = MM.VRES*0.75*16

GM_run

TestObj_destroy%(obj1%)
TestObj_destroy%(obj2%)

LM_writeLog("Test Complete.")

GM_shutdown

END

FUNCTION TestObj_create%(colr%, c%, focus%)
  LOCAL obj_id% = Obj_create%()
  LOCAL extra% = (focus%<<31) OR (c%<<24) OR (colr% AND &HFFFFFF)
  Obj_setExtra(obj_id%, extra%)
  Obj_setType(obj_id%, OBJ_TYPE_TEST%)
  
  Obj_registerInterest(obj_id%, EVT_STEP%)
  Obj_registerInterest(obj_id%, EVT_KBD%)
  Obj_registerInterest(obj_id%, EVT_COL%)
  
  obj_drawSub$(obj_id%) = "TestObj_draw"
  obj_eventH$(obj_id%) = "TestObj_eventHandler%"
  
  TestObj_create% = obj_id%
END FUNCTION

SUB TestObj_destroy(obj_id%)
  PRINT "TestObject destroyed: "+STR$(obj_id%)
END SUB

FUNCTION TestObj_eventHandler%(obj_id%, ev%)
  TestObj_eventHandler% = 0
  
  LOCAL eType% = EVT_getType%(ev%)
  
  IF eType% = EVT_STEP% THEN
    STATIC wBox% = Box_create%(0,0,0)
    
    LOCAL center_x% = MM.HRES*0.5*16
    LOCAL center_y% = MM.VRES*0.5*16
    
    TEXT MM.HRES*0.5, MM.VRES*0.5, "+", LT,,,RGB(white)
    
    getWorldBox(obj_id%, wBox%)
    
    IF boxContainsPosition%(wBox%, center_x%, center_y%) THEN
      LM_writeLog("!")
    ENDIF  
    
    STATIC wBox1% = Box_create%(0,0,0,0)
    STATIC wBox2% = Box_create%(0,0,0,0)

    getWorldBox(obj1%, wBox1%)
    getWorldBox(obj2%, wBox2%)

    IF boxContainsBox%(wBox1%, wBox2%) THEN
      LM_writeLog("?")
    ENDIF

    IF boxContainsBox%(wBox2%, wBox1%) THEN
      LM_writeLog(".")
    ENDIF    
  ENDIF

  IF eType% = EVT_COL% THEN
    LM_writeLog "X"
  ENDIF    
  
  IF eType% = EVT_KBD% THEN
    IF EVT_KBD_getAction%(ev%) = EVT_KBD_ACT_KEY_PRESSED% THEN
      LOCAL key% = EVT_KBD_getKey%(ev%) 
      LOCAL extra% = Obj_getExtra%(obj_id%)
      LOCAL focus% = extra%>>31
      LOCAL box_id% = Obj_getBox%(obj_id%)

      LM_writeLog("Key pressed: "+STR$(key%))
            
      IF key% = F_KEY% THEN
        focus% = NOT focus%
        extra% = (focus%<<31) OR (extra% AND &H7FFFFFFF)
        Obj_setExtra(obj_id%, extra%)
      ENDIF

      IF NOT focus% THEN
        EXIT FUNCTION
      ENDIF
            
      SELECT CASE key%
      CASE T_KEY%
        IF obj_id% = obj1% THEN
          Obj_setActive(obj2%, NOT Obj_isActive%(obj2%))
        ELSE
          Obj_setActive(obj1%, NOT Obj_isActive%(obj1%))        
        ENDIF
      CASE H_KEY%
        LM_writeLog("solid hard")
        Obj_setSolidness(obj_id%, OBJ_HARD%)
      CASE W_KEY%
        INC obj_velY%(obj_id%), -16
      CASE A_KEY%
        INC obj_velX%(obj_id%), -16
      CASE S_KEY%
        INC obj_velY%(obj_id%), 16
      CASE D_KEY%
        INC obj_velX%(obj_id%), 16
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
        INC box_height%(box_id%),-MM.INFO(FONTHEIGHT)*16
      CASE COMMA_KEY%
        INC box_height%(box_id%), MM.INFO(FONTHEIGHT)*16
      END SELECT
    ENDIF
  ENDIF
END FUNCTION

SUB TestObj_draw(obj_id%)
  STATIC worldBox% = Box_create%(0,0,0,0)

  Obj_defaultDraw(obj_id%)
  
  getWorldBox(obj_id%, worldBox%)

  LOCAL wBoxCorner_x% = box_cornerX%(worldBox%)
  LOCAL wBoxCorner_y% = box_cornerY%(worldBox%)
  
  LOCAL hor% = box_width%(worldBox%)
  LOCAL ver% = box_height%(worldBox%)
  LOCAL otherCorner_x% = wBoxCorner_x% + hor%
  LOCAL otherCorner_y% = wBoxCorner_y% + ver%

  LOCAL x%=0, y%=0
  LOCAL extra% = Obj_getExtra%(obj_id%)
  LOCAL c$ = CHR$((extra%>>24) AND 127)
  LOCAL colr% = extra% AND &HFFFFFF
  
  IF colr% = ASC("y") THEN
    colr% = RGB(yellow)
  ELSE
    colr% = RGB(blue)
  ENDIF

  DO WHILE x% < hor%
    y%=0
    DO WHILE y% < ver%
      DM_drawCh(x%+wBoxCorner_x%, y%+wBoxCorner_y%, c$, colr%)

      INC y%, MM.INFO(FONTHEIGHT)*16
    LOOP
    INC x%, MM.INFO(FONTWIDTH)*16
  LOOP

  DM_drawCh(wBoxCorner_x%, wBoxCorner_y%, "b", RGB(green))
  DM_drawCh(otherCorner_x%, otherCorner_y%, "B", RGB(green))
END SUB

