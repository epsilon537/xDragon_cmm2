'Read-Only variables
'-------------------
DIM IM_isStarted% = 0

'Private variables and constants
'-------------------------------
CONST _IM_LCTRL_MSK% = 2
CONST _IM_RCTRL_MSK% = 32
CONST _IM_LALT_MSK% = 1
CONST _IM_RALT_MSK% = 16
CONST _IM_LSHFT_MSK% = 8
CONST _IM_RSHFT_MSK% = 128

CONST _IM_NUNCHUK_CHANNEL% = 3

DIM _im_keyWasDown%(6)
DIM _im_keyIsDown%(6)
DIM _im_keyWasDownIdx%
DIM _im_keyIsDownIdx%

DIM _im_nunchuckOpen%=0
DIM _im_nunchuckJxl%=0
DIM _im_nunchuckJxc%=0
DIM _im_nunchuckJxr%=0
DIM _im_nunchuckJyt%=0
DIM _im_nunchuckJyc%=0
DIM _im_nunchuckJyb%=0
DIM _im_nunchukZbuttonWasPressed%=0
DIM _im_nunchukCbuttonWasPressed%=0
DIM _im_nunchukMoved%=0

'Public subs and functions
'-------------------------

'Get ready to capture input
SUB IM_startUp
  _im_keyWasDownIdx%=0
  _im_keyIsDownIdx%=0

  ON ERROR SKIP 1
  WII NUNCHUK OPEN
  
  IF MM.ERRNO=0 THEN 
    _im_nunchuckOpen%=1
  ELSE
    ON ERROR CLEAR
  ENDIF
  
  IF _im_nunchuckOpen% THEN
    LM_writeLog("IM: Nunchuk detected.")
    
    _im_nunchuckJxl% = NUNCHUK(JXL, 3)
    _im_nunchuckJxc% = NUNCHUK(JXC, 3)
    _im_nunchuckJxr% = NUNCHUK(JXR, 3)

    _im_nunchuckJyt% = NUNCHUK(JYT, 3)
    _im_nunchuckJyc% = NUNCHUK(JYC, 3)
    _im_nunchuckJyb% = NUNCHUK(JYB, 3)
    
    LM_writeLog("jxl="+STR$(_im_nunchuckJxl%)+" jxc="+STR$(_im_nunchuckJxc%)+" jxr=" +STR$(_im_nunchuckJxr%))
    LM_writeLog("jyt="+STR$(_im_nunchuckJyt%)+" jyc="+STR$(_im_nunchuckJyc%)+" jyb=" +STR$(_im_nunchuckJyb%))
  ENDIF
  
  IM_isStarted%=1
END SUB

'Shut down the Input Manager
SUB IM_shutDown
  IF _im_nunchuckOpen% THEN
    WII NUNCHUK CLOSE
    _im_nunchuckOpen%=0
  ENDIF
  
  IM_isStarted%=0
END SUB

'Get input and pass event to all Objects.
SUB IM_getInput
  _IM_keyboardInput
  
  IF _im_nunchuckOpen% THEN
    _IM_nunchukInput
  ENDIF
END SUB

'Private subs and functions
'--------------------------

SUB _IM_nunchukInput
  LOCAL ev% = EVT_JOY_create%()
  
  'Check is nunchuk moved
  LOCAL jx% = NUNCHUK(JX, _IM_NUNCHUK_CHANNEL%) - _im_nunchuckJxc%
  LOCAL jy% = NUNCHUK(JY, _IM_NUNCHUK_CHANNEL%) - _im_nunchuckJyc%
  LOCAL jx_norm!, jy_norm!
  
  IF jx% < 0 THEN
    jx_norm! = jx%/(_im_nunchuckJxc%-_im_nunchuckJxl%)
  ELSE
    jx_norm! = jx%/(_im_nunchuckJxr%-_im_nunchuckJxc%)
  ENDIF

  IF jy% < 0 THEN
    jy_norm! = jy%/(_im_nunchuckJyc%-_im_nunchuckJyb%)
  ELSE
    jy_norm! = jy%/(_im_nunchuckJyt%-_im_nunchuckJyc%)
  ENDIF  
  
  'LM_writeLog("jx_norm="+STR$(jx_norm!)+" jy_norm="+STR$(jy_norm!))
  
  IF (jx_norm!>0.1) OR (jx_norm!<-0.1) OR (jy_norm!>0.1) OR (jy_norm!<-0.1) THEN
    EVT_JOY_setAction ev%, EVT_JOY_ACT_MOVED%
    EVT_JOY_setPos ev%, CINT(jx_norm!*16), -CINT(jy_norm!*16)
    _im_nunchukMoved%=1
    dummy_i% = EVT_MGR_onEvent%(ev%)
  ELSE
    IF _im_nunchukMoved%=1 THEN 'Were we moving the joystick and we just stopped?
      EVT_JOY_setAction ev%, EVT_JOY_ACT_MOVE_END%
      _im_nunchukMoved%=0
      dummy_i% = EVT_MGR_onEvent%(ev%)
    ENDIF
  ENDIF
  
  'Check z button
  IF NUNCHUK(Z, _IM_NUNCHUK_CHANNEL%) THEN
    EVT_JOY_setAction(ev%, EVT_JOY_ACT_PRESSED%)
    EVT_JOY_setButton(ev%, EVT_JOY_BUTTON_Z%)
    dummy_i% = EVT_MGR_onEvent%(ev%)
    IF NOT _im_nunchukZbuttonWasPressed% THEN
      EVT_JOY_setAction(ev%, EVT_JOY_ACT_CLICKED%)    
      dummy_i% = EVT_MGR_onEvent%(ev%)
      _im_nunchukZbuttonWasPressed% = 1
    ENDIF
  ELSE
    _im_nunchukZbuttonWasPressed% = 0
  ENDIF

  'Check c button
  IF NUNCHUK(C, _IM_NUNCHUK_CHANNEL%) THEN
    EVT_JOY_setAction(ev%, EVT_JOY_ACT_PRESSED%)
    EVT_JOY_setButton(ev%, EVT_JOY_BUTTON_C%)
    dummy_i% = EVT_MGR_onEvent%(ev%)
    IF NOT _im_nunchukCbuttonWasPressed% THEN
      EVT_JOY_setAction(ev%, EVT_JOY_ACT_CLICKED%)    
      dummy_i% = EVT_MGR_onEvent%(ev%)
      _im_nunchukCbuttonWasPressed% = 1
    ENDIF
  ELSE
    _im_nunchukCbuttonWasPressed% = 0
  ENDIF  
