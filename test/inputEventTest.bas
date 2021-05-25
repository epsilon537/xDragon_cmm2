OPTION EXPLICIT
OPTION DEFAULT NONE
OPTION BASE 0
OPTION CONSOLE BOTH

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

DIM ev% = EVT_KBD_create%()

IF EVT_getType%(ev%) <> EVT_KBD% THEN
  Error "Event Type incorrect."
ENDIF

DIM ii%

FOR ii%=0 to 10
  EVT_KBD_setKey(ev%, ii%)
  IF EVT_KBD_getKey%(ev%) <> ii% THEN
    Error "KBD_getKey error."
  ENDIF
NEXT ii%

EVT_KBD_setAction(ev%, EVT_KBD_ACT_KEY_PRESSED%)
IF EVT_KBD_getAction%(ev%) <> EVT_KBD_ACT_KEY_PRESSED% THEN
  Error "KBD_getAction error."
ENDIF

EVT_KBD_setAction(ev%, EVT_KBD_ACT_KEY_RELEASED%)
IF EVT_KBD_getAction%(ev%) <> EVT_KBD_ACT_KEY_RELEASED% THEN
  Error "KBD_getAction error."
ENDIF

EVT_KBD_setAction(ev%, EVT_KBD_ACT_KEY_DOWN%)
IF EVT_KBD_getAction%(ev%) <> EVT_KBD_ACT_KEY_DOWN% THEN
  Error "KBD_getAction error."
ENDIF

PRINT "Test Complete."

END

