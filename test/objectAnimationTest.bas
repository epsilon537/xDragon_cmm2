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

Box_scale(wm_boundary%, 10.0)

Sheet_load("../resources/spritesheet.bmp")
Anim_load("../resources/enemy.csv", "Saucer")
Anim_load("../resources/ship.csv", "Ship")

DIM ship% = TestObj_create%()
DIM saucer% = TestObj_create%()

DIM shipBox% = Obj_getBox%(ship%)

IF (box_cornerX%(shipBox%) <> 0) OR (box_cornerY%(shipBox%) <> 0) THEN
  ERROR "Box_getCorner error."
ENDIF
IF box_width%(shipBox%) <> 16 THEN
  ERROR "Box_getHorizontal error."
ENDIF
IF box_height%(shipBox%) <> 16 THEN
  ERROR "Box_getVertical error."
ENDIF

Obj_setAnim(ship%, "Ship")
obj_posX%(ship%) = 16*MM.HRES/3
obj_posY%(ship%) = 16*MM.VRES/2

obj_velX%(ship%) = 16
obj_velY%(ship%) = 8

IF box_cornerX%(shipBox%) <> -12*16 THEN
  ERROR "Box_getCorner error"
ENDIF
IF box_cornerY%(shipBox%) <> -8*16 THEN
  ERROR "Box_getCorner error"
ENDIF

IF box_width%(shipBox%) <> 24*16 THEN
  ERROR "Box_getHorizontal error."
ENDIF
IF box_height%(shipBox%) <> 16*16 THEN
  ERROR "Box_getVertical error."
ENDIF

Obj_setAnim(saucer%, "Saucer")
obj_posX%(saucer%) = 16*MM.HRES*2/3
obj_posY%(saucer%) = 16*MM.VRES/2
obj_velX%(saucer%) = -16/2
obj_velY%(saucer%) = 16

GM_run

Anim_unload("Saucer")
Anim_unload("Ship")

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
  'IF Anim_getLabel$(obj_anim%(obj_id%)) = "Saucer" THEN
  '  LM_writeLog("Saucer drawn "+STR$(gm_frameCount%))
  'ENDIF  
  Obj_defaultDraw(obj_id%)
END SUB

FUNCTION TestObj_eventHandler%(obj_id%, ev%)
  IF EVT_getType%(ev%) = EVT_OUT% THEN
    LM_writeLog("Event Out for obj "+STR$(obj_id%))
    obj_velX%(obj_id%) = -obj_velX%(obj_id%)
    obj_velY%(obj_id%) = -obj_velY%(obj_id%)
  ELSEIF EVT_getType%(ev%) = EVT_STEP% THEN
    'LM_writeLog("OBJ_id "+STR$(obj_id%)+" Event Step count ="+STR$(EVT_STEP_getStepCount%(ev%)))

    IF EVT_STEP_getStepCount%(ev%) = 100 THEN
      WM_setViewPos%(30*16, -30*16)
    ELSEIF EVT_STEP_getStepCount%(ev%) = 250 THEN
      LOCAL aid% = OBJ_getAnim%(obj_id%)
      LM_writeLog("Doubled slowdown")
      anim_slowdownLimit%(aid%) = anim_slowdownLimit%(aid%)*2
    ELSEIF EVT_STEP_getStepCount%(ev%) = 500 THEN
      LM_writeLog("Freeze")
      LOCAL aid% = OBJ_getAnim%(obj_id%)
      anim_slowdownCount%(aid%) = -1
    ENDIF
  ENDIF  
  
  TestObj_eventHandler% = 0
END FUNCTION
