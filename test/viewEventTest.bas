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

DIM obj% = TestObj_create%()
DIM vo% = VObj_create%()
VObj_setViewString(vo%, "Points")
VObj_setEventTag(vo%, VIEW_EVT_TAG_POINTS%)
VObj_commit(vo%)

GM_run

GM_shutDown

END

FUNCTION TestObj_create%()
  LOCAL obj_id% = Obj_create%()
  Obj_setType(obj_id%, OBJ_TYPE_TEST%)
  Obj_registerInterest(obj_id%, EVT_STEP%)
  obj_eventH$(obj_id%) = "TestObj_eventHandler%"  
  obj_drawSub$(obj_id%) = "Dummy_sub_i"
  obj_destroySub$(obj_id%) = "TestObj_destroy"  
  TestObj_create% = obj_id%
END FUNCTION

SUB TestObj_destroy(obj_id%)
  PRINT "TestObject destroyed: "+STR$(obj_id%)
  Obj_defaultDestroy obj_id%
END SUB

FUNCTION TestObj_eventHandler%(obj_id%, ev%)
  IF EVT_getType%(ev%) = EVT_STEP% THEN
    LOCAL frameCount% = gm_frameCount%
    LOCAL dummy%
   
    IF frameCount% = 100 THEN
      LOCAL ev1% = EVT_VIEW_create%(VIEW_EVT_TAG_POINTS%, 10, 1)
      IF EVT_VIEW_getTag%(ev1%) <> VIEW_EVT_TAG_POINTS% THEN
        ERROR "EVT_VIEW_getTag error."
      ENDIF
      IF EVT_VIEW_getValue%(ev1%) <> 10 THEN
        ERROR "EVT_VIEW_getValue error."
      ENDIF
      IF EVT_VIEW_getDelta%(ev1%) <> 1 THEN
        ERROR "EVT_VIEW_getDelta error."
      ENDIF
      
      dummy% = EVT_MGR_onEvent%(ev1%)
    ENDIF

    IF frameCount% = 200 THEN
      LOCAL ev2% = EVT_VIEW_create%(VIEW_EVT_TAG_POINTS%, 10, 1)
      IF EVT_VIEW_getTag%(ev2%) <> VIEW_EVT_TAG_POINTS% THEN
        ERROR "EVT_VIEW_getTag error."
      ENDIF
      IF EVT_VIEW_getValue%(ev2%) <> 10 THEN
        ERROR "EVT_VIEW_getValue error."
      ENDIF
      IF EVT_VIEW_getDelta%(ev2%) <> 1 THEN
        ERROR "EVT_VIEW_getDelta error."
      ENDIF
      
      dummy% = EVT_MGR_onEvent%(ev2%)
    ENDIF

    IF frameCount% = 300 THEN
      LOCAL ev3% = EVT_VIEW_create%(VIEW_EVT_TAG_POINTS%, 10, 1)
      EVT_VIEW_setDelta(ev3%, 0)
      EVT_VIEW_setValue(ev3%, 1000)
      
      IF EVT_VIEW_getTag%(ev3%) <> VIEW_EVT_TAG_POINTS% THEN
        ERROR "EVT_VIEW_getTag error."
      ENDIF
      IF EVT_VIEW_getValue%(ev3%) <> 1000 THEN
        ERROR "EVT_VIEW_getValue error."
      ENDIF
      IF EVT_VIEW_getDelta%(ev3%) <> 0 THEN
        ERROR "EVT_VIEW_getDelta error."
      ENDIF
      
      dummy% = EVT_MGR_onEvent%(ev3%)
    ENDIF
    
    IF frameCount% = 400 THEN
      VObj_setViewString(vo%, "ValMustNotChange")    
      VObj_commit(vo%)
    ENDIF

    IF frameCount% = 500 THEN
      LOCAL ev5% = EVT_VIEW_create%(VIEW_EVT_TAG_POINTS%, 10, 1)
      EVT_VIEW_setDelta(ev5%, 0)
      EVT_VIEW_setValue(ev5%, 1000)
      EVT_VIEW_setTag(ev5%, VIEW_EVT_TAG_TEST%)
      
      IF EVT_VIEW_getTag%(ev5%) <> VIEW_EVT_TAG_TEST% THEN
        ERROR "EVT_VIEW_getTag error."
      ENDIF
  
      dummy% = EVT_MGR_onEvent%(ev5%)
    ENDIF
  ENDIF
  
  TestObj_eventHandler% = 0
END FUNCTION

