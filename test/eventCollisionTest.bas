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

GM_startUp

DIM obj1% = Obj_create%()
DIM obj2% = Obj_create%()
DIM vx% = 3*16
DIM vy% = 4*16

DIM ev% = EVT_COL_create%(obj1%, obj2%, vx%, vy%)

IF EVT_getType%(ev%) <> EVT_COL% THEN
  Error "Event Type incorrect".
ENDIF

IF EVT_COL_getObj1%(ev%) <> obj1% THEN
  Error "EvtCol obj1 incorrect."
ENDIF

IF EVT_COL_getObj2%(ev%) <> obj2% THEN
  Error "EvtCol obj2 incorrect."
ENDIF

IF EVT_COL_getPosX%(ev%) <> vx% THEN
  Error "EvtCol position incorrect."
ENDIF

IF EVT_COL_getPosY%(ev%) <> vy% THEN
  Error "EvtCol position incorrect."
ENDIF

DIM obj3% = Obj_create%()
DIM obj4% = Obj_create%()
DIM v2x% = 5*16
DIM v2y% = 6*16

EVT_COL_setObj1(ev%, obj3%)
EVT_COL_setObj2(ev%, obj4%)
EVT_COL_setPos(ev%, v2x%, v2y%)

IF EVT_getType%(ev%) <> EVT_COL% THEN
  Error "Event Type incorrect".
ENDIF

IF EVT_COL_getObj1%(ev%) <> obj3% THEN
  Error "EvtCol obj1 incorrect."
ENDIF

IF EVT_COL_getObj2%(ev%) <> obj4% THEN
  Error "EvtCol obj2 incorrect."
ENDIF

IF EVT_COL_getPosX%(ev%) <> v2x% THEN
  Error "EvtCol position incorrect."
ENDIF

IF EVT_COL_getPosY%(ev%) <> v2y% THEN
  Error "EvtCol position incorrect."
ENDIF

IF NOT EVT_COL_isAllocated%(ev%) THEN
  Error "EvtCol allocation error"
ENDIF

DIM xx%, yy%

EVT_COL_getPos(ev%, xx%, yy%)
IF xx% <> v2x% THEN
  Error "EvtCol position incorrect."
ENDIF

IF yy% <> v2y% THEN
  Error "EvtCol position incorrect."
ENDIF

EVT_destroy(ev%)
IF EVT_COL_isAllocated%(ev%) THEN
  Error "EvtCol destroy error"
ENDIF

GM_shutdown

PRINT "Test Completed."

END

