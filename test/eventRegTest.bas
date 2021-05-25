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

PAL_clr
PAL_load("../resources/xDragonDemo.pal")
PAL_allocStdColors
PAL_commitToCLUT

Sheet_load("../resources/spritesheet.BMP")

Anim_load("../resources/enemy.csv", "Enemy")
Anim_load("../resources/ship.csv", "Ship")

DIM ship% = TestObj_create%()
DIM enemy% = TestObj_create%()

DIM shipBox% = Obj_getBox%(ship%)

IF box_cornerX%(shipBox%) <> 0 THEN
  ERROR "Box_getCorner error."
ENDIF
IF box_cornerY%(shipBox%) <> 0 THEN
  ERROR "Box_getCorner error."
ENDIF
IF box_width%(shipBox%) <> 16 THEN
  ERROR "Box_getHorizontal error."
ENDIF
IF box_height%(shipBox%) <> 16 THEN
  ERROR "Box_getVertical error."
ENDIF

DIM v%

Obj_setAnim(ship%, "Ship")
obj_posX%(ship%) = MM.HRES*16/3
obj_posY%(ship%) = MM.VRES*16/2
obj_velX%(ship%) = 16
obj_velY%(ship%) = 8

Obj_setAnim(enemy%, "Enemy")
obj_posX%(enemy%) = MM.HRES*2*16/3
obj_posY%(enemy%) = MM.HRES*16/2
obj_velX%(enemy%) = -8
obj_velY%(enemy%) = 16

Obj_registerInterest(ship%, EVT_KBD%)
Obj_registerInterest(ship%, EVT_STEP%)
Obj_registerInterest(ship%, EVT_JOY%)

GM_run

RM_unloadAnim("Saucer")
RM_unloadAnim("Ship")

LM_writeLog("Test Complete.")

GM_shutdown

END

FUNCTION TestObj_create%()
  LOCAL obj_id% = Obj_create%()
  Obj_setType(obj_id%, OBJ_TYPE_TEST%)
  obj_drawSub$(obj_id%) = "TestObj_draw"
  obj_eventH$(obj_id%) = "TestObj_eventHandler%"  
  obj_destroySub$(obj_id%) = "TestObj_destroy"  
  TestObj_create% = obj_id%
END FUNCTION

SUB TestObj_destroy(obj_id%)
  PRINT "TestObject destroyed: "+STR$(obj_id%)
  Obj_defaultDestroy obj_id%
END SUB

SUB TestObj_draw(obj_id%)
  IF Anim_getLabel$(obj_anim%(obj_id%)) = "Enemy" THEN
    LM_writeLog("Enemy drawn "+STR$(gm_frameCount%))
  ENDIF
  
  Obj_defaultDraw(obj_id%)
END SUB

FUNCTION TestObj_eventHandler%(obj_id%, ev%)
  SELECT CASE EVT_getType%(ev%)
    CASE EVT_OUT%
      LM_writeLog("Event Out for obj "+STR$(obj_id%))
      obj_velX%(obj_id%) = -obj_velX%(obj_id%)
      obj_velY%(obj_id%) = -obj_velY%(obj_id%)

      'TestObj_eventHandler% = 1
      'EXIT FUNCTION
    CASE EVT_STEP%
      LM_writeLog("OBJ_id "+STR$(obj_id%)+" Event Step count ="+STR$(EVT_STEP_getStepCount%(ev%)))    
      'TestObj_eventHandler% = 1
      'EXIT FUNCTION
    CASE EVT_KBD%
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
  
        IF focus% THEN
          Obj_registerInterest(obj_id%, EVT_STEP%)
        ELSE
          LOCAL dummy% = Obj_unregisterInterest%(obj_id%, EVT_STEP%)
        ENDIF
        
        IF key% = Q_KEY% THEN
          Obj_destroy(obj_id%)
        ENDIF
      ENDIF
    CASE EVT_JOY%
      LOCAL act% = EVT_JOY_getAction%(ev%)
      LM_writeLog("Joystick event action: "+STR$(act%))
      LOCAL button% = EVT_JOY_getButton%(ev%)
      LM_writeLog("Joystick event button: "+STR$(button%))      
      LOCAL position_x% = EVT_JOY_getPosX%(ev%)
      LOCAL position_y% = EVT_JOY_getPosY%(ev%)

      LM_writeLog("Joystick event position: X="+STR$(position_x%)+" Y="+STR$(position_y%)) 
  END SELECT
  
  TestObj_eventHandler% = 0
END FUNCTION
 
