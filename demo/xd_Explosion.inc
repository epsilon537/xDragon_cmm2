'Explosion object constructor.
FUNCTION xd_Explosion_create%(origObjId%, launchGameOverWhenDone%)
  LOCAL objId% = Obj_create%()
  'Link to animation.
  Obj_setAnim(objId%, "explosion")
  Obj_setType(objId%, OBJ_TYPE_XD_EXPLOSION%)
  
  Obj_registerInterest(objId%, EVT_STEP%) 'Receive step events.
  obj_eventH$(objId%) = "xd_Explosion_eventHandler%" 'Register event handler function.
  
  'Set time to live as long as animation duration.
  LOCAL aid% = obj_anim%(objId%)
  LOCAL ttl% = Anim_getNumFrames%(aid%)*anim_slowdownLimit%(aid%)
  
  obj_posX%(objId%) = obj_posX%(origObjId%)
  obj_posY%(objId%) = obj_posY%(origObjId%)
  
  'When ship explodes the launchGameOverWhenDone flag is set. Remember this in
  'the object's extra data field.
  Obj_setExtra(objId%, ttl% OR (launchGameOverWhenDone%<<31))
  
  xd_Explosion_create% = objId%
END FUNCTION

FUNCTION xd_Explosion_eventHandler%(objId%, ev%)
  IF EVT_getType%(ev%) = EVT_STEP% THEN 'Frame step event.
    LOCAL ttl% = Obj_getExtra%(objId%)
    'Extract launchGameOverWhenDone flag and the time-to-live counter.
    LOCAL launchGameOverWhenDone% = (ttl% AND &H80000000)
    ttl% = (ttl% AND &HFFFF) - 1
    IF ttl% <= 0 THEN 'Explosion expires...
      WM_markForDelete(objId%)
      'If the ship is done exploding, launch game-over.
      IF launchGameOverWhenDone% THEN
        dummy_i% = xd_GameOver_create%()
      ENDIF
    ENDIF
    Obj_setExtra(objId%, ttl% OR launchGameOverWhenDone%)
    
    xd_Explosion_eventHandler% = 1
    EXIT FUNCTION
  ENDIF
  
  xd_Explosion_eventHandler% = 0
END FUNCTION

