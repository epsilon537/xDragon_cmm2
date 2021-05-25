'Public constants
'----------------
CONST PAL_NUM_ENTRIES% = 256 'Number of entries in the color palette.

'Read-Only variables
'-------------------
'The indices in the palette of the standard colors. These get set after calling PAL_allocStdColors.
DIM pal_whiteIdx%, pal_notBlackIdx%, pal_blueIdx%, pal_greenIdx%, pal_cyanIdx%, pal_redIdx%, pal_magentaIdx%, pal_yellowIdx%, pal_brownIdx%, pal_greyIdx%

'The reference palette. This is usually the palette in use, but in case of palette transitions such as fading,
'this is the reference palette from which the transition starts or to which the transition is going.
'-1 entries are free.
DIM pal_refR%(PAL_NUM_ENTRIES%-1)
DIM pal_refG%(PAL_NUM_ENTRIES%-1)
DIM pal_refB%(PAL_NUM_ENTRIES%-1)

'The default palette
DIM pal_defR%(PAL_NUM_ENTRIES%-1)
DIM pal_defG%(PAL_NUM_ENTRIES%-1)
DIM pal_defB%(PAL_NUM_ENTRIES%-1)

'Private variables and constants
'-------------------------------
CONST _PAL_TRANS_STEP% = 85
'The palette data used in transitions.
DIM _pal_deltaR%(PAL_NUM_ENTRIES%-1) 'Colors split out because deltas can be negative.
DIM _pal_deltaG%(PAL_NUM_ENTRIES%-1)
DIM _pal_deltaB%(PAL_NUM_ENTRIES%-1)

'Variables used in transitions
DIM _pal_transStep!=0, _pal_transSlider!=0
DIM _pal_transStartIdx%

'Public subs and functions
'-------------------------

'Initialise the Palette module. Call this first.
SUB PAL_init
  MAP RESET

  LOCAL ii%
  FOR ii%=0 TO PAL_NUM_ENTRIES%-1
    pal_defR%(ii%) = (MAP(ii%)>>16) AND 255
    pal_defG%(ii%) = (MAP(ii%)>>8) AND 255
    pal_defB%(ii%) = MAP(ii%) AND 255    
  NEXT ii%
  
  PAL_clr
END SUB

'Clear the reference palette, free all entries.
SUB PAL_clr
  MEMORY SET INTEGER PEEK(VARADDR pal_refR%()), -1, PAL_NUM_ENTRIES%
  MEMORY SET INTEGER PEEK(VARADDR pal_refG%()), 0, PAL_NUM_ENTRIES%
  MEMORY SET INTEGER PEEK(VARADDR pal_refB%()), 0, PAL_NUM_ENTRIES%
END SUB

'Set the reference palette to the default palette. All colors are allocated.
SUB PAL_setRefToDef
  MEMORY COPY INTEGER PEEK(VARADDR pal_defR%()), PEEK(VARADDR pal_refR%()), PAL_NUM_ENTRIES%
  MEMORY COPY INTEGER PEEK(VARADDR pal_defG%()), PEEK(VARADDR pal_refG%()), PAL_NUM_ENTRIES%
  MEMORY COPY INTEGER PEEK(VARADDR pal_defB%()), PEEK(VARADDR pal_refB%()), PAL_NUM_ENTRIES%
  
  LOCAL ii%
  
  'Find the standard color indices    
  FOR ii%=0 TO PAL_NUM_ENTRIES%-1
    SELECT CASE (RGB(pal_refR%(ii%), pal_refG%(ii%), pal_refB%(ii%)) AND &HFFFFFF)
      CASE RGB(white) AND &H00E0E0C0
        pal_whiteIdx% = ii%
      CASE RGB(black) AND &H00E0E0C0
        IF ii% <> 0 THEN
         pal_notBlackIdx% = ii%
        ENDIF
      CASE RGB(blue) AND &H00E0E0C0
        pal_blueIdx% = ii%
      CASE RGB(green) AND &H00E0E0C0
        pal_greenIdx% = ii%
      CASE RGB(cyan) AND &H00E0E0C0
        pal_cyanIdx% = ii%
      CASE RGB(red) AND &H00E0E0C0
        pal_redIdx% = ii%
      CASE RGB(magenta) AND &H00E0E0C0
        pal_magentaIdx% = ii%
      CASE RGB(yellow) AND &H00E0E0C0
        pal_yellowIdx% = ii%
      CASE RGB(brown) AND &H00E0E0C0
        pal_brownIdx% = ii%
      CASE RGB(grey) AND &H00E0E0C0
        pal_greyIdx% = ii%        
    END SELECT
  NEXT ii%  
