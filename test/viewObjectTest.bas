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

box_width%(wm_boundary%) = box_width%(wm_boundary%)*2
box_height%(wm_boundary%) = box_height%(wm_boundary%)*2

DIM viewPosX%, viewPosY%
WM_getViewPos(viewPosX%, viewPosY%)

dummy_i% = WM_setViewPos%(viewPosX%+16*MM.HRES/2, viewPosY%+16*MM.VRES/2)

DIM vo1% = VObj_create%()
PRINT vo1%
VObj_setViewString(vo1%, "Hello World")
VObj_commit(vo1%)

DIM vo2% = VObj_create%()
VObj_setViewString(vo2%, "Hello World")
VObj_setLocation(vo2%, VOBJ_LOC_CENTER_CENTER%)
VObj_setBorder(vo2%, 0)
VObj_commit(vo2%)

DIM vo3% = VObj_create%()
VObj_setViewString(vo3%, "Hello World")
VObj_setLocation(vo3%, VOBJ_LOC_BOTTOM_LEFT%)
VObj_setBorder(vo3%, 1)
VObj_commit(vo3%)

DIM vo4% = VObj_create%()
VObj_setViewString(vo4%, "Hello World")
VObj_setLocation(vo4%, VOBJ_LOC_BOTTOM_RIGHT%)
VObj_setBorder(vo4%, 1)
VObj_setColor(vo4%, RGB(yellow))
VObj_commit(vo4%)

DIM vo5% = VObj_create%()
VObj_setViewString(vo5%, "Hello World")
VObj_setLocation(vo5%, VOBJ_LOC_TOP_LEFT%)
VObj_setBorder(vo5%, 1)
VObj_setColor(vo5%, RGB(yellow))
VObj_setDrawValue(vo5%, 0)
VObj_commit(vo5%)

DIM vo6% = VObj_create%()
PRINT vo6%
VObj_setViewString(vo6%, "Hello there!")
VObj_setLocation(vo6%, VOBJ_LOC_CENTER_RIGHT%)
VObj_setBorder(vo6%, 1)
VObj_setColor(vo6%, RGB(yellow))
VObj_setDrawValue(vo6%, 1)
VObj_setValue(vo6%, 12345678)
VObj_commit(vo6%)

IF VObj_getViewString$(vo6%) <> "Hello there!") THEN
  ERROR "VOBJ_getViewString error."
ENDIF

IF VObj_getLocation%(vo6%) <> VOBJ_LOC_CENTER_RIGHT%) THEN
  ERROR "VOBJ_getLocation error."
ENDIF

IF VObj_getBorder%(vo6%) <> 1) THEN
  ERROR "VObj_getBorder error."
ENDIF

IF VObj_getColor%(vo6%) <> RGB(yellow) THEN
  ERROR "VOBJ_getColor error."
ENDIF

IF VObj_getDrawValue%(vo6%) <> 1 THEN
  ERROR "VOBJ_getDrawValue error."
ENDIF

IF VObj_getValue%(vo6%) <> 12345678 THEN
  ERROR "VOBJ_getValue error."
ENDIF

GM_run

GM_shutDown

END

