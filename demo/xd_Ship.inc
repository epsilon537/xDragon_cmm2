'Hero object flag bitfields
CONST SHIP_FLG_FIRE_SLOWDOWN_MSK% = 63<<12
CONST SHIP_FLG_FIRE_SLOWDOWN_SHFT% = 12
CONST SHIP_FLG_FIRE_COUNTDOWN_MSK% = 63<<6
CONST SHIP_FLG_FIRE_COUNTDOWN_SHFT% = 6
CONST SHIP_FLG_NUKE_COUNT_MSK% = 63
CONST SHIP_FLG_NUKE_COUNT_SHFT% = 0

'The animation object ids (aids) corresponding to the different ship animations
'N=neutral, N2U is neutral-to-up, etc.
DIM xd_shipNaid%, xd_shipN2Uaid%, xd_shipU2Naid%, xd_shipN2Daid%, xd_shipD2Naid%
DIM xd_shipFadeComplete%=0 'Flag set when fade has completed.

'xd Ship module level init function, called once, at start up.
SUB xd_Ship_init
  'The ship animation resource contains all the animations used. From this we will
  'create the various variants.
  LOCAL refAid% = Anim_find%("ship")

  'Create ship animation variants
      
  xd_shipNaid% = Anim_create%()
  Anim_copyProps refAid%, xd_shipNaid% 'Copy properties such as with and height from the reference animation.
  'Ship neutral animation is frames 4 and 9  
  Anim_addFrame(xd_shipNaid%, Anim_getFrame%(refAid%, 4))
  Anim_addFrame(xd_shipNaid%, Anim_getFrame%(refAid%, 9))

  xd_shipN2Uaid% = Anim_create%()
  Anim_copyProps refAid%, xd_shipN2Uaid%
  'Ship neutral-to-up animation is frames 0-2, looping at last frame
  Anim_addFrame(xd_shipN2Uaid%, Anim_getFrame%(refAid%, 0))
  Anim_addFrame(xd_shipN2Uaid%, Anim_getFrame%(refAid%, 1))
  Anim_addFrame(xd_shipN2Uaid%, Anim_getFrame%(refAid%, 2))
  anim_loopRewindIdx%(xd_shipN2Uaid%) = 2 'After animation reaches last frame, it loops back to frame #2 (the last frame in this case).

  xd_shipU2Naid% = Anim_create%()
  Anim_copyProps refAid%, xd_shipU2Naid%
  'Ship up-to-neutral animation is frames 3,4,9, looping at last two frames
  Anim_addFrame(xd_shipU2Naid%, Anim_getFrame%(refAid%, 3))
  Anim_addFrame(xd_shipU2Naid%, Anim_getFrame%(refAid%, 4))
  Anim_addFrame(xd_shipU2Naid%, Anim_getFrame%(refAid%, 9))
  anim_loopRewindIdx%(xd_shipU2Naid%) = 1

  xd_shipN2Daid% = Anim_create%()
  Anim_copyProps refAid%, xd_shipN2Daid%
  'Ship neutral-to-down animation is frames 5-7, looping at last frame
  Anim_addFrame(xd_shipN2Daid%, Anim_getFrame%(refAid%, 5))
  Anim_addFrame(xd_shipN2Daid%, Anim_getFrame%(refAid%, 6))
  Anim_addFrame(xd_shipN2Daid%, Anim_getFrame%(refAid%, 7))
  anim_loopRewindIdx%(xd_shipN2Daid%) = 2

  xd_shipD2Naid% = Anim_create%()
  Anim_copyProps refAid%, xd_shipD2Naid%
  'Ship down-to-neutral animation is frames 8,9,4, looping at last two frames
  Anim_addFrame(xd_shipD2Naid%, Anim_getFrame%(refAid%, 8))
  Anim_addFrame(xd_shipD2Naid%, Anim_getFrame%(refAid%, 9))
  Anim_addFrame(xd_shipD2Naid%, Anim_getFrame%(refAid%, 4))
  anim_loopRewindIdx%(xd_shipD2Naid%) = 1
END SUB

