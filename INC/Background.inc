'Public constants
'----------------
CONST BG_PAGE_ID% = 3 'Background page id.
CONST BG_MAX_WIDTH% = (1600<<4) 'Max. width of bitmap image in .4 subpixel units.
CONST BG_MAX_HEIGHT% = (1200<<4) 'Max. height of bitmap image in .4 subpixel units.

'Read-Write variables
'--------------------
'Background layer horizontal and vertical scroll velocity. .4 subpixel units
DIM bg_velX%, bg_velY%
DIM bg_enabled% 'Enable the background layer

'Read-Only variables
'-------------------
'Current position of viewport within background image
DIM bg_posX%, bg_posY% '.4 subpixel units
DIM bg_width% 'Background image width. .4 subpixel units
DIM bg_height% 'Background image height. .4 subpixel units

'Private variables and constants
'-------------------------------
CONST _BG_TABLE_WIDTH% = 0
CONST _BG_TABLE_HEIGHT% = 1
CONST _BG_TABLE_NUM_ENTRIES% = 2

DIM _bg_table%(2-1)
DIM _bg_frameBufferOpened%=0

'Public subs and functions
'-------------------------

'Initialize the background module. Should be done once, at start-up of the game engine.
SUB BG_init
  bg_enabled% = 0
  bg_posX% = &H7FFFF
  bg_posY% = &H7FFFF 'So the initial call results in a full page render
  bg_velX% = 0
  bg_velY% = 0
  BG_clr
END SUB

