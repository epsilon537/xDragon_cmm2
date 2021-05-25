DIM xd_pointsObj%

'Points ViewObject constructor.
FUNCTION xd_Points_create%()
  LOCAL objId% = VObj_create%() 'Points derives from ViewObject
  Obj_setType(objId%, OBJ_TYPE_XD_POINTS%)
  
  VObj_setLocation(objId%, VOBJ_LOC_TOP_RIGHT%)
  VObj_setViewString(objId%, "Points")
  VObj_setEventTag(objId%, VIEW_EVT_TAG_POINTS%)
  VObj_setColor(objId%, MAP(pal_redIdx%))
  VObj_commit(objId%) 'ViewObject settings need to be committed to take effect.
  
  Obj_registerInterest(objId%, EVT_STEP%) 'Receive step events
  obj_eventH$(objId%) = "xd_Points_eventHandler%" 'Event handler function.

  xd_pointsObj% = objId%  
  xd_Points_create% = objId%
END FUNCTION

FUNCTION xd_Points_eventHandler%(objId%, ev%)
  IF EVT_getType%(ev%) = EVT_STEP% THEN 'Frame Step event.
    IF (EVT_STEP_getStepCount%(ev%) AND 63) = 0 THEN
      VObj_setValue(objId%, VObj_getValue%(objId%) + 1) 'Increase points every 64 frames.
      VObj_commit(objId%)
      xd_Points_eventHandler% = 1
    ENDIF
    
    'Graduallly go faster...
    IF (EVT_STEP_getStepCount%(ev%) AND 511) = 0 THEN
      INC wm_viewVelX%, 8
      INC bg_velX%, 4
      xd_playSound XD_SOUND_SPEEDUP%
    ENDIF
    
    EXIT FUNCTION
  ELSE
    'Defer to parent ViewObject
    xd_Points_eventHandler% = VObj_eventHandler%(objId%, ev%)
    EXIT FUNCTION
  ENDIF
  
  xd_Points_eventHandler% = 0
END FUNCTION

'Invoked when ship explodes so we should no longer collect points.
SUB xd_Points_stopCounting
  dummy_i% = Obj_unregisterInterest%(xd_pointsObj%, EVT_STEP%)  
END SUB

