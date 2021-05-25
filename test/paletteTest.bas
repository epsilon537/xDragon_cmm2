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

PAL_clr

DIM ii%
FOR ii%=0 TO PAL_NUM_ENTRIES%-1
  IF PAL_entryIsAllocated%(ii%) THEN
    ERROR "PAL_clr error"
  ENDIF
NEXT ii%

PAL_setRefToDef
FOR ii%=0 TO PAL_NUM_ENTRIES%-1
  IF NOT PAL_entryIsAllocated%(ii%) THEN
    ERROR "PAL_setRefToDefault error"
  ENDIF
NEXT ii%

PAL_commitToCLUT

LM_writeLog("white idx = "+STR$(pal_whiteIdx%))

IF PAL_getDefRGB%(pal_whiteIdx%) <> (RGB(white) AND &HE0E0C0) THEN
  ERROR "White error: "+HEX$(PAL_getDefRGB%(pal_whiteIdx%))+" vs "+HEX$(RGB(white))
ENDIF

IF PAL_getDefRGB%(pal_notBlackIdx%) <> (RGB(black) AND &HE0E0C0) THEN
  ERROR "not black error."
ENDIF

IF PAL_getDefRGB%(pal_blueIdx%) <> (RGB(blue) AND &HE0E0C0) THEN
  ERROR "Blue error."
ENDIF

IF PAL_getDefRGB%(pal_greenIdx%) <> (RGB(green) AND &HE0E0C0) THEN
  ERROR "Green error."
ENDIF

IF PAL_getDefRGB%(pal_cyanIdx%) <> (RGB(cyan) AND &HE0E0C0) THEN
  ERROR "Cyan error."
ENDIF

IF PAL_getDefRGB%(pal_redIdx%) <> (RGB(red) AND &HE0E0C0) THEN
  ERROR "Red error."
ENDIF

IF PAL_getDefRGB%(pal_magentaIdx%) <> (RGB(magenta) AND &HE0E0C0) THEN
  ERROR "Magenta error."
ENDIF

IF PAL_getDefRGB%(pal_yellowIdx%) <> (RGB(yellow) AND &HE0E0C0) THEN
  ERROR "yellow error."
ENDIF

IF MAP(pal_brownIdx%) <> (RGB(brown) AND &HE0E0C0) THEN
  ERROR "Brown error."
ENDIF

IF MAP(pal_greyIdx%) <> (RGB(grey) AND &HE0E0C0) THEN
  ERROR "Grey error."
ENDIF

TEXT 0, 0, "WHITE",,,,MAP(pal_whiteIdx%)
TEXT 0, MM.INFO(FONTHEIGHT), "NOT BLACK",,,,MAP(pal_notBlackIdx%)
TEXT 0, 2*MM.INFO(FONTHEIGHT), "BLUE",,,,MAP(pal_blueIdx%)
TEXT 0, 3*MM.INFO(FONTHEIGHT), "GREEN",,,,MAP(pal_greenIdx%)
TEXT 0, 4*MM.INFO(FONTHEIGHT), "CYAN",,,,MAP(pal_cyanIdx%)
TEXT 0, 5*MM.INFO(FONTHEIGHT), "RED",,,,MAP(pal_redIdx%)
TEXT 0, 6*MM.INFO(FONTHEIGHT), "MAGENTA",,,,MAP(pal_magentaIdx%)
TEXT 0, 7*MM.INFO(FONTHEIGHT), "YELLOW",,,,MAP(pal_yellowIdx%)
TEXT 0, 8*MM.INFO(FONTHEIGHT), "BROWN",,,,MAP(pal_brownIdx%)
TEXT 0, 9*MM.INFO(FONTHEIGHT), "GREY",,,,MAP(pal_greyIdx%)

DM_refreshPages

DO WHILE INKEY$="": LOOP

PAL_clr

FOR ii%=0 TO PAL_NUM_ENTRIES%-1
  IF PAL_entryIsAllocated%(ii%) THEN
    ERROR "PAL_clr error"
  ENDIF
NEXT ii%

PAL_load("../resources/xDragonDemo.pal")

IF NOT PAL_entryIsAllocated%(20) THEN
  ERROR "PAL_load error"
ENDIF

IF PAL_entryIsAllocated%(240) THEN
  ERROR "PAL_load error"
ENDIF

PAL_allocStdColors

PAL_commitToCLUT

IF PAL_getRefRGB%(pal_whiteIdx%) <> (RGB(white) AND &HFFFFFF) THEN
  ERROR "White error: "+HEX$(PAL_getRefRGB%(pal_whiteIdx%))
ENDIF

IF PAL_getRefRGB%(pal_notBlackIdx%) <> (RGB(black) AND &HFFFFFF) THEN
  ERROR "Black error."
ENDIF

IF PAL_getRefRGB%(pal_blueIdx%) <> (RGB(blue) AND &HFFFFFF) THEN
  ERROR "Blue error."
ENDIF

IF PAL_getRefRGB%(pal_greenIdx%) <> (RGB(green) AND &HFFFFFF) THEN
  ERROR "Green error."
ENDIF

IF PAL_getRefRGB%(pal_cyanIdx%) <> (RGB(cyan) AND &HFFFFFF) THEN
  ERROR "Cyan error."
ENDIF

IF PAL_getRefRGB%(pal_redIdx%) <> (RGB(red) AND &HFFFFFF) THEN
  ERROR "Red error."
