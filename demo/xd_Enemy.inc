'Enemy object constructor.
FUNCTION xd_Enemy_create%(xPos%, yPos%)
  LOCAL objId% = Obj_create%()

  Obj_registerInterest(objId%, EVT_XD_NUKE%) 'Receive nuke events.
  
  Obj_setAnim(objId%, "Enemy") 'Link to animation.
  Obj_setType(objId%, OBJ_TYPE_XD_ENEMY%)

  'Set speed in horizontal direction.
  obj_velX%(objId%) = -16
  
  Obj_setSolidness(objId%, OBJ_HARD%) 'Enemies can't move through other objects.
  Obj_setColGroup(objId%, CG_ENEMY%) 'Enemies belong to Enemy collision group.
  
  obj_posX%(objId%) = xPos%
  obj_posY%(objId%) = yPos%
  
  obj_eventH$(objId%) = "xd_Enemy_eventHandler%" 'Register event handler function
  obj_destroySub$(objId%) = "xd_Enemy_destroy" 'Register destructor sub
  
  xd_Enemy_create% = objId%
END FUNCTION

FUNCTION xd_Enemy_eventHandler%(objId%, ev%)
  LOCAL eType% = EVT_getType%(ev%)
  
  IF eType% = EVT_OUT% THEN 'Enemy leaves the screen.
    xd_Enemy_out(objId%)
    xd_Enemy_eventHandler% = 1  
  ELSEIF eType% = EVT_COL% THEN 'Collision.
    xd_Enemy_hit(objId%)
    xd_Enemy_eventHandler% = 1  
  ELSEIF eType% = EVT_XD_NUKE% THEN 'Nuke event.
    xd_Enemy_hit(objId%)  
    xd_Enemy_eventHandler% = 1
  ENDIF
  
  xd_Enemy_eventHandler% = 0
END FUNCTION

SUB xd_Enemy_out(objId%)
  WM_markForDelete(objId%) 'Remove the enemy object.
END SUB

SUB xd_Enemy_hit(objId%)
  'Create an explosion
  dummy_i% = xd_Explosion_create%(objId%, 0)
    
  WM_markForDelete(objId%) 'Remove the enemy object.

  xd_playSound XD_SOUND_EXPLODE%
END SUB

'Enemy object destructor.
SUB xd_Enemy_destroy(objId%)
  'Send view event with points to interested objects
  LOCAL ev% = EVT_View_create%(VIEW_EVT_TAG_POINTS%, 10, 1)
  dummy_i% = EVT_MGR_onEvent%(ev%)

  LM_writeLog("Enemy destroy")
  Obj_defaultDestroy(objId%) 'Invoke base object destructor as well.
END SUB
