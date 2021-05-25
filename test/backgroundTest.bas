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

DIM testObj% = TestObj_create%()

PAL_clr
PAL_load("../resources/xDragonDemo.pal")
PAL_allocStdColors
PAL_commitToCLUT

BG_load("../resources/background.bmp")

IF bg_width% <> (900<<4) THEN
  ERROR "BG_load error. Incorrect width."
ENDIF

IF bg_height% <> (480<<4) THEN
  ERROR "BG_load error. Incorrect height."
ENDIF

bg_enabled%=1
BG_setPos(0,0)

GM_run

LM_writeLog("Test Complete.")

GM_shutdown

END

FUNCTION TestObj_create%()
  LOCAL obj_id% = Obj_create%()
  Obj_setType(obj_id%, OBJ_TYPE_TEST%)
  Obj_registerInterest(obj_id%, EVT_KBD%)
  obj_drawSub$(obj_id%) = "Dummy_sub_i"
  obj_eventH$(obj_id%) = "TestObj_eventHandler%"  
  obj_destroySub$(obj_id%) = "TestObj_destroy"  
  TestObj_create% = obj_id%
END FUNCTION

SUB TestObj_destroy(obj_id%)
  Obj_destroy obj_id%
  PRINT "TestObject destroyed: "+STR$(obj_id%)
END SUB

FUNCTION TestObj_eventHandler%(obj_id%, ev%)  
  IF EVT_getType%(ev%) = EVT_KBD% THEN
    LOCAL action% = EVT_KBD_getAction%(ev%)
    LOCAL key% = EVT_KBD_getKey%(ev%)
  
    IF action% = EVT_KBD_ACT_KEY_PRESSED% THEN
      IF obj_id% = testObj% THEN        
        IF key%=C_KEY% THEN
          BG_setPos(100<<4,0)
        ENDIF
        IF key%=V_KEY% THEN
          BG_setPos(100<<4,100<<4)
        ENDIF
        
        IF key% = RIGHT_KEY% THEN
          INC bg_velX%
        ENDIF
        IF key% = LEFT_KEY% THEN
          INC bg_velX%, -1
        ENDIF
        IF key% = UP_KEY% THEN
          INC bg_velY%, -1
        ENDIF
        IF key% = DOWN_KEY% THEN
          INC bg_velY%        
        ENDIF        
      ENDIF      
    ENDIF
  ENDIF

  LM_writeLog("bg_velX = "+STR$(bg_velX%))
  LM_writeLog("bg_velY = "+STR$(bg_velY%))
        
  TestObj_eventHandler% = 1
END FUNCTION

