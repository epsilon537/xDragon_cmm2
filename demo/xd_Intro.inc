DIM xd_Intro_fadePal%(PAL_NUM_ENTRIES%-1) 'The all-black palette to fade from/to.
DIM xd_Intro_startGameFlg% = 0 'Set when the player presses the key/button to start the game.
DIM xd_IntroVol% = 100 'Music volume.

'Intro object constructor
FUNCTION xd_Intro_create%()
  LOCAL objId% = Obj_create%() 'Object base class constructor.
  Obj_setType(objId%, OBJ_TYPE_XD_INTRO%) 'Set object type to into.  
  Obj_setAnim(objId%, "intro") 'Select the intro animation.
  
  'Put object in center of window. Objects are centered around their position.
  obj_posX%(objId%) = MM.HRES*16\2
  obj_posY%(objId%) = MM.VRES*16\2    

  'Indicate that this object wants to receive Step, Keyboard and Joystick events.
  Obj_registerInterest(objId%, EVT_KBD%)
  Obj_registerInterest(objId%, EVT_JOY%)  
  obj_eventH$(objId%) = "xd_Intro_eventHandler%" 'Specify the event handler function.

  'Set the fade palette array to all-black.
  LOCAL ii%
  FOR ii%=0 TO PAL_NUM_ENTRIES%-1
    xd_Intro_fadePal%(ii%)=0
  NEXT ii%
  
  xd_Intro_start objId% 'Kick-off the intro.
  xd_Intro_create% = objId% 'Return the object id.
END FUNCTION

'Start or Restart Intro.
SUB xd_Intro_start(objId%)
  Obj_setActive(objId%, 1) 'If we're restarting, this object is inactive. Make it active again.
  dummy_i% = WM_setViewPos%(0, 0) '(Re)Set View position to origin.
  xd_Intro_startGameFlg% = 0 'Reset start game flag.
  te_enabled% = 0 'Disable tile engine
  te_trigMapEnabled% = 0 'Disable tile engine triggers
  bg_enabled% = 0 'Disable background layer.
  BG_clrPage 'Clear background page
  TE_clrPage 'Clear tile page
  xd_Intro_playMusic 'Start intro music
  PAL_startCLUTtransition(xd_Intro_fadePal%(), 12, 1) 'Start fade-in, in 12 steps.
  Obj_registerInterest(objId%, EVT_STEP%) '(Re)register interest in step events.
END SUB

'Intro object event handler.
FUNCTION xd_Intro_eventHandler%(objId%, ev%)
  LOCAL eType% = EVT_getType%(ev%) 'Retrieve event type.

  'Step events are generated every frame.
  IF eType% = EVT_STEP% THEN
    IF NOT xd_Intro_startGameFlg% THEN 'We're not starting the game yet. We're just fading in.   
      PAL_CLUTtransitionStep 'Next fade step.
      IF PAL_CLUTtransitionIsDone%() THEN 'Done fading in?
        dummy_i% = Obj_unregisterInterest%(objId%, EVT_STEP%) 'Remove ourselves from step event.
      ENDIF
    ELSE 'The player pressed the key/button to start the game. Fade out.
      'Fade out music.
      PLAY VOLUME xd_IntroVol%, xd_IntroVol%
      INC xd_IntroVol%, -3
      IF xd_IntroVol%<0 THEN
        xd_IntroVol%=0
      ENDIF
      'and fade out palette.
      PAL_CLUTtransitionStep
      IF PAL_CLUTtransitionIsDone%() THEN 'Done fading out?
        Sound_stop
        PAUSE 2000
        xd_Intro_startGame objId% 'Start the game
      ENDIF
    ENDIF
  ENDIF

  IF eType% = EVT_KBD% THEN 'We have keyboard input.
    IF EVT_KBD_getAction%(ev%) = EVT_KBD_ACT_KEY_PRESSED% THEN
      SELECT CASE EVT_KBD_getKey%(ev%) 
        CASE P_KEY% 'P starts the game.
          xd_Intro_startGameFlg% = 1
          LM_writeLog("P pressed")
          'Start fade-out and reregister interest in step events.
          PAL_startCLUTtransition(xd_Intro_fadePal%(), 12, 0)
          Obj_registerInterest(objId%, EVT_STEP%)  
          xd_Intro_eventHandler% = 1
          EXIT FUNCTION
        CASE Q_KEY% 'Quit the game.
          GM_setGameOver 1
          xd_Intro_eventHandler% = 1
          EXIT FUNCTION
        CASE S_KEY%
          dm_saveImage%=1
      END SELECT
    ENDIF
  ELSE IF eType% = EVT_JOY% THEN 'Joystick events
    IF EVT_JOY_getAction%(ev%) = EVT_JOY_ACT_CLICKED% THEN
      IF EVT_JOY_getButton%(ev%) = EVT_JOY_BUTTON_Z% THEN
        xd_Intro_startGameFlg% = 1
        LM_writeLog("Z pressed")
        'Start fade-out and reregister interest in step events.
        PAL_startCLUTtransition(xd_Intro_fadePal%(), 8, 0)
        Obj_registerInterest(objId%, EVT_STEP%)  
        xd_Intro_eventHandler% = 1
        EXIT FUNCTION
      ENDIF
    ENDIF
  ENDIF
  
  'If we get here, ignore the event.
  xd_Intro_eventHandler% = 0
END FUNCTION

SUB xd_Intro_playMusic
  xd_IntroVol% = 100
  PLAY VOLUME xd_IntroVol%, xd_IntroVol%
  'Intro music is a WAV. Just for illustration purposes.
  'I recommend using .mods and the MMBasic MOD API instead (see further. MOD is used in-game).
  Sound_play "../resources/industrial.wav", 1
END SUB

SUB xd_Intro_startGame(objId%)
  dummy_i% = xd_Ship_create%() 'Our here.
  dummy_i% = xd_Points_create%() 'The points ViewObject.
  
  'Count of nukes
  LOCAL nuke_vo% = VObj_create%() 'Also a ViewObject.
  VObj_setLocation(nuke_vo%, VOBJ_LOC_TOP_LEFT%)
  VObj_setViewString(nuke_vo%, "Nukes")
  VObj_setValue(nuke_vo%, 1) 'Player gets 1 nuke.
  VObj_setColor(nuke_vo%, RGB(yellow))
  VObj_setEventTag(nuke_vo%, VIEW_EVT_TAG_XD_NUKES%) 'Sending a ViewEvent with this tag goes to this ViewObject.
  VObj_commit(nuke_vo%) 'ViewObject changes need to be committed to take effect.
    
  'When game starts, intro object becomes inactive.
  Obj_setActive objId%, 0

  'Load the in-game modfile containing both music and sound effects.
  xd_loadSounds
    
  te_enabled% = 1 'Enable Tile Engine
  te_trigMapEnabled% = 1 'Enable Tile Engine Triggers
  bg_enabled% = 1 'Enable Background Layer
  BG_setPos 0, 0 'Set background starting position
END SUB

