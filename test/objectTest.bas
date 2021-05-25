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

DIM obj1% = Obj_create%()
DIM obj2% = Obj_create%()

PRINT "Obj.1 id="+STR$(obj1%)
PRINT "Obj.2 id="+STR$(obj2%)

IF Obj_isAllocated%(obj1%) = 0 THEN
  ERROR "Object allocate failed."
ENDIF

IF Obj_isAllocated%(obj2%) = 0 THEN
  ERROR "Object allocate failed."
ENDIF

Obj_setType(obj1%, OBJ_TYPE_TEST%)
PRINT "Obj.1 type="+STR$(Obj_getType%(obj1%))
PRINT "Obj.2 type="+STR$(Obj_getType%(obj2%))

DIM vx0%=0, vy0%=0
IF vx0% <> obj_posX%(obj1%) THEN
  ERROR "Object default position failed.
ENDIF
IF vy0% <> obj_posY%(obj1%) THEN
  ERROR "Object default position failed.
ENDIF

IF vx0% <> obj_velX%(obj1%) THEN
  ERROR "Object default velocity failed.
ENDIF
IF vy0% <> obj_velY%(obj1%) THEN
  ERROR "Object default velocity failed.
ENDIF

IF Obj_getSolidness%(obj1%) <> OBJ_HARD% THEN
  ERROR "Default solidness error."
ENDIF

Obj_setSolidness(obj1%, OBJ_SOFT%)
IF Obj_getSolidness%(obj1%) <> OBJ_SOFT% THEN
  ERROR "Default solidness error."
ENDIF

Obj_setAlt(obj1%, 3)
IF Obj_getAlt%(obj1%) <> 3 THEN
  ERROR "Object altitude error."
ENDIF

Obj_destroy(obj1%)
Obj_destroy(obj2%)

IF Obj_isAllocated%(obj1%) <> 0 THEN
  ERROR "Object free failed."
ENDIF

IF Obj_isAllocated%(obj2%) <> 0 THEN
  ERROR "Object free failed."
ENDIF

IF obj_numAllocated%<> 0 THEN
  PRINT "Num Objs allocated: "+STR$(obj_numAllocated%)
  Error "Object leak."
ENDIF

GM_shutDown

PRINT "Test Completed."
END

FUNCTION TestObj_create%()
  LOCAL obj_id% = Obj_create%()
  Obj_setType(obj_id%, OBJ_TYPE_TEST%)
  obj_destroySub$(obj_id%) = "TestObj_destroy"  
  TestObj_create% = obj_id%
END FUNCTION

SUB TestObj_destroy(obj_id%)
  PRINT "TestObject destroyed: "+STR$(obj_id%)
  Obj_destroy obj_id% 'Invoke base class destructor.
END SUB

