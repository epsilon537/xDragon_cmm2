OPTION EXPLICIT
OPTION DEFAULT NONE
OPTION BASE 0
OPTION CONSOLE BOTH

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

GM_startup

PRINT "Test started."

DIM testObject% = TestObj_create%()
DIM testObject2% = TestObj_create%()

DO
  IM_getInput
LOOP

END

FUNCTION TestObj_create%()
  LOCAL obj_id% = Obj_create%()
  Obj_setType(obj_id%, OBJ_TYPE_TEST%)
  Obj_registerInterest(obj_id%, EVT_KBD%)
  Obj_registerInterest(obj_id%, EVT_JOY%)
  obj_eventH$(obj_id%) = "TestObj_eventHandler%"
  obj_destroySub$(obj_id%) = "TestObj_destroy"  
  TestObj_create% = obj_id%
END FUNCTION

SUB TestObj_destroy(obj_id%)
  PRINT "TestObject destroyed: "+STR$(obj_id%)
  Obj_defaultDestroy obj_id%
END SUB

FUNCTION TestObj_eventHandler%(obj_id%, ev%)

  IF EVT_getType%(ev%) = EVT_KBD% THEN

    LOCAL action% = EVT_KBD_getAction%(ev%)
    LOCAL key% = EVT_KBD_getKey%(ev%)
    
    SELECT CASE action%
      CASE EVT_KBD_ACT_KEY_PRESSED%
        PRINT "Obj. "+STR$(obj_id%)+" Key pressed: "+STR$(key%)
      CASE EVT_KBD_ACT_KEY_RELEASED%
        PRINT "Obj. "+STR$(obj_id%)+" Key released: "+STR$(key%)
      CASE EVT_KBD_ACT_KEY_DOWN%
        'PRINT STR$(obj_id%)+" Key down: "+STR$(key%)
      CASE ELSE
        Error "Unknown action"
    END SELECT
  ELSE IF EVT_getType%(ev%) = EVT_JOY% THEN
    LOCAL action% = EVT_JOY_getAction%(ev%)

    SELECT CASE action%
      CASE EVT_JOY_ACT_PRESSED%
        'PRINT "Joy button pressed "+STR$(EVT_JOY_getButton%(ev%))        
      CASE EVT_JOY_ACT_CLICKED%
        PRINT "Joy button clicked "+STR$(EVT_JOY_getButton%(ev%))        
      CASE EVT_JOY_ACT_MOVED%
        LOCAL x%, y%
        EVT_JOY_getPos(ev%, x%, y%)
        PRINT "Joy button moved :"+STR$(x%)+" "+STR$(y%)
      CASE ELSE      
        Error "Unknown action"
    END SELECT

  ELSE    
    Error "Incorrect event type"
  ENDIF
  
  TestObj_eventHandler% = 1
END FUNCTION

