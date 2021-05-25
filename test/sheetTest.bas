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

LM_writeLog("Max. num sheets = "+STR$(sheet_maxNum%))
LM_writeLog("Num sheets allocated = "+STR$(sheet_numAllocated%))

DIM initialNum% = sheet_numAllocated%

DIM page_id% = Sheet_create%()
LM_writeLog("Allocated page id: "+STR$(page_id%))
IF sheet_numAllocated% <> initialNum%+1 THEN
  ERROR "sheet_numAllocated error."
ENDIF

IF NOT Sheet_isAllocated%(page_id%) THEN
  ERROR "Sheet_isAllocated error."
ENDIF

IF (page_id%=0) OR (page_id%=1) THEN
  ERROR "Page id error."
ENDIF

Sheet_setLabel(page_id%, "sheet label")
IF Sheet_getLabel$(page_id%) <> "sheet label" THEN
  ERROR "Sheet label error."
ENDIF

DIM another_page_id% = Sheet_create%()

IF (another_page_id%=0) OR (another_page_id%=1) THEN
  ERROR "Page id error."
ENDIF

Sheet_setLabel(another_page_id%, "another sheet label")
IF Sheet_getLabel$(another_page_id%) <> "another sheet label" THEN
  ERROR "Sheet label error."
ENDIF

IF Sheet_find%("non-existing label") <> -1 THEN
  ERROR "Sheet find error."
ENDIF

IF Sheet_find%("sheet label") <> page_id% THEN
  ERROR "Sheet find error."
ENDIF

IF Sheet_find%("another sheet label") <> another_page_id% THEN
  ERROR "Sheet find error."
ENDIF

Sheet_destroy(another_page_id%)

Sheet_destroy(page_id%)

IF Sheet_isAllocated%(page_id%) THEN
  ERROR "Sheet_isAllocated error."
ENDIF

IF Sheet_isAllocated%(another_page_id%) THEN
  ERROR "Sheet_isAllocated error."
ENDIF

Sheet_load("../resources/spritesheet.BMP")

IF Sheet_find%("spritesheet") = -1 THEN
  ERROR "Sheet find error."
ENDIF

LM_writeLog("spritesheet page id = "+STR$(Sheet_find%("spritesheet")))

Sheet_unload("spritesheet")
IF Sheet_find%("spritesheet") <> -1 THEN
  ERROR "Sheet find error."
ENDIF

IF sheet_numAllocated% <> initialNum% THEN
  ERROR "sheet_numAllocated error."
ENDIF

LM_writeLog("Test Complete.")

GM_shutdown

END