END SUB

SUB _IM_keyboardInput
  LOCAL k% = 0
  LOCAL m% = 0
  LOCAL ii%=1, jj%, ev%
  LOCAL numKeysPressed%=KEYDOWN(0)
  
  ev% = EVT_KBD_create%()
  
  _im_keyIsDownIdx%=0

  DO WHILE ii% <= numKeysPressed%
    k% = KEYDOWN(ii%)
    IF k%<>0 THEN
      _im_keyIsDown%(_im_keyIsDownIdx%) = k%
      INC _im_keyIsDownIdx%
    ENDIF
    INC ii%
  LOOP

  m% = KEYDOWN(7)
    
  IF (m% AND _IM_LCTRL_MSK%) THEN
      _im_keyIsDown%(_im_keyIsDownIdx%) = LCTRL_KEY%
      INC _im_keyIsDownIdx% 
  ENDIF 

  IF (m% AND _IM_RCTRL_MSK%) THEN
      _im_keyIsDown%(_im_keyIsDownIdx%) = RCTRL_KEY%
      INC _im_keyIsDownIdx% 
  ENDIF 

  IF (m% AND _IM_LALT_MSK%) THEN
      _im_keyIsDown%(_im_keyIsDownIdx%) = LALT_KEY%
      INC _im_keyIsDownIdx% 
  ENDIF 

  IF (m% AND _IM_RALT_MSK%) THEN
      _im_keyIsDown%(_im_keyIsDownIdx%) = RALT_KEY%
      INC _im_keyIsDownIdx% 
  ENDIF 

  IF (m% AND _IM_LSHFT_MSK%) THEN
      _im_keyIsDown%(_im_keyIsDownIdx%) = LSHFT_KEY%
      INC _im_keyIsDownIdx% 
  ENDIF 

  IF (m% AND _IM_RSHFT_MSK%) THEN
      _im_keyIsDown%(_im_keyIsDownIdx%) = RSHFT_KEY%
      INC _im_keyIsDownIdx% 
  ENDIF 

  'PRINT "num keys pressed + modifiers = "+STR$(_im_keyIsDownIdx%)
  
  'Check for key release events
  ii%=0
  EVT_KBD_setAction ev%, EVT_KBD_ACT_KEY_RELEASED%
  
  DO WHILE ii%<_im_keyWasDownIdx%
    jj%=0
    DO WHILE jj%<_im_keyIsDownIdx%
      IF _im_keyWasDown%(ii%) = _im_keyIsDown%(jj%) THEN 'Still down
        GOTO key_not_just_released
      ENDIF
      INC jj%
    LOOP

    'No match so key was released
    EVT_KBD_setKey ev%, _im_keyWasDown%(ii%)
    dummy_i% = EVT_MGR_onEvent%(ev%)
    
key_not_just_released:
    INC ii%
  LOOP
  
  'Check for key press events
  ii%=0
  EVT_KBD_setAction ev%, EVT_KBD_ACT_KEY_PRESSED%
  
  DO WHILE ii%<_im_keyIsDownIdx%
    jj%=0
    DO WHILE jj%<_im_keyWasDownIdx%
      IF _im_keyIsDown%(ii%) = _im_keyWasDown%(jj%) THEN 'Still down
        GOTO key_not_just_pressed
      ENDIF
      INC jj%
    LOOP

    'No match so key was just pressed
    EVT_KBD_setKey ev%, _im_keyIsDown%(ii%)
    dummy_i% = EVT_MGR_onEvent%(ev%)

key_not_just_pressed:
    INC ii%
  LOOP

  'Generate key is down events
  ii%=0
  EVT_KBD_setAction ev%, EVT_KBD_ACT_KEY_DOWN%

  DO WHILE ii%<_im_keyIsDownIdx%
    EVT_KBD_setKey ev%, _im_keyIsDown%(ii%)
    dummy_i% = EVT_MGR_onEvent%(ev%)
    
    'Also copy key-is-down to key-was-down while we're at it.
    _im_keyWasDown%(ii%) = _im_keyIsDown%(ii%)
    INC ii%
  LOOP

  _im_keyWasDownIdx% = _im_keyIsDownIdx%
END SUB