'Ship object constructor
FUNCTION xd_Ship_create%()
  LOCAL objId% = Obj_create%()
  
  'Link to sprite and start with neutral variant
  Obj_setAnim(objId%, "ship")
  Obj_setAnimVariant(objId%, xd_shipNaid%)
  
  Obj_setType(objId%, OBJ_TYPE_XD_SHIP%)

  Obj_setSolidness(objId%, OBJ_HARD%) 'Ship object can't move through other objects.
  Obj_setColGroup(objId%, CG_SHIP%) 'Ship belongs to ship collision group.

  'Starting position of the ship.
  obj_posX%(objId%) = 56*16
  obj_posY%(objId%) = MM.VRES*16/2
  
  'Set slowdown, fire, nuke attributes
  LOCAL xtra% = (15<<SHIP_FLG_FIRE_SLOWDOWN_SHFT%) OR (15<<SHIP_FLG_FIRE_COUNTDOWN_SHFT%) OR 1  
  Obj_setExtra(objId%, xtra%) 'This info goes into the base object 'extra' data field.

  Obj_registerInterest(objId%, EVT_KBD%) 'Ship wants to receive keyboard events.
  Obj_registerInterest(objId%, EVT_STEP%) 'Ship wants to receive step events.
  Obj_registerInterest(objId%, EVT_JOY%) 'Ship wants to receive joystick events.
  Obj_registerInterest(objId%, EVT_TE_TRIG%) 'We catch Tile Engine Triggers in this object

  obj_eventH$(objId%) = "xd_Ship_eventHandler%" 'Install event handler function for Ship object
  obj_destroySub$(objId%) = "xd_Ship_destroy" 'Install Ship object destructor sub.
  
  'Start fade-in, borrow palette from intro module
  PLAY VOLUME 100, 100
  PAL_startCLUTtransition(xd_Intro_fadePal%(), 16, 1)
  xd_shipFadeComplete%=0
  
  xd_Ship_create% = objId%
END FUNCTION

SUB xd_Ship_destroy(objId%)
  wm_viewVelX% = 0 'Stop scrolling
  bg_velX% = 0 'Background also should stop scrolling

  xd_Points_stopCounting  
  
  'Screen shake animation.
  dm_shakeSeverity%=10
  dm_shakeDuration%=20

  'Create an explosion
  dummy_i% = xd_Explosion_create%(objId%, 1) '1 here means: launch GameOverWhenDone
  xd_playSound XD_SOUND_NUKE%
  
  Obj_defaultDestroy(objId%) 'Also call base object destructor.
END SUB

'Ship event handler function
FUNCTION xd_Ship_eventHandler%(objId%, ev%)
  LOCAL eType% = EVT_getType%(ev%) 'Retrieve event type

  SELECT CASE eType%
    CASE EVT_COL% 'Collision event
      xd_Ship_hit objId%
      EXIT FUNCTION
    CASE EVT_KBD% 'Keyboard event
      xd_Ship_eventHandler% = 1
      xd_Ship_kbd(objId%, ev%)
      EXIT FUNCTION
    CASE EVT_JOY% 'Joystick event
      xd_Ship_eventHandler% = 1
      xd_Ship_joy(objId%, ev%)
      EXIT FUNCTION
    CASE EVT_STEP% 'Frame Step event.
      xd_Ship_step(objId%)
      xd_Ship_eventHandler% = 1
    CASE EVT_TE_TRIG%
      xd_Ship_TE_trig ev%      
      xd_Ship_eventHandler% = 1
  END SELECT
  
  xd_Ship_eventHandler% = 0
END FUNCTION

SUB xd_Ship_hit(objId%)
  WM_markForDelete(objId%) 'Mark ship for deletion
END SUB

'Tile Engine trigger -> spawn an enemy ship at trigger position.
SUB xd_Ship_TE_trig(ev%)
  LOCAL tileX%, tileY%, trigId%
  EVT_TE_TRIG_get(ev%, tileX%, tileY%, trigId%)
  dummy_i% = xd_Enemy_create%(tileX%*TE_TILE_SUBPIXEL_W%, tileY%*TE_TILE_SUBPIXEL_H%)
END SUB

