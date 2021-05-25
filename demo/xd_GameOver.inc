
DIM gameOverTTL% 'Game-Over time-to-live counter
DIM gameOverVol% = 100 'Volume level, used for fading.

'GameOver object constructor.
FUNCTION xd_GameOver_create%()
  LOCAL objId% = Obj_create%()

  Obj_setType(objId%, OBJ_TYPE_XD_GAMEOVER%)  
  Obj_setAnim(objId%, "gameover") 'Link to gameover animation.

  LOCAL vx%, vy%
  WM_getViewPos(vx%, vy%) 'Retrieve view position
  
  'Put Game-Over in center of window. Add view position because object position is in world coordinates.
  obj_posX%(objId%) = vx% + MM.HRES*16\2
  obj_posY%(objId%) = vy% + MM.VRES*16\2    

  'Set Game-Over time-to-live as long as animation duration.
  LOCAL aid% = obj_anim%(objId%)  
  Anim_reset(aid%)
  gameOverTTL% = Anim_getNumFrames%(aid%)*anim_slowdownLimit%(aid%)
  anim_loopRewindIdx%(aid%) = 7 'So the animation doesn't loop

  Obj_registerInterest(objId%, EVT_STEP%) 'Receive step events.

  obj_eventH$(objId%) = "xd_GameOver_eventHandler%" 'Register event handler function.
  obj_destroySub$(objId%) = "xd_GameOver_destroy" 'Object destructor.
  
  gameOverVol% = 100 'Start fade at full volume.

  xd_GameOver_create% = objId%
END FUNCTION


FUNCTION xd_GameOver_eventHandler%(objId%, ev%)
  LOCAL eType% = EVT_getType%(ev%)

  IF eType% = EVT_STEP% THEN 'Frame step event.
    xd_GameOver_step objId%
    xd_GameOver_eventHandler% = 1
    EXIT FUNCTION
  ENDIF
  
  xd_GameOver_eventHandler% = 0
END FUNCTION

SUB xd_GameOver_step(objId%)
  INC gameOverTTL%, -1
  IF gameOverTTL% = 0 THEN 'When time-to-live reaches zero, start fade-out.
    PAL_startCLUTtransition(xd_Intro_fadePal%(), 16, 0)
  ELSE IF gameOverTTL% < 0 THEN
    IF NOT PAL_CLUTtransitionIsDone%() THEN
      'Fade out sound as well.
      PLAY VOLUME gameOverVol%, gameOverVol%
      INC gameOverVol%, -3
      IF gameOverVol%<0 THEN
        gameOverVol%=0
      ENDIF

      PAL_CLUTtransitionStep
    ELSE
      Sound_stop
      WM_markForDelete(objId%)
    ENDIF
  ENDIF
END SUB

'Object Destructor.
SUB xd_GameOver_destroy(objId%)  
  'Unload sounds so intro music can play
  xd_unloadSounds
  
  'Remove Enemies and ViewObjects, re-active GameStart
  LOCAL allObjLid% = ObjLst_create%()
  WM_getAllObjs(allObjLid%, 1)

  LOCAL curObj%, index%=0
    
  DO WHILE index% < objLst_numElems%(allObjLid%)
    curObj% = objLst_list%(index%, allObjLid%)

    SELECT CASE Obj_getType%(curObj%)
      CASE OBJ_TYPE_XD_ENEMY%, OBJ_TYPE_VIEW_OBJECT%, OBJ_TYPE_XD_POINTS%
        WM_markForDelete(curObj%)
        
      CASE OBJ_TYPE_XD_INTRO%
        PAUSE 1000 'A bit hairy, but it works fine.
        xd_Intro_start curObj%
    END SELECT
    
    INC index%
  LOOP

  ObjLst_destroy(allObjLid%) 'Don't forget to release ObjectList object, or we have a resource leak.
  Obj_defaultDestroy(objId%) 'Base object destructor needs to be called as well.
END SUB

