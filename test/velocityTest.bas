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

DIM testObjs%(3)
testObjs%(0) = TestObj_create%()
testObjs%(1) = TestObj_create%()
testObjs%(2) = TestObj_create%()
testObjs%(3) = TestObj_create%()

Obj_setExtra(testObjs%(0), RGB(yellow))
Obj_setExtra(testObjs%(1), RGB(blue))
Obj_setExtra(testObjs%(2), RGB(white))
Obj_setExtra(testObjs%(3), RGB(green))

obj_posX%(testObjs%(0)) = MM.HRES*16/2
obj_posY%(testObjs%(0)) = MM.VRES*16/2
obj_velX%(testObjs%(0)) = 16
obj_velY%(testObjs%(0)) = 16

obj_posX%(testObjs%(1)) = MM.HRES*16/2
obj_posY%(testObjs%(1)) = MM.VRES*16/2
obj_velX%(testObjs%(1)) = -16
obj_velY%(testObjs%(1)) = -16

obj_posX%(testObjs%(2)) = MM.HRES*16/2
obj_posY%(testObjs%(2)) = MM.VRES*16/2
obj_velX%(testObjs%(2)) = 16
obj_velY%(testObjs%(2)) = -16

obj_posX%(testObjs%(3)) = MM.HRES*16/2
obj_posY%(testObjs%(3)) = MM.VRES*16/2
obj_velX%(testObjs%(3)) = -16
obj_velY%(testObjs%(3)) = 16

LM_writeLog "Frame Counter: "+STR$(gm_frameCount%)
LM_writeLog "Frame Time: "+STR$(GM_frame_time_ms!)
LM_writeLog "Game Over Status: "+STR$(gm_gameOver%)

LM_writeLog "Starting game loop"

GM_run

LM_writeLog "Game loop ended."
LM_writeLog "Frame Counter: "+STR$(gm_frameCount%)
LM_writeLog "Frame Time: "+STR$(GM_frame_time_ms!)
LM_writeLog "Game Over Status: "+STR$(gm_gameOver%)

GM_shutDown

PRINT "GM_isStarted="+STR$(GM_is_Started%)
PRINT "lm_enabled="+STR$(LM_is_Started%)
PRINT "wm_isStarted="+STR$(WM_is_Started%)

IF objLst_numAllocated%<> 0 THEN
  PRINT "Num ObjLists allocated: "+STR$(objLst_numAllocated%)
  Error "ObjectList leak."
ENDIF

IF obj_numAllocated%<> 0 THEN
  PRINT "Num Objs allocated: "+STR$(obj_numAllocated%)
  Error "Object leak."
ENDIF

LM_writeLog "Test completed."+STR$(LM_is_Started%)

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
  Obj_defaultDestroy obj_id% 'destroy base class object
END SUB

FUNCTION TestObj_eventHandler%(obj_id%, ev%)
  TestObj_eventHandler% = 1
END FUNCTION


SUB TestObj_draw(obj_id%)
  LOCAL col% = Obj_getExtra%(obj_id%)
  DM_drawCh(obj_posX%(obj_id%), obj_posY%(obj_id%), STR$(obj_id%), col%)  
END SUB