'Joystick actions
SUB xd_Ship_joy(objId%, ev%)  
  SELECT CASE EVT_JOY_getAction%(ev%)
    CASE EVT_JOY_ACT_CLICKED%
      IF EVT_JOY_getButton%(ev%) = EVT_JOY_BUTTON_Z% THEN
        xd_Ship_fire(objId%)
      ELSE
        xd_Ship_nuke(objId%)
      ENDIF
    CASE EVT_JOY_ACT_MOVED%
      LOCAL xx%, yy%, objVelY% = obj_velY%(objId%)
      EVT_JOY_getPos(ev%, xx%, yy%)
      
      'Select animation variant depending on joystick movement
      IF (objVelY%<=0) AND (yy%>0) THEN
        Obj_setAnimVariant objId%, xd_shipN2Daid%
      ELSEIF (objVelY%>0) AND (yy%<=0) THEN 
        Obj_setAnimVariant objId%, xd_shipD2Naid%
      ELSE IF (objVelY%>=0) AND (yy%<0) THEN
        Obj_setAnimVariant objId%, xd_shipN2Uaid%
      ELSE IF (objVelY%<0) AND (yy%>=0) THEN
        Obj_setAnimVariant objId%, xd_shipU2Naid%
      ENDIF
      
      INC obj_velX%(objId%), xx%\2
      INC obj_velY%(objId%), yy%\2            
    CASE EVT_JOY_ACT_MOVE_END%
      'obj_velX%(objId%) = wm_viewVelX%
      'obj_velY%(objId%) = 0
  END SELECT
END SUB

SUB xd_Ship_kbd(objId%, ev%)
  LOCAL key% = EVT_KBD_getKey%(ev%) 

  SELECT CASE key%
    CASE Q_KEY% 'Quits the game and returns to intro.
      IF EVT_KBD_getAction%(ev%) = EVT_KBD_ACT_KEY_PRESSED% THEN
        WM_markForDelete(objId%) 'Same action as if ship got hit.
      ENDIF
    'Ship movement actions...
    CASE W_KEY%
      SELECT CASE EVT_KBD_getAction%(ev%)
        CASE EVT_KBD_ACT_KEY_PRESSED% 
          INC obj_velY%(objId%), -12
          Obj_setAnimVariant(objId%, xd_shipN2Uaid%)
        CASE EVT_KBD_ACT_KEY_RELEASED
          'obj_velY%(objId%) = 0
          Obj_setAnimVariant(objId%, xd_shipU2Naid%)
      END SELECT
    CASE A_KEY%
      SELECT CASE EVT_KBD_getAction%(ev%)
        CASE EVT_KBD_ACT_KEY_PRESSED% 
          INC obj_velX%(objId%), -12
        CASE EVT_KBD_ACT_KEY_RELEASED
          'obj_velX%(objId%) = wm_viewVelX%
      END SELECT    
    CASE S_KEY%
      SELECT CASE EVT_KBD_getAction%(ev%)
        CASE EVT_KBD_ACT_KEY_PRESSED% 
          INC obj_velY%(objId%), 12
          Obj_setAnimVariant(objId%, xd_shipN2Daid%)
        CASE EVT_KBD_ACT_KEY_RELEASED
          'obj_velY%(objId%) = 0
          Obj_setAnimVariant(objId%, xd_shipD2Naid%)
      END SELECT
    CASE D_KEY%
      SELECT CASE EVT_KBD_getAction%(ev%)
        CASE EVT_KBD_ACT_KEY_PRESSED% 
          INC obj_velX%(objId%), 12
        CASE EVT_KBD_ACT_KEY_RELEASED
          'obj_velX%(objId%) = wm_viewVelX%
      END SELECT
    'Nukes and bullets.        
    CASE SPC_KEY% 
      IF EVT_KBD_getAction%(ev%) = EVT_KBD_ACT_KEY_PRESSED% THEN
        xd_Ship_nuke(objId%)
      ENDIF  
    CASE ENTER_KEY%
      IF EVT_KBD_getAction%(ev%) = EVT_KBD_ACT_KEY_PRESSED% THEN
        xd_Ship_fire(objId%)
      ENDIF  
    CASE P_KEY%
      dm_saveImage%=1    
  END SELECT
END SUB