END SUB

'Returns non-zero if given color index in the reference palette is allocated
FUNCTION PAL_entryIsAllocated%(colorIdx%)
  PAL_entryIsAllocated% = (pal_refR%(colorIdx%) <> -1)
END FUNCTION

'Find a free entry in the reference palette and allocate it. Returns color index.
FUNCTION PAL_allocateEntry%()
  LOCAL ii%
  FOR ii%=0 TO PAL_NUM_ENTRIES%-1
    IF pal_refR%(ii%) = -1 THEN
      pal_refR%(ii%) = 0
      PAL_allocateEntry% = ii%
      EXIT FUNCTION
    ENDIF
  NEXT ii%
  
  ERROR "Can't alloc entry in ref. palette. All entries in use."
END FUNCTION

'Release the given color index in the reference palette.
SUB PAL_freeEntry(colorIdx%)
  IF pal_refR%(colorIdx%) = -1 THEN
    ERROR "Attempt to free ref. pal. entry that was not allocated."
  ENDIF
  
  pal_refR%(colorIdx%) = -1
END SUB

'Set RGB value of given color index in the reference palette. 
'Must only be used on allocated entries.
SUB PAL_setColor(colorIdx%, rgbVal%)
  IF pal_refR%(colorIdx%) = -1 THEN
    ERROR "Attempt to set color on entry that was not allocated."
  ENDIF
  
  pal_refR%(colorIdx%) = (rgbVal%>>16) AND 255
  pal_refG%(colorIdx%) = (rgbVal%>>8) AND 255
  pal_refB%(colorIdx%) = rgbVal% AND 255  
END SUB

'Allocate the standard colors in the reference palette. The pal_whiteIdx%... globals will be updated.
'Make sure to call this sub after setting up a custom palette, if you still want access to the standard colors.
SUB PAL_allocStdColors
  pal_whiteIdx% = PAL_allocateEntry%()
  pal_refR%(pal_whiteIdx%) = (RGB(white)>>16) AND 255
  pal_refG%(pal_whiteIdx%) = (RGB(white)>>8) AND 255
  pal_refB%(pal_whiteIdx%) = RGB(white) AND 255
  
  LM_writeLog("white_idx = "+STR$(pal_whiteIdx%))
  
  pal_notBlackIdx% = PAL_allocateEntry%()
  pal_refR%(pal_notBlackIdx%) = (RGB(black)>>16) AND 255
  pal_refG%(pal_notBlackIdx%) = (RGB(black)>>8) AND 255
  pal_refB%(pal_notBlackIdx%) = RGB(black) AND 255
  
  LM_writeLog("not_black_idx = "+STR$(pal_notBlackIdx%))

  pal_blueIdx% = PAL_allocateEntry%()
  pal_refR%(pal_blueIdx%) = (RGB(blue)>>16) AND 255
  pal_refG%(pal_blueIdx%) = (RGB(blue)>>8) AND 255
  pal_refB%(pal_blueIdx%) = RGB(blue) AND 255
  
  LM_writeLog("blue_idx = "+STR$(pal_blueIdx%))

  pal_greenIdx% = PAL_allocateEntry%()
  pal_refR%(pal_greenIdx%) = (RGB(green)>>16) AND 255
  pal_refG%(pal_greenIdx%) = (RGB(green)>>8) AND 255
  pal_refB%(pal_greenIdx%) = RGB(green) AND 255

  LM_writeLog("green_idx = "+STR$(pal_greenIdx%))

  pal_cyanIdx% = PAL_allocateEntry%()
  pal_refR%(pal_cyanIdx%) = (RGB(cyan)>>16) AND 255
  pal_refG%(pal_cyanIdx%) = (RGB(cyan)>>8) AND 255
  pal_refB%(pal_cyanIdx%) = RGB(cyan) AND 255
  
  LM_writeLog("cyan_idx = "+STR$(pal_cyanIdx%))

  pal_redIdx% = PAL_allocateEntry%()
  pal_refR%(pal_redIdx%) = (RGB(red)>>16) AND 255
  pal_refG%(pal_redIdx%) = (RGB(red)>>8) AND 255
  pal_refB%(pal_redIdx%) = RGB(red) AND 255
  
  LM_writeLog("red_idx = "+STR$(pal_redIdx%))

  pal_magentaIdx% = PAL_allocateEntry%()
  pal_refR%(pal_magentaIdx%) = (RGB(magenta)>>16) AND 255
  pal_refG%(pal_magentaIdx%) = (RGB(magenta)>>8) AND 255
  pal_refB%(pal_magentaIdx%) = RGB(magenta) AND 255
  
  LM_writeLog("magenta_idx = "+STR$(pal_magentaIdx%))

  pal_yellowIdx% = PAL_allocateEntry%()
  pal_refR%(pal_yellowIdx%) = (RGB(yellow)>>16) AND 255
  pal_refG%(pal_yellowIdx%) = (RGB(yellow)>>8) AND 255
  pal_refB%(pal_yellowIdx%) = RGB(yellow) AND 255
  
  LM_writeLog("yellow_idx = "+STR$(pal_yellowIdx%))

  pal_brownIdx% = PAL_allocateEntry%()
  pal_refR%(pal_brownIdx%) = (RGB(brown)>>16) AND 255
  pal_refG%(pal_brownIdx%) = (RGB(brown)>>8) AND 255
  pal_refB%(pal_brownIdx%) = RGB(brown) AND 255
  
  LM_writeLog("brown_idx = "+STR$(pal_brownIdx%))

  pal_greyIdx% = PAL_allocateEntry%()
  pal_refR%(pal_greyIdx%) = (RGB(grey)>>16) AND 255
  pal_refG%(pal_greyIdx%) = (RGB(grey)>>8) AND 255
  pal_refB%(pal_greyIdx%) = RGB(grey) AND 255
  
  LM_writeLog("grey_idx = "+STR$(pal_greyIdx%))  
