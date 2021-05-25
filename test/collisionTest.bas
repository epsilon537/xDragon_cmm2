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

DIM collision_count% = 0

GM_startUp
LM_writeLog "GM_isStarted="+STR$(GM_isStarted%)
LM_writeLog "lm_enabled="+STR$(lm_enabled%)
LM_writeLog "wm_isStarted="+STR$(WM_isStarted%)

CONST CG_SOLID% = CG_ID_1%
CONST CG_SOLID2% = CG_ID_2%

CONST CG_SOLID_MSK% = CG_ID_1_MSK%
CONST CG_SOLID2_MSK% = CG_ID_2_MSK%

cg_maps%(CG_SOLID%) = CG_SOLID_MSK%
cg_maps%(CG_SOLID2%) = CG_SOLID2_MSK%

DIM obj0% = TestObj_create%()
DIM obj1% = TestObj_create%()
DIM obj2% = TestObj_create%()
DIM obj3% = TestObj_create%()

Obj_setExtra(obj0%, RGB(yellow))
Obj_setExtra(obj1%, RGB(blue))
Obj_setExtra(obj2%, RGB(yellow))
Obj_setExtra(obj3%, RGB(blue))

DIM vx%, vy%

vx%= MM.HRES*16*0.25
vy% = MM.VRES*16*0.5
obj_posX%(obj0%) = vx%
obj_posY%(obj0%) = vy%
obj_velX%(obj0%) = 8
obj_velY%(obj0%) = 0
Obj_setSolidness(obj0%, OBJ_HARD%)
Obj_setColGroup(obj0%, CG_SOLID%)

vx%= MM.HRES*16*0.75
vy% = MM.VRES*16*0.5
obj_posX%(obj1%) = vx%
obj_posY%(obj1%) = vy%
obj_velX%(obj1%) = -8
obj_velY%(obj1%) = 0
Obj_setSolidness(obj1%, OBJ_HARD%)
Obj_setColGroup(obj1%, CG_SOLID%)

vx%= MM.HRES*16*0.25
vy% = MM.VRES*16*0.75
obj_posX%(obj2%) = vx%
obj_posY%(obj2%) = vy%
obj_velX%(obj2%) = 8
obj_velY%(obj2%) = 0
Obj_setSolidness(obj2%, OBJ_SOFT%)
Obj_setColGroup(obj2%, CG_SOLID%)

vx%= MM.HRES*16*0.75
vy% = MM.VRES*16*0.75
obj_posX%(obj3%) = vx%
obj_posY%(obj3%) = vy%
obj_velX%(obj3%) = -8
obj_velY%(obj3%) = 0
Obj_setSolidness(obj3%, OBJ_SOFT%)
Obj_setColGroup(obj3%, CG_SOLID%)

LM_writeLog "Frame Counter: "+STR$(gm_frameCount%)
LM_writeLog "Frame Time: "+STR$(GM_frame_time_ms!)
LM_writeLog "Game Over Status: "+STR$(gm_gameOver%)

LM_writeLog "Starting game loop"

GM_run

LM_writeLog "Game loop ended. Duration: "+STR$(Clock_split_us%(test_clock%))+"us"
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
  Obj_defaultDestroy obj_id%
  PRINT "TestObject destroyed: "+STR$(obj_id%)
END SUB

FUNCTION TestObj_eventHandler%(obj_id%, ev%)
  TestObj_eventHandler% = 1
  
  LOCAL eType% = EVT_getType%(ev%)
  
  IF eType% = EVT_COL% THEN
    INC collision_count%
    LOCAL objId1% = EVT_COL_getObj1%(ev%)
    LOCAL objId2% = EVT_COL_getObj2%(ev%)
    LOCAL vx%, vy%
    
    EVT_COL_getPos(ev%, vx%, vy%)

    LM_writeLog "Obj "+STR$(objId1%)+" collision with obj "+STR$(objId2%)+" at position "+STR$(vx%>>>4) + " " + STR$(vy%>>>4))
    
    Obj_setExtra(obj_id%, RGB(red))

    IF collision_count% MOD 200 = 0 THEN
      WM_markForDelete(obj_id%)
    ENDIF
    
    IF collision_count% = 200 THEN   
      LOCAL obj4% = TestObj_create%()
      Obj_setExtra(obj4%, RGB(yellow))
      Obj_setColGroup(obj4%, CG_ID_SPECTRAL%)

      obj_posX%(obj4%) = MM.HRES*0.25*16
      obj_posY%(obj4%) = MM.VRES*0.25*16      
      obj_velX%(obj4%) = 8
      obj_velY%(obj4%) = 0

      LOCAL obj5% = TestObj_create%()
      Obj_setExtra(obj5%, RGB(blue))
      Obj_setColGroup(obj5%, CG_ID_SPECTRAL%)

      obj_posX%(obj5%) = MM.HRES*0.75*16
      obj_posY%(obj5%) = MM.VRES*0.25*16
      obj_velX%(obj5%) = -8
      obj_velY%(obj5%) = 0
      
      
      LOCAL obj6% = TestObj_create%()
      obj_posX%(obj6%) = MM.HRES*0.25*16
      obj_posY%(obj6%) = MM.VRES*0.75*16
      obj_velX%(obj6%) = 8
      obj_velY%(obj6%) = 0
      
      Obj_setSolidness(obj6%, OBJ_HARD%)
      Obj_setColGroup(obj6%, CG_SOLID2%)
      Obj_setExtra(obj6%, RGB(white))

      LOCAL obj7% = TestObj_create%()      
      obj_posX%(obj7%) = MM.HRES*0.75*16
      obj_posY%(obj7%) = MM.VRES*0.75*16
      obj_velX%(obj7%) = -8
      obj_velY%(obj7%) = 0
      Obj_setSolidness(obj6%, OBJ_HARD%)
      Obj_setColGroup(obj7%, CG_SOLID2%)
      IF Obj_getColGroup%(obj7%) <> CG_SOLID2% THEN
        ERROR "Obj_setColGroup error."
      ENDIF      
      Obj_setExtra(obj7%, RGB(white))
    ENDIF
  ELSE IF eType% = EVT_OUT% THEN
    LM_writeLog "Obj "+STR$(obj_id%)+" out of bounds."
    obj_velX%(obj_id%) = -obj_velX%(obj_id%)
    obj_velY%(obj_id%) = -obj_velY%(obj_id%)
  ENDIF
END FUNCTION


SUB TestObj_draw(obj_id%)
  LOCAL x% = obj_posX%(obj_id%)
  LOCAL y% = obj_posY%(obj_id%)  
  LOCAL col% = Obj_getExtra%(obj_id%)
  
  DM_drawCh(x%, y%, STR$(obj_id%), col%)  
END SUB