'Ship Step event handler
SUB xd_Ship_step(objId%)  
  IF NOT xd_shipFadeComplete% THEN
    IF NOT PAL_CLUTtransitionIsDone%() THEN 'Fade in...
      PAL_CLUTtransitionStep
    ELSE 'Fade complete -> Start scrolling
      obj_velX%(objId%) = 32
      wm_viewVelX% = 32
      bg_velX% = 16 'Background moves at half the speed to create parallax effect.
      xd_shipFadeComplete%=1
    ENDIF    
  ENDIF

  'Keep ship on screen (there has to be a better way of doing this...)
  IF obj_posX%(objId%) - box_cornerX%(wm_view%) < 0 THEN
    obj_posX%(objId%) = box_cornerX%(wm_view%) 'Clamp position
    obj_velX%(objId%) = wm_viewVelX% 'Match view velocity
  ELSE IF obj_posX%(objId%) - box_cornerX%(wm_view%) >= MM.HRES*16 THEN
    obj_posX%(objId%) = box_cornerX%(wm_view%) + (MM.HRES - 1)*16
    obj_velX%(objId%) = wm_viewVelX%
  ENDIF

  IF obj_posY%(objId%) - box_cornerY%(wm_view%) < 0 THEN
    obj_posY%(objId%) = box_cornerY%(wm_view%)
    obj_velY%(objId%) = wm_viewVelY%
  ELSE IF obj_posY%(objId%) - box_cornerY%(wm_view%) >= MM.VRES*16 THEN
    obj_posY%(objId%) = box_cornerY%(wm_view%) + (MM.VRES - 1)*16
    obj_velY%(objId%) = wm_viewVelY%
  ENDIF

  'Fire bullet rate limiting.
  LOCAL xtra% = Obj_getExtra%(objId%)    
  LOCAL fire_countdown%
  
  getBitField(xtra%, SHIP_FLG_FIRE_COUNTDOWN_MSK%, SHIP_FLG_FIRE_COUNTDOWN_SHFT%, fire_countdown%)
  IF fire_countdown% > 0 THEN: INC fire_countdown%, -1: ENDIF
  setBitField(xtra%, SHIP_FLG_FIRE_COUNTDOWN_MSK%, SHIP_FLG_FIRE_COUNTDOWN_SHFT%, fire_countdown%)
  
  Obj_setExtra(objId%, xtra%)
END SUB

SUB xd_Ship_nuke(objId%)         
  LOCAL xtra% = Obj_getExtra%(objId%)  
  LOCAL nuke_count%
  
  getBitField(xtra%, SHIP_FLG_NUKE_COUNT_MSK%, SHIP_FLG_NUKE_COUNT_SHFT%, nuke_count%)
  'Check if any nukes left
  IF nuke_count% = 0 THEN: EXIT SUB: ENDIF
  
  INC nuke_count%, -1

  'Create nuke event and send to interested Objects
  LOCAL ev% = EVT_create%()
  EVT_setType(ev%, EVT_XD_NUKE%) 'EVT_XD_NUKE is a used defined event
  dummy_i% = EVT_MGR_onEvent%(ev%)

  'Send view event with nukes to nuke ViewObject
  ev% = EVT_View_create%(VIEW_EVT_TAG_XD_NUKES%, -1, 1)
  dummy_i% = EVT_MGR_onEvent%(ev%)

  setBitField(xtra%, SHIP_FLG_NUKE_COUNT_MSK%, SHIP_FLG_NUKE_COUNT_SHFT%, nuke_count%)
    
  Obj_setExtra(objId%, xtra%)
  xd_playSound XD_SOUND_NUKE%
END SUB

SUB xd_Ship_fire(objId%)
  LOCAL xtra% = Obj_getExtra%(objId%)  
  LOCAL fire_countdown%
  
  'Fire bullet rate limiting
  getBitField(xtra%, SHIP_FLG_FIRE_COUNTDOWN_MSK%, SHIP_FLG_FIRE_COUNTDOWN_SHFT%, fire_countdown%)

  IF fire_countdown% > 0 THEN: EXIT SUB: ENDIF
  
  'Create a bullet object.
  dummy_i% = xd_Bullet_create%(obj_posX%(objId%), obj_posY%(objId%))    
  xd_playSound XD_SOUND_FIRE%
END SUB

