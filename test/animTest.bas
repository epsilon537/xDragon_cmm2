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

PAL_clr
PAL_load("../resources/xDragonDemo.pal")
PAL_allocStdColors
PAL_commitToCLUT

Sheet_load("../resources/spritesheet.BMP")

Anim_load("../resources/enemy.csv", "Enemy")
Anim_load("../resources/ship.csv", "Ship")

DIM aid% = Anim_find%("Enemy")
IF aid% = -1 THEN
  Error "Anim_find error."
ENDIF

DIM spr% = Anim_create%()

Anim_setWidth(spr%, 40*16)
Anim_setHeight(spr%, 20*16)
Anim_setLabel(spr%, "Test")
anim_slowdownLimit%(spr%) = 2


DIM frame% = Frame_create%(50*16, 50*16, 40*16, 20*16, Sheet_find%("spritesheet"))

Anim_addFrame(spr%, frame%)


frame% = Frame_create%(45*16, 45*16, 40*16, 20*16, Sheet_find%("spritesheet"))
Anim_addFrame(spr%, frame%)

IF Anim_getWidth%(spr%) <> 40*16 THEN
  Error "Anim width incorrect"
ENDIF

IF Anim_getHeight%(spr%) <> 20*16 THEN
  Error "Anim height incorrect"
ENDIF

IF Anim_getLabel$(spr%) <> "Test" THEN
  Error "Anim label incorrect"
ENDIF

IF anim_slowdownLimit%(spr%) <> 2 THEN
  Error "Anim slowdown incorrect"
ENDIF

IF Anim_getNumFrames%(spr%) <> 2 THEN
  Error "Anim framecount incorrect"
ENDIF

DIM frame2% = Anim_getFrame%(spr%, 1)
IF frame2% <> frame% THEN
  Error "Anim getFrame error."
ENDIF

frame% = Frame_create%(40*16, 40*16, 40*16, 20*16, Sheet_find%("spritesheet"))

Anim_addFrame(spr%, frame%)

DIM ii%=0, jj%=0, qq%=0

DO
  DM_Vsync 'Sync to screen refresh.  
  DM_refreshPages

  Anim_drawFrame(aid%, jj%, MM.HRES*16/2+100*16, MM.VRES*16/2)
  Anim_drawFrame(spr%, ii%, MM.HRES*16/2, MM.VRES*16/2)

  IF (qq% MOD 50) = 0 THEN  
    INC ii%
    INC jj%
  ENDIF
  
  INC qq%
  
  IF ii% >= Anim_getNumFrames%(spr%) THEN
    ii%=0
  ENDIF

  IF jj% >= Anim_getNumFrames%(aid%) THEN
    jj%=0
  ENDIF
LOOP

END


