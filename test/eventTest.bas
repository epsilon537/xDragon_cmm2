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

GM_startUp

DIM event% = EVT_create%()

PRINT "Default event type "+EVT_NAMES$(EVT_getType%(event%))

EVT_setType(event%, EVT_TEST%)

PRINT "Event type after set "+EVT_NAMES$(EVT_getType%(event%))

DIM eventStep% = EVT_STEP_create%(10)

PRINT "Event step type "+EVT_NAMES$(EVT_getType%(eventStep%))

IF EVT_STEP_getStepCount%(eventStep%) <> 10 THEN
  Error "EventStep count error."
ENDIF

DIM eventStep2% = EVT_STEP_create%(20)

PRINT "Event step 2 type "+EVT_NAMES$(EVT_getType%(eventStep2%))

IF EVT_STEP_getStepCount%(eventStep2%) <> 20 THEN
  Error "EventStep count error."
ENDIF

DIM testObject% = TestObj_create%()

PRINT "Test Object type "+OBJ_NAMES$(Obj_getType%(testObject%))

DIM ehRes% = Obj_eventHandler%(testObject%, event%)
ehRes% = Obj_eventHandler%(testObject%, eventStep%)
ehRes% = Obj_eventHandler%(testObject%, eventStep2%)

Obj_destroy(testObject%)

IF obj_numAllocated%<> 0 THEN
  ERROR "Object leak. Num Objs allocated: "+STR$(obj_numAllocated%)
ENDIF

GM_shutDown

PRINT "Test Completed."
END

FUNCTION TestObj_create%()
  LOCAL obj_id% = Obj_create%()
  Obj_setType(obj_id%, OBJ_TYPE_TEST%)
  obj_eventH$(obj_id%) = "TestObj_eventHandler%"  
  obj_destroySub$(obj_id%) = "TestObj_destroy"  
  TestObj_create% = obj_id%
END FUNCTION

SUB TestObj_destroy(obj_id%)
  PRINT "TestObject destroyed: "+STR$(obj_id%)
  Obj_defaultDestroy obj_id%
END SUB

FUNCTION TestObj_eventHandler%(obj_id%, ev%)
  PRINT "TestObject eventHandler received event "+EVT_NAMES$(EVT_getType%(ev%))
  TestObj_eventHandler% = 1
END FUNCTION

