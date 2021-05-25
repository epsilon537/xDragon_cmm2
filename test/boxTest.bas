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

DIM box1% = Box_create%(0, 0, 0, 0)
DIM c1x% = 3
DIM c1y% = 2

box_cornerX%(box1%) = c1x%
box_cornerY%(box1%) = c1y%
box_width%(box1%) = 100
box_height%(box1%) = 200

DIM c2x% = 10
DIM c2y% = 20
DIM box2% = Box_create%(c2x%, c2y%, 1000, 2000)

IF box_cornerX%(box2%) <> c2x% THEN
  ERROR "Box_getCorner error."
ENDIF

IF box_cornerY%(box2%) <> c2y% THEN
  ERROR "Box_getCorner error."
ENDIF

IF box_width%(box2%) <> 1000 THEN
  ERROR "Box_getHorizontal error"
ENDIF

IF box_height%(box2%) <> 2000 THEN
  ERROR "Box_getVertical error"
ENDIF

Box_copy(box2%, box1%)
IF NOT Box_eq%(box2%, box1%) THEN
  ERROR "Box_copy error"
ENDIF

box_cornerX%(box2%) = -1
box_cornerY%(box2%) = -1
box_width%(box2%) = 2
box_height%(box2%) = 2
Box_scale(box2%, 2)

box_cornerX%(box1%) = -2
box_cornerY%(box1%) = -2
box_width%(box1%) = 4
box_height%(box1%) = 4
IF NOT Box_eq%(box2%, box1%) THEN
  ERROR "Box_scale error"
ENDIF

Box_destroy(box1%)
Box_destroy(box2%)

PRINT "Num boxes allocated = "+STR$(box_numAllocated%)

PRINT "Test Complete."

END

