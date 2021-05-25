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

Sheet_load("../resources/spritesheet.BMP")

Anim_load("../resources/enemy.csv", "Enemy")
Anim_load("../resources/ship.csv", "Ship")

IF Anim_find%("Wrong") <> -1 THEN
  Error "Anim_find error."
ENDIF

DIM aid% = Anim_find%("Ship")
IF aid% = -1 THEN
  Error "Anim_find error."
ENDIF

DIM frid%
frid% = Anim_getFrame%(aid%, 1)

aid% = Anim_find%("Enemy")
IF aid% = -1 THEN
  Error "Anim_find error."
ENDIF

IF Anim_getWidth%(aid%) <> 30*16 THEN
  Error "Anim_getWidth error."
ENDIF

IF Anim_getHeight%(aid%) <> 26*16 THEN
  Error "Anim_getHeight error."
ENDIF

IF Anim_getLabel$(aid%) <> "Enemy" THEN
  Error "Anim_getLabel error."
ENDIF

IF anim_slowdownLimit%(aid%) <> 10 THEN
  Error "ANIM_getSlowdown error."
ENDIF

IF Anim_getNumFrames%(aid%) <> 2 THEN
  ERROR "Anim_getFrameCount error."
ENDIF

frid% = Anim_getFrame%(aid%, 0)

IF _frame_h%(frid%) <> 26*16 THEN
  ERROR "Frame_getHeight error: "+STR$(_frame_h%(frid%))
ENDIF

IF _frame_w%(frid%) <> 30*16 THEN
  ERROR "Frame_getWidth error"
ENDIF

frid% = Anim_getFrame%(aid%, 0)

IF _frame_h%(frid%) <> 26*16 THEN
  ERROR "Frame_getHeight error"
ENDIF

IF _frame_w%(frid%) <> 30*16 THEN
  ERROR "Frame_getWidth error"
ENDIF

Anim_unload("Enemy")
'RM_unloadSheet("spritesheet") 'This should fail.

Anim_unload("Ship")
Sheet_unload("spritesheet")

LM_writeLog("Test Complete.")
GM_shutDown()

END



