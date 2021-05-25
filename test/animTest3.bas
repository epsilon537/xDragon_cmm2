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


DIM enemy% = Anim_find%("Enemy")
DIM ship% = Anim_find%("Ship")

IF anim_slowdownLimit%(ship%) <> 10 THEN
  Error "ANIM_getSlowdown error"
ENDIF

IF anim_index%(enemy%) <> 0 THEN
  Error "anim index error"
ENDIF

anim_index%(ship%) = 1
anim_index%(enemy%) = 2
anim_slowdownCount%(enemy%) = -1

DIM vx% = 100*16
DIM vy% = 100*16

Anim_draw(ship%, vx%, vy%)
IF anim_slowdownCount%(ship%) <> 1 THEN
  Error "slowdown count error"
ENDIF
IF anim_index%(ship%) <> 1 THEN
  Error "Anim index error"
ENDIF

Anim_draw(ship%, vx%, vy%)
IF anim_slowdownCount%(ship%) <> 2 THEN
  Error "slowdown count error"
ENDIF

IF anim_index%(ship%) <> 1 THEN
  Error "index error"
ENDIF

Anim_draw(ship%, vx%, vy%)
Anim_draw(ship%, vx%, vy%)
Anim_draw(ship%, vx%, vy%)
Anim_draw(ship%, vx%, vy%)
Anim_draw(ship%, vx%, vy%)
Anim_draw(ship%, vx%, vy%)
Anim_draw(ship%, vx%, vy%)
Anim_draw(ship%, vx%, vy%)

IF anim_slowdownCount%(ship%) <> 0 THEN
  Error "Anim_getSlowdownCount error"
ENDIF
IF anim_index%(ship%) <> 2 THEN
  Error "index error"
ENDIF

Anim_unload("Enemy")
Anim_unload("Ship")

LM_writeLog("Test Complete.")

GM_shutdown

END