END SUB

'Allocate a specific block of colored indices in the reference palette.
SUB PAL_allocBlock(startIdx%, endIdx%)
  LOCAL ii%
  FOR ii%=startIdx% TO endIdx%
    pal_refR%(ii%) = 0
  NEXT ii%
END SUB

'Free a specific block of colored indices in the reference palette.
SUB PAL_freeBlock(startIdx%, endIdx%)
  LOCAL ii%
  FOR ii%=startIdx% TO endIdx%
    pal_refR%(ii%) = -1
  NEXT ii%
END SUB

'Load a JASC palette file from disc into the reference palette.
SUB PAL_load(fname$)
  'This implementation is losely based on the LoadPalette() function of Nautilus' MAPSTER engine.
  
  LOCAL a$
  LOCAL maxPal%, ii%, rr%, gg%, bb%
  
  OPEN fname$ FOR INPUT AS #1
  LINE INPUT #1, a$
  IF a$ <> "JASC-PAL" THEN
    ERROR "Could not read palette file."
  ENDIF
 
  LINE INPUT #1, a$
  LINE INPUT #1, a$
  
  maxPal% = VAL(a$)
  
  PAL_allocBlock 0, maxPal%-1
  
  FOR ii% = 0 TO maxPal%-1
    IF NOT PAL_entryIsAllocated%(ii%) THEN
      ERROR "Attempt to set color at non-allocated entry: "+STR$(ii%)
    ENDIF
    
    LINE INPUT #1, a$
   'LM_writeLog("pal("+STR$(ii%)+")="+a$)
    rr% = VAL(FIELD$(a$, 1, " "))
    gg% = VAL(FIELD$(a$, 2, " "))
    bb% = VAL(FIELD$(a$, 3, " "))

    pal_refR%(ii%) = rr%
    pal_refG%(ii%) = gg%
    pal_refB%(ii%) = bb%    
  NEXT ii%
  CLOSE #1
END SUB