ENDIF

IF PAL_getRefRGB%(pal_magentaIdx%) <> (RGB(magenta) AND &HFFFFFF) THEN
  ERROR "Magenta error."
ENDIF

IF PAL_getRefRGB%(pal_yellowIdx%) <> (RGB(yellow) AND &HFFFFFF) THEN
  ERROR "Yellow error."
ENDIF

IF PAL_getRefRGB%(pal_brownIdx%) <> (RGB(brown) AND &HFFFFFF) THEN
  ERROR "Brown error."
ENDIF

IF PAL_getRefRGB%(pal_greyIdx%) <> (RGB(grey) AND &HFFFFFF) THEN
  ERROR "Grey error."
ENDIF

DIM aid1% = Anim_find%("Enemy")
IF aid1% = -1 THEN
  Error "Anim_find error."
ENDIF

TEXT 0, 0, "WHITE",,,,MAP(pal_whiteIdx%)
TEXT 0, MM.INFO(FONTHEIGHT), "NOT BLACK",,,,MAP(pal_notBlackIdx%)
TEXT 0, 2*MM.INFO(FONTHEIGHT), "BLUE",,,,MAP(pal_blueIdx%)
TEXT 0, 3*MM.INFO(FONTHEIGHT), "GREEN",,,,MAP(pal_greenIdx%)
TEXT 0, 4*MM.INFO(FONTHEIGHT), "CYAN",,,,MAP(pal_cyanIdx%)
TEXT 0, 5*MM.INFO(FONTHEIGHT), "RED",,,,MAP(pal_redIdx%)
TEXT 0, 6*MM.INFO(FONTHEIGHT), "MAGENTA",,,,MAP(pal_magentaIdx%)
TEXT 0, 7*MM.INFO(FONTHEIGHT), "YELLOW",,,,MAP(pal_yellowIdx%)
TEXT 0, 8*MM.INFO(FONTHEIGHT), "BROWN",,,,MAP(pal_brownIdx%)
TEXT 0, 9*MM.INFO(FONTHEIGHT), "GREY",,,,MAP(pal_greyIdx%)

DM_swapBuffers

DO WHILE INKEY$="": LOOP

DIM aid2% = Anim_find%("Ship")
IF aid2% = -1 THEN
  Error "Anim_find error."
ENDIF

DIM palEntry% = PAL_allocateEntry%()
IF NOT PAL_entryIsAllocated%(palEntry%) THEN
  ERROR "PAL_entryIsAllocated error"
ENDIF

PAL_setColor(palEntry%, RGB(1,2,3))

PAL_freeEntry(palEntry%)
IF PAL_entryIsAllocated%(palEntry%) THEN
  ERROR "PAL_freeEntry error"
ENDIF

PAL_allocBlock(200, 220)
IF NOT PAL_entryIsAllocated%(210) THEN
  ERROR "PAL_allocBlock error"
ENDIF

PAL_freeBlock(200, 220)
IF PAL_entryIsAllocated%(210) THEN
  ERROR "PAL_freeBlock error"
ENDIF

DIM jj%=0

DIM target_pal%(PAL_NUM_ENTRIES%-1)
DIM target_pal2%(PAL_NUM_ENTRIES%-1)

FOR ii%=0 TO PAL_NUM_ENTRIES%-1
  target_pal%(ii%)=0
NEXT ii%

FOR ii%=0 TO PAL_NUM_ENTRIES%-1
  target_pal2%(ii%)=RGB(255,255,255)
NEXT ii%

PAL_startCLUTtransition(target_pal%(), 20, 0)

ii%=0

DIM phaseCounter%=0

Anim_drawFrame(aid1%, ii%, (MM.HRES\2+100)*16, MM.VRES*16\2)
Anim_drawFrame(aid2%, jj%, (MM.HRES\2)*16, MM.VRES*16\2))
DM_swapBuffers

DO WHILE INKEY$="": LOOP

DO WHILE INKEY$=""
  DM_Vsync 'Sync to screen refresh.
  'DM_swapBuffers
    
  PAL_CLUTtransitionStep
  
  IF PAL_CLUTtransitionIsDone%() THEN
    SELECT CASE phaseCounter%
      CASE 0
        PAL_startCLUTtransition(target_pal%(), 20, 1)
        INC phaseCounter%
      CASE 1
        PAL_startCLUTtransition(target_pal2%(), 20, 0)
        INC phaseCounter%
      CASE 2
        PAL_startCLUTtransition(target_pal2%(), 20, 1)
        INC phaseCounter%
      CASE 3
        PAL_startCLUTtransition(target_pal%(), 20, 0)
        phaseCounter% = 0
    END SELECT
  ENDIF

'  Anim_drawFrame(aid1%, ii%, (MM.HRES\2+100)*16, MM.VRES*16\2)
'  Anim_drawFrame(aid2%, jj%, (MM.HRES\2)*16, MM.VRES*16\2))
  
'  INC ii%
'  INC jj%
    
'  IF ii% >= Anim_getNumFrames%(aid1%) THEN
'    ii%=0
'  ENDIF

'  IF jj% >= Anim_getNumFrames%(aid2%) THEN
'    jj%=0
'  ENDIF
  
'  PAUSE 100
LOOP

PAL_setCLUTtoColor(RGB(80,80,80))
DO WHILE INKEY$="": LOOP

PAL_commitToCLUT
DO WHILE INKEY$="": LOOP
 
END


