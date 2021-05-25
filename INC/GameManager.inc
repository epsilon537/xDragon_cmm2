'Private variables and constants
'-------------------------------
DIM _gm_logCallOut$ = "_LM_LOG_CALLOUT"+CHR$(0)
DIM _gm_loopTimeMs!
DIM _gm_swapBufferTimeMs!
DIM _gm_onEventTimeMs!
DIM _gm_inputTimeMs!
DIM _gm_updateTimeMs!
DIM _gm_bgUpdateTimeMs!
DIM _gm_drawTimeMs!
DIM _gm_timerLatchMs!

'Public constants
'----------------
'Frame time (game loop time) in ms (26.6ms=75fps/2)
CONST GM_FRAME_TIME_MS! = 26.6

'Minimum loop time. If we get through the loop faster than this, we wait, to ensure
'that game loop time remains 75fps/2.
CONST GM_MIN_LOOP_TIME_MS! = 14.0

'Read-Only variables
'-------------------
DIM gm_gameOver%=0 'Game Over Status
DIM gm_frameCount%=0 'Current frame counter
DIM gm_isStarted% = 0 'Flag indicating that the GameManager is active.

'Read-Write variables
'--------------------
DIM gm_displayLoopTime% = 0 'Set to one to display game loop time in upper left corner of screen.

'Public subs and functions
'-------------------------

'Start all GameManager services.
SUB GM_startUp  
  EVT_MGR_startUp
  LM_startUp
  WM_startUp
  DM_startUp
  IM_startUp
  Anim_startUp
  
  'Set world and view size to DM view dimensions
  box_width%(wm_boundary%) = MM.HRES<<4
  box_height%(wm_boundary%) = MM.VRES<<4
  WM_setViewDim(MM.HRES<<4, MM.VRES<<4)
  dummy_i% = WM_setViewPos%(0, 0)
      
  'Initialize the palette, sheet module and Background modules  
  PAL_init
  Sheet_startUp
  VObj_startUp
  BG_init
  TE_init 'Must come after Sheet_init.

  gm_isStarted% = 1
END SUB

'Shut down GameManager services.
SUB GM_shutDown
  GM_setGameOver(1)
  Sound_stop
  Anim_shutDown
  IM_shutDown
  DM_shutDown
  WM_shutDown
  VObj_shutDown
  Sheet_shutDown
  LM_shutDown
  EVT_MGR_shutDown
  gm_isStarted% = 0
END SUB

'Run game loop
SUB GM_run      
  DO WHILE NOT gm_gameOver%
    DM_Vsync 'Sync to screen refresh.
    
    TIMER=0: _gm_loopTimeMs!=0
    INC gm_frameCount%

    DM_refreshPages
    
    _gm_swapBufferTimeMs! = TIMER: INC _gm_loopTimeMs!, _gm_swapBufferTimeMs!: TIMER=0
    
    'Send a step event to all registered objects.    
    dummy_i% = EVT_MGR_onEvent%(EVT_STEP_create%(gm_frameCount%))

    _gm_onEventTimeMs! = TIMER: INC _gm_loopTimeMs!, _gm_onEventTimeMs!: TIMER=0

    IF (gm_frameCount% AND 1) THEN
      IM_getInput
    ENDIF

    _gm_inputTimeMs!=TIMER: INC _gm_loopTimeMs!, _gm_inputTimeMs!: TIMER=0
      
    WM_update

    _gm_updateTimeMs!=TIMER: INC _gm_loopTimeMs!, _gm_updateTimeMs!: TIMER=0
    
    IF bg_enabled% THEN: BG_update: ENDIF

    _gm_bgUpdateTimeMs!=TIMER: INC _gm_loopTimeMs!, _gm_bgUpdateTimeMs!: TIMER=0
    
    WM_draw

    _gm_drawTimeMs!=TIMER: INC _gm_loopTimeMs!, _gm_drawTimeMs!: TIMER=0
            
    IF gm_displayLoopTime% THEN: TEXT 0,0, STR$(_gm_loopTimeMs!)+" ms",,,,MAP(pal_notBlackIdx%): ENDIF
    IF _gm_loopTimeMs! >= GM_FRAME_TIME_MS! THEN
      LM_writeLog("GM: deadline missed. looptime: "+STR$(_gm_loopTimeMs!)+" frame: "+STR$(gm_frameCount%))
      LM_writeLog("SB:"+STR$(_gm_swapBufferTimeMs!)+" EV:"+STR$(_gm_onEventTimeMs!))
      LM_writeLog("IN:"+STR$(_gm_inputTimeMs!)+" UP:"+STR$(_gm_updateTimeMs!)+" BG:"+STR$(_gm_bgUpdateTimeMs!))
      LM_writeLog("DR:"+STR$(_gm_drawTimeMs!))
    ENDIF
    
    DO WHILE (_gm_loopTimeMs!+TIMER) < GM_MIN_LOOP_TIME_MS!: LOOP
      
  LOOP
END SUB

'Set Game Over status to indicated value. True stops game loop
SUB GM_setGameOver(gameOverVal%)
  gm_gameOver% = gameOverVal%
END SUB