'Commit the reference palette to the CLUT
SUB PAL_commitToCLUT
  LOCAL ii%
  
  FOR ii%=0 TO PAL_NUM_ENTRIES%-1
    '-1 entries are set to 0.
    MAP(ii%) = CHOICE(pal_refR%(ii%) = -1, 0, PAL_getRefRGB%(ii%))
  NEXT ii%

  MAP SET
END SUB

'Start a palette transition to target palette. 
'Note that this only affects the CLUT, not the reference palette.
SUB PAL_startCLUTtransition(targetPal%(), numSteps%, startAtEnd%)

  IF startAtEnd% THEN
    _pal_transStep! = -1.0/numSteps%
    _pal_transSlider! = 1.0  
  ELSE
    _pal_transStep! = 1.0/numSteps%
    _pal_transSlider! = 0  
  ENDIF
  
  'Compute the delta between reference and target palette
  LOCAL ii%
  FOR ii%=0 TO PAL_NUM_ENTRIES%-1
    _pal_deltaR%(ii%) = ((targetPal%(ii%)>>16) AND 255) - pal_refR%(ii%)
    _pal_deltaG%(ii%) = ((targetPal%(ii%)>>8) AND 255) - pal_refG%(ii%)
    _pal_deltaB%(ii%) = (targetPal%(ii%) AND 255) - pal_refB%(ii%)
  NEXT ii%

  LOCAL rr%, gg%, bb%
  
  FOR ii%= 0 TO PAL_NUM_ENTRIES%-1
    rr% = pal_refR%(ii%) + _pal_deltaR%(ii%)*_pal_transSlider!
    gg% = pal_refG%(ii%) + _pal_deltaG%(ii%)*_pal_transSlider!
    bb% = pal_refB%(ii%) + _pal_deltaB%(ii%)*_pal_transSlider!
        
    MAP(ii%) = RGB(rr% AND &HFF,gg%,bb%)
  NEXT ii%
  
  _pal_transStartIdx% = 0
  dm_updateCLUT%=1
END SUB

'Next step in the transtion.
SUB PAL_CLUTtransitionStep  
  IF _pal_transStartIdx% >= PAL_NUM_ENTRIES%-1 THEN
    'Flag Display Manager that CLUT needs updating.  
    dm_updateCLUT%=1
    _pal_transStartIdx% = 0
    _pal_transSlider! = _pal_transSlider! + _pal_transStep!
    EXIT SUB
  ENDIF

  LOCAL slider! = MAX(MIN(_pal_transSlider!, 1),0)

  LOCAL ii%, rr%, gg%, bb%
  
  FOR ii%=_pal_transStartIdx% TO _pal_transStartIdx% + _PAL_TRANS_STEP%
    rr% = (pal_refR%(ii%) + _pal_deltaR%(ii%)*slider!)
    gg% = (pal_refG%(ii%) + _pal_deltaG%(ii%)*slider!)
    bb% = (pal_refB%(ii%) + _pal_deltaB%(ii%)*slider!)
        
    MAP(ii%) = RGB(rr% AND 255,gg%,bb%)
  NEXT ii%

  INC _pal_transStartIdx%, _PAL_TRANS_STEP%
END SUB

'Returns non-zero when transition is conplete.
FUNCTION PAL_CLUTtransitionIsDone%()  
  PAL_CLUTtransitionIsDone% = (_pal_transSlider! > 1.0) OR (_pal_transSlider! < 0.0)  
END FUNCTION

'Set all CLUT entries to given color. Reference palette is not affected.
SUB PAL_setCLUTtoColor(rgbVal%)
  LOCAL ii%
  FOR ii%=0 TO PAL_NUM_ENTRIES%-1
    MAP(ii%) = rgbVal%
  NEXT ii%
  
  MAP SET
END SUB

'Get RGB color value of given reference palette entry.
FUNCTION PAL_getRefRGB%(colorIdx%)
  PAL_getRefRGB% = RGB(pal_refR%(colorIdx%), pal_refG%(colorIdx%), pal_refB%(colorIdx%)) AND &HFFFFFF
END FUNCTION

'Get RGB color value of given default palette entry.
FUNCTION PAL_getDefRGB%(colorIdx%)
  PAL_getDefRGB% = RGB(pal_defR%(colorIdx%), pal_defG%(colorIdx%), pal_defB%(colorIdx%)) AND &HFFFFFF
END FUNCTION