'Load a Background BMP file.
SUB BG_load(fname$)
  OPEN fname$ FOR INPUT AS #1
  
  'This code is lifted out of Jiri's GRF.INC
  LOCAL vers$ = INPUT$(2, #1)
  IF vers$ <> "BM" THEN
    ERROR "BMP version "+STR(vers$)+" not supported. File: "+fname$
  ENDIF
  LOCAL sData$ = INPUT$(16, #1)
  sData$ = INPUT$(4, #1)
  'Convert to .4 subpixel units
  bg_width% = ABS(STR2BIN(INT32, sData$))<<4
  sData$ = INPUT$(4, #1)
  bg_height% = ABS(STR2BIN(INT32, sData$))<<4
  
  _bg_table%(_BG_TABLE_WIDTH%) = bg_width%
  _bg_table%(_BG_TABLE_HEIGHT%) = bg_height%

  CLOSE #1
  
  IF bg_width% > BG_MAX_WIDTH% THEN
    ERROR "Background too wide: "+STR$(bg_width%)+" file: "+fname$
  ENDIF
  IF bg_height% > BG_MAX_HEIGHT% THEN
    ERROR "Background too high: "+STR$(bg_height%)+" file: "+fname$
  ENDIF
  
  FRAMEBUFFER CREATE MAX(bg_width%>>4, MM.HRES), MAX(bg_height%>>4, MM.VRES)

  FRAMEBUFFER WRITE
  'Load BMP
  LOAD BMP fname$

  'Restore write page
  DM_restoreDefWritePage  
  
  _bg_frameBufferOpened%=1
  
  LM_writeLog("Loaded Background image: "+STR$(bg_width%>>4)+"x"+STR$(bg_height%>>4)+" pixels.")
END SUB

'Clear Background module configuration
SUB BG_clr
  IF _bg_frameBufferOpened% THEN
    FRAMEBUFFER CLOSE
    _bg_frameBufferOpened%=0
  ENDIF
  
  BG_clrPage
   
  bg_width%=0
  bg_height%=0  
  _bg_table%(_BG_TABLE_WIDTH%) = 0
  _bg_table%(_BG_TABLE_HEIGHT%) = 0  
END SUB

'Clear background page
SUB BG_clrPage
  'Set page write to the background page
  DM_setWritePage BG_PAGE_ID%
  
  BOX 0,0, MM.HRES, MM.VRES, 0,,0

  'Restore write page
  DM_restoreDefWritePage
END SUB

'Update background camera position using bg_velX/Y velocity vector
'Wraps as needed.
SUB BG_update
  IF (bg_velX%=0) AND (bg_velY%=0) THEN
    EXIT SUB
  ENDIF

  IF NOT _bg_frameBufferOpened% THEN
    ERROR "Attempt to set background position but no background in place"
  ENDIF
  
  LOCAL newPosX% = bg_posX%+bg_velX%
  LOCAL newPosY% = bg_posY%+bg_velY%

  'Pixels here, not subpixels
  LOCAL deltaXpixels% = (newPosX%\16) - (bg_posX%\16)
  LOCAL deltaYpixels% = (newPosY%\16) - (bg_posY%\16)

  'Wraparound
  IF newPosX% < 0 THEN
    INC newPosX%, bg_width%
  ENDIF
  IF newPosX% >= bg_width% THEN
    INC newPosX%, -bg_width%
  ENDIF

  IF newPosY% < 0 THEN
    INC newPosY%, bg_height%
  ENDIF
  IF newPosY% >= bg_height% THEN
    INC newPosY%, -bg_height%
  ENDIF
    
  'LM_writeLog("newPosX="+STR$(newPosX%))
  'LM_writeLog("newPosY="+STR$(newPosY%))
  'LM_writeLog("curPosX="+STR$(bg_posX%))
  'LM_writeLog("curPosY="+STR$(bg_posY%))

  'Pixel units here.
  LOCAL srcX% = newPosX%\16, srcY% = newPosY%\16, w%=MM.HRES, h%=MM.VRES
  LOCAL dstX% = 0, dstY% = 0
    
  'Incremental update  
  'Scroll page to the new offset.
  PAGE SCROLL BG_PAGE_ID%, -deltaXpixels%, deltaYpixels%, -1

  IF deltaXpixels% < 0 THEN
    w% = -deltaXpixels%
  ENDIF  

  IF deltaXpixels% > 0 THEN
    srcX% = (srcX% + MM.HRES - deltaXpixels%) MOD (bg_width%>>4)
    dstX% = MM.HRES - deltaXpixels%
    w% = deltaXpixels%
  ENDIF
  
  IF deltaYpixels% < 0 THEN
    h% = -deltaYpixels%
  ENDIF  

  IF deltaYpixels% > 0 THEN
    srcY% = (srcY% + MM.VRES - deltaYpixels%) MOD (bg_height%>>4)
    dstY% = MM.VRES - deltaYpixels%
    h% = deltaYpixels%
  ENDIF        
  
  _BG_render srcX%, srcY%, dstX%, dstY%, w%, h%
  
  'Update position
  bg_posX% = newPosX%
  bg_posY% = newPosY%
END SUB

'Set the position of the camera within the background. .4 subpixel units.
SUB BG_setPos(xx%, yy%)
  IF NOT _bg_frameBufferOpened% THEN
    ERROR "Attempt to set background position but no background in place"
  ENDIF
        
  'LM_writeLog("BG_setPos new X="+STR$(xx%))
  'LM_writeLog("BG_setPos new Y="+STR$(yy%))
    
  '_BG_render uses pixel units
  _BG_render xx%\16, yy%\16, 0, 0, MM.HRES, MM.VRES
  bg_posX% = xx%
  bg_posY% = yy%
END SUB
 
'Private subs and functions
'--------------------------

'void __BG_renderCSUB(long long *srcXp, long long *srcYp, long long *dstXp, long long *dstYp, 
'  long long *wp, long long *hp, char *callout_sub, long long *callout_args) {
CSUB _BG_renderCSUB INTEGER, INTEGER, INTEGER, INTEGER, INTEGER, INTEGER, STRING, INTEGER
  00000000
  4FF0E92D 680E4D3B 6800682D 681B6C29 F8D2688F 113FB000 8B02ED2D 9C13B087 
  68249D12 44349005 93016808 1A20EA4F 98056829 EB009D15 97020901 F3F7FB94 
  4213FB07 F3FAFB99 FB0A4296 92039913 9B14D04A 463C1C5F 46309B03 461A9902 
  BFB842B3 17F1460A 46901B93 461A17DF 463B9304 230AE9C5 E9C59B01 17DA0102 
  46134619 E9C5460A 9A052306 D01F454A 4A10EE08 45914B18 464CBFAC 68194654 
  1AA017D3 2300E9C5 4606465A 448317C7 EE1817D3 E9C50A10 E9C52304 47886708 
  F2FAFB94 4212FB0A D1E34591 4A10EE18 9A049B01 93014413 FB989B02 FB03F6F3 
  9B038616 D1B742B3 ECBDB007 E8BD8B02 BF008FF0 0800031C 08000314 
End CSUB

DIM _bg_csubCalloutArgs%(5)

SUB _BG_RENDER_CSUB_CALLOUT
  BLIT _bg_csubCalloutArgs%(0), _bg_csubCalloutArgs%(1), _bg_csubCalloutArgs%(2), _bg_csubCalloutArgs%(3), _bg_csubCalloutArgs%(4), _bg_csubCalloutArgs%(5), FRAMEBUFFER
END SUB

'Internal rendering function. Using pixel units!
SUB _BG_render(srcX%, srcY%, dstX%, dstY%, ww%, hh%)  
  'Do nothing if with or height 0.
  IF (ww%=0) OR (hh%=0) THEN
    EXIT SUB
  ENDIF
  
  'Set page write to the background page
  DM_setWritePage BG_PAGE_ID%

  _BG_renderCSUB srcX%, srcY%, dstX%, dstY%, ww%, hh%, "_BG_RENDER_CSUB_CALLOUT"+CHR$(0), _bg_csubCalloutArgs%(0)
    
  'Restore write page
  DM_restoreDefWritePage  
END SUB


