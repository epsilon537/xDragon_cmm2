'
'Much of this code is derived from Nautilus' excellent MAPSTER engine.
'

'Public constants
'----------------
CONST TE_MAX_MAP_WIDTH% = 32*10 'Max. map width in number of tiles.
CONST TE_MAX_MAP_HEIGHT% = 32 'Max. map height in number of tiles.
CONST TE_TILE_PIXEL_W% = 16 'Width of one tile in pixels.
CONST TE_TILE_PIXEL_H% = 16 'Height of one tile in pixels.
CONST TE_TILE_SUBPIXEL_W% = 256 'Width of one tile in .4 subpixels.
CONST TE_TILE_SUBPIXEL_H% = 256 'Height of one tile in .4 subpixels.
CONST TE_TILE_PIXEL_W_MASK% = 15 'Tile pixel width mask value, i.e. width-1. Used for efficient modulo operations. 
CONST TE_TILE_PIXEL_H_MASK% = 15 'Tile pixel height mask value, i.e. height-1. Used for efficient modulo operations.
CONST TE_TILE_PIXEL_W_SHIFT% = 4 'Tile pixel width shift value, i.e. log2(width). Used for efficient modulo operations.
CONST TE_TILE_PIXEL_H_SHIFT% = 4 'Tile pixel height shift value, i.e. log2(height). Used for efficient modulo operations.
CONST TE_TILE_SUBPIXEL_W_MASK% = 255 'Tile .4 subpixel width mask. i.e. subpixel width-1. Used for efficient modulo operations. 
CONST TE_TILE_SUBPIXEL_H_MASK% = 255 'Tile subpixel height mask value, i.e. subheight-1. Used for efficient modulo operations.
CONST TE_TILE_SUBPIXEL_W_SHIFT% = 8 'Tile subpixel width shift value, i.e. log2(subpixel width). Used for efficient modulo operations.
CONST TE_TILE_SUBPIXEL_H_SHIFT% = 8 'Tile subpixel height shift value, i.e. log2(subpixel height). Used for efficient modulo operations.

CONST TE_TILES_PER_ROW% = 20 'Number of tiles per row on screen.
CONST TE_TILES_PER_COL% = 15 'Number of tiles per column on screen.

CONST TE_PAGE_ID% = 2 'Page id onto which the tiles get rendered.

'Private variables and constants
'-------------------------------

CONST _TE__TE_TILE_TUP_MSK% = &HFFFF
CONST _TE_TILE_TUP_SHFT% = 0
CONST _TE_TILE_COL_MSK% = &HFF0000
CONST _TE_TILE_COL_SHFT% = 16
CONST _TE_TILE_TRIG_MSK% = &HFF000000
CONST _TE_TILE_TRIG_SHFT% = 24

CONST _TE_TABLE_MAX_MAP_WIDTH% = 0
CONST _TE_TABLE_MAX_MAP_HEIGHT% = 1
CONST _TE_TABLE_MAP_WIDTH% = 2
CONST _TE_TABLE_MAP_HEIGHT% = 3
CONST _TE_TABLE_MAP_XMASK% = 4
CONST _TE_TABLE_MAP_YMASK% = 5
CONST _TE_TABLE_TRIGMAP_VIEW% = 6
CONST _TE_TABLE_TRIGMAP_PREV_TILEX% = 7
CONST _TE_TABLE_TRIGMAP_PREV_TILEY% = 8

CONST _TE_TABLE_NUM_ENTRIES% = 9

DIM _te_table%(_TE_TABLE_NUM_ENTRIES%-1)

'View on triggerMap, defined as a Box relative to wm_view position. Tile units.
DIM te_trigMapView% = Box_create%(0,0,0)
_te_table%(_TE_TABLE_TRIGMAP_VIEW%) = te_trigMapView%

'Tile Engine Map
DIM _te_map%(TE_MAX_MAP_WIDTH%-1, TE_MAX_MAP_HEIGHT%-1)

DIM _te_mapXmsk%, _te_mapYmsk%

DIM _te_prevViewXpixels%, _te_prevViewYpixels% 'Pixels units

DIM _te_deltaXpixels%, _te_deltaYpixels%, _te_startTileX%, _te_startTileY%
DIM _te_endTileX%, _te_endTileY%, _te_screenStartX%, _te_screenStartY%

'Read-Only variables
'-------------------
DIM te_enabled% 'Tile Engine enable flag
DIM te_trigMapEnabled% 'Tile Engine Trigger Map enable flag

'Tile Sheet page id.
DIM te_sheetPageId%

'Private subs and functions
'--------------------------

'Internal function used by _TE_loadMap
FUNCTION _TE_getVal$(nextRow%)  
  LOCAL f$
  
  _TE_getVal$ = ""
  nextRow%=0

  DO 
    f$ = INPUT$(1, #1)
    SELECT CASE f$
      CASE ","
        EXIT FUNCTION
      CASE CHR$(13), "" 'CR or EOF
        nextRow% = 1
        EXIT FUNCTION
      CASE CHR$(10) 'Absorb LF
      CASE ELSE
        _TE_getVal$ = _TE_getVal$ + f$
    END SELECT
  LOOP UNTIL EOF(#1)
END FUNCTION

'Internal sub used by TE_load* functions
'Sets a tile in the map array at given mask/shift position.
SUB _TE_setTile(xx%, yy%, tileVal%, msk%, shft%)
  'LM_writeLog("setTile: "+STR$(xx%)+" "+STR$(yy%)+" "+STR$(tile_tuple%))
  
  IF xx% >= TE_MAX_MAP_WIDTH% THEN
    ERROR "Tile Map wider than max: "+STR$(xx%)+" vs. "+STR$(TE_MAX_MAP_WIDTH%)
  ENDIF
  IF yy% >= TE_MAX_MAP_HEIGHT% THEN
    ERROR "Tile Map higher than max: "+STR$(yy%)+" vs. "+STR$(TE_MAX_MAP_HEIGHT%)
  ENDIF
  
  _te_map%(xx%, yy%) = (_te_map%(xx%, yy%) AND (INV msk%)) OR ((tileVal%<<shft%) AND msk%)
END SUB

'void _TE_trigMapUpdateCSUB(long long *tileXp, long long *tileYp, 
'  char *callout_sub, long long *callout_args) {
CSUB _TE_trigMapUpdateCSUB INTEGER, INTEGER, STRING, INTEGER
  00000000
  4FF0E92D B09F4CBE 46986800 92066824 68096BE2 46136B95 E040F8D2 1B409010 
  0C0EEBA1 9511910D 020CEA50 80E0F000 1C416B1D 2902931D 2302E9D4 EB036BA4 
  9B1D09C5 EB029407 681A04C5 92189412 F8CD6A1A 920C904C 9B126A9A E9D39219 
  E9D92300 E9CD4500 F2402302 9B0D80C4 46192C01 72E3EA4F 0300F175 E9CD9B10 
  46191216 72E3EA4F 120AE9CD 80A6F2C0 8070F8CD 46AB46A2 2316E9DD 8902E9DD 
  2308E9CD 17D34672 231AE9CD 0F01F1B8 0300F179 9B19DB7A 98189A08 2300401A 
  FB004611 E9DDF202 E9DDAB1A EBB1450A 920D010A 030BEB63 991C910E E9CD930F 
  9B0C8902 0E00F04F EA039A07 9B060A04 EA4F4677 1C586C1E 2F009B0D F8524453 
  F0033033 EA4F467F BF086316 4F7FF1B6 230EEA43 9A11D030 AE04E9CD AB04E9DD 
  8902E9DD 0A02EBBA 7BE2EB6B EB7B45C2 DA010209 DA1F45F3 890EE9DD 0F00F1B8 
  0200F179 9A13DB06 AB00E9D2 EB7945D0 DB11020B 3C04E9C1 8908E9DD 681B4B69 
  E9C19110 E9C14500 47988902 99109B12 2300E9D3 2302E9CD E9DD3401 E9DDAB0A 
  F1458902 EB180500 464A030A 465B9300 0303EB42 E9DD9301 42942300 0303EB75 
  9B13DB9F 3400E9D3 46A3469A 33019B08 9B099308 4516E9DD 0300F143 EB149309 
  9314030A 030BEB45 E9DD9315 E9DD1214 428B3408 0302EB74 AF68F6FF E9DD9B1D 
  E9C3120A 4619120E 2316E9DD 2310E9C1 E8BDB01F F10C8FF0 2B020301 AF37F63F 
  9B022801 3001D010 8084F000 F1BC9A10 44130F01 D0779A0D 92043B01 9B04930A 
  9B10930E E00F9308 44139A10 3B019A0D 93084414 930E1E63 230DE9DD DB534293 
  930A9B08 93049B0D 46199B04 E9CD17DA 9B081200 42939A0A 9B04DC39 9E089A19 
  0702EA03 46349B0A F10317F5 9B180901 F307FB03 93022700 46C84643 461E46B1 
  2F009A06 F1029B0C 9A020001 0303EA09 0901F109 9A074413 3033F852 4A7FF003 
  F1BABF08 EA4F4F7F D00A631A 1200E9DD 3704E9C6 681B4B13 4500E9C6 1202E9C6 
  34014798 0500F145 D1D945C1 9B0046B0 33019A01 0200F142 9B049300 33019201 
  42939A0E DDB49304 46199B0D 9B1017DA 1216E9CD 17DA4619 120AE9CD BF00E76E 
  0800031C 08000314 44143B01 1E63930A E7849304 441C9B0D 93089B10 930E1E63 
  BF00E78A 
End CSUB

DIM _te_trigMapCallOutArgs%(2)

SUB _TE_TRIGMAP_UPDATE_CALLOUT
  dummy_i% = EVT_MGR_onEvent%(EVT_TE_TRIG_create%(_te_trigMapCallOutArgs%(0), _te_trigMapCallOutArgs%(1), _te_trigMapCallOutArgs%(2)))        
END SUB

'Internal function called by TE_tileMap.
'Update view position in trigger map to tileX%/Y% (tile units).
'Wraps as needed. Generates EVT_TE_TRIG events when 'trigger tiles' scroll into view
SUB _TE_trigMapUpdate(tileX%, tileY%)
  _TE_trigMapUpdateCSUB tileX%, tileY%, "_TE_TRIGMAP_UPDATE_CALLOUT"+CHR$(0), _te_trigMapCallOutArgs%(0)
END SUB

'Internal function used by the TE_load* functions
'Loads a .csv map into the map array at given mask/shift position.
SUB _TE_loadMap(fname$, mask%, shift%)
  IF (_te_table%(_TE_TABLE_MAP_WIDTH%)=0) OR (_te_table%(_TE_TABLE_MAP_HEIGHT%)=0) THEN
    ERROR "TileMap must be loaded before loading "+fname$
  ENDIF

  LOCAL g$
  LOCAL mx%, my%, nextRow%=0

  OPEN fname$ FOR INPUT AS #1
  
  my%=0
  DO
    nextRow%=0 : mx%=0
    DO
      g$ = _TE_getVal$(nextRow%)            
      IF g$<>"" THEN
        _TE_setTile mx%, my%, VAL(g$), mask%, shift%
        INC mx%
      ENDIF
    LOOP UNTIL nextRow%
    INC my%
  LOOP UNTIL EOF(#1)
  CLOSE #1  
END SUB

DIM _te_tilePageCallOutArgs%(3)

SUB TE_TILE_PAGE_CALLOUT
  BLIT _te_tilePageCallOutArgs%(0), _te_tilePageCallOutArgs%(1), _te_tilePageCallOutArgs%(2), _te_tilePageCallOutArgs%(3), TE_TILE_PIXEL_W%, TE_TILE_PIXEL_H%, te_sheetPageId%
END SUB

'void _TE_tilePageCSUB(long long *screenStartYp, long long *screenStartXp, 
'  long long *startTileXp, long long *startTileYp,
'  long long *endTileXp, long long *endTileYp,
'  char *callout_sub, long long *callout_args) {
CSUB _TE_tilePageCSUB INTEGER, INTEGER, INTEGER, INTEGER, INTEGER, INTEGER, STRING, INTEGER
  00000000
  4FF0E92D 68124C3F 681F6824 68096BE3 9020F8D3 ED2D6800 B08D8B02 92059D19 
  9E186BA2 9203682D 42AF681A 97046A9B 68339308 9E1B9109 9306920A 1C6ADC5C 
  F1034601 F8CD0B01 9207902C E9CD17C2 9A1A1200 EE081C50 E9DD0A10 42933205 
  9B04DC3D 9A0846B2 F8DD465E F04F9014 EA030800 9B090702 0A10EE18 B02CF8DD 
  17DD461C FB039B0A EA09F707 4B1E0C0B 0901F109 44BC6819 90029B03 303CF853 
  8004F8CA 1E03EA4F 800CF8CA 1C13EA4F 627FF40E 637FF40C 2000F8CA 3008F8CA 
  2300E9DD 4504E9CA 2306E9CA 34104788 F1459802 45B10500 46B3D1D7 9B004656 
  33109A01 0200F142 9B049300 33019201 429A9A07 D1B09304 ECBDB00D E8BD8B02 
  BF008FF0 0800031C 08000314 
End CSUB

'void _TE_checkColMapCSUB(long long *obj_idp, long long *xp, long long *yp, long long *res) 
CSUB _TE_checkColMapCSUB INTEGER, INTEGER, INTEGER, INTEGER
  00000000
  68124A3A 4FF0E92D B0836991 93016806 7036F851 F3C769D1 689037C9 3036F851 
  F8506811 F8510037 EB031037 68510E01 F8516A13 EB0E1037 F8530C00 6B963036 
  0001F1BC 6BD1440B BF4868D2 00FEF10C B000F8D1 2037F852 6A0F1200 F8D1441A 
  F1B2A028 BF480901 09FEF102 0F00F1BE F10EBFB8 2B000EFF 2929EA4F 33FFBFB8 
  2E2EEA4F 2F23EBB9 2823EA4F F04FDB25 45700C00 EA08DB1D 4672010A F101FB0B 
  4290E001 EA02DB15 46650307 440B3201 F8562D00 F4033033 BF08047F 0F7FF5B4 
  2201D0EF 99012300 2300E9C1 E8BDB003 F1088FF0 45C10801 2200DADB 99012300 
  2300E9C1 E8BDB003 BF008FF0 0800031C 
End CSUB

'Public subs and functions
'-------------------------

'Initialize the Tile Engine.
SUB TE_init
  TE_clrMap
  te_enabled% = 0
  te_trigMapEnabled% = 0
  box_width%(te_trigMapView%) = MM.HRES\TE_TILE_PIXEL_W%
  box_height%(te_trigMapView%) = MM.VRES\TE_TILE_PIXEL_H%
  _te_table%(_TE_TABLE_MAX_MAP_WIDTH%) = TE_MAX_MAP_WIDTH%
  _te_table%(_TE_TABLE_MAX_MAP_HEIGHT%) = TE_MAX_MAP_HEIGHT%
END SUB

'Set Tile Engine Map dimensions, in tiles.
SUB TE_setMapDim(ww%, hh%)
  _te_table%(_TE_TABLE_MAP_WIDTH%) = ww%
  _te_table%(_TE_TABLE_MAP_HEIGHT%) = hh%
END SUB

'Get Tile Engine Map dimensions, in tiles.
SUB TE_getMapDim(ww%, hh%)
  ww% = _te_table%(_TE_TABLE_MAP_WIDTH%) 
  hh% = _te_table%(_TE_TABLE_MAP_HEIGHT%)
END SUB

'Wraparound indexing in the tilemap can be enabled by specifying a mask here.
'E.g. a mask of 255 will result in a wraparound at tile offset 256.
'If set to -1, no wrapping is done and expectation is that world boundary
'matches the map dimensions. Unit: tiles.
SUB TE_setMapMask(xMask%, yMask%)
  _te_table%(_TE_TABLE_MAP_XMASK%) = xMask%
  _te_table%(_TE_TABLE_MAP_YMASK%) = yMask%
  _te_mapXmsk% = xMask%
  _te_mapYmsk% = yMask%
END SUB

'Get the x and y wraparound mask values.
SUB TE_getMapMask(xMask%, yMask%)
  xMask% = _te_table%(_TE_TABLE_MAP_XMASK%)
  yMask% = _te_table%(_TE_TABLE_MAP_YMASK%)
END SUB

'Clear the tile map
SUB TE_clrMap
  LOCAL xx%, yy%
  FOR xx%=0 TO TE_MAX_MAP_WIDTH%-1
    FOR yy%=0 TO TE_MAX_MAP_HEIGHT%-1
      _te_map%(xx%, yy%) = -1
    NEXT yy%
  NEXT xx%

  TE_setMapMask(-1, -1)
  TE_setMapDim(0, 0)
  
  _te_prevViewXpixels%=&H7FFFFFFFFFFF
  _te_prevViewYpixels%=&H7FFFFFFFFFFF
  _te_table%(_TE_TABLE_TRIGMAP_PREV_TILEX%) = &H7FFFFFFFFFFF
  _te_table%(_TE_TABLE_TRIGMAP_PREV_TILEY%) = &H7FFFFFFFFFFF
END SUB

'Clear the tile page
SUB TE_clrPage
  'Set page write to the background page
  DM_setWritePage TE_PAGE_ID%
  
  BOX 0,0, MM.HRES, MM.VRES, 0,,0

  'Restore write page
  DM_restoreDefWritePage
END SUB

'Load a trigger map .csv into the map array.
SUB TE_loadTrigMap(fname$)
  LM_writeLog("Loading trigger map: "+fname$)
  _TE_loadMap(fname$, _TE_TILE_TRIG_MSK%, _TE_TILE_TRIG_SHFT%)  
END SUB

'Load a collision map .csv into the map array,
SUB TE_loadColMap(fname$)
  LM_writeLog("Loading collision map: "+fname$)
  _TE_loadMap(fname$, _TE_TILE_COL_MSK%, _TE_TILE_COL_SHFT%)
END SUB

'Load a tilemap .csv into the map array. Also specify the tilesheet it's referencing.
SUB TE_loadTileMap(fname$, sheetLabel$)
  LM_writeLog("Loading tile map: "+fname$)

  te_sheetPageId% = Sheet_find%(sheetLabel$)
  IF te_sheetPageId%=-1 THEN
    ERROR "TileSheet not found: "+sheetLabel$)
  ENDIF
    
  LOCAL g$
  LOCAL mx%, my%, t%, tx%, ty%, nextRow%=0

  OPEN fname$ FOR INPUT AS #1

  LOCAL mapWidth%=0
  LOCAL mapHeight%=0
  
  my%=0
  DO
    nextRow%=0 : mx%=0
    DO
      g$ = _TE_getVal$(nextRow%)
            
      IF g$<>"" THEN
        t% = VAL(g$) 'read tile ID
        IF t% <> -1 THEN 'Skip -1 values
          tx% = t% MOD TE_TILES_PER_ROW%  ' and convert to tile_X and Tile_Y
          ty% = t% \ TE_TILES_PER_ROW%
            
          _TE_setTile mx%, my%, (ty% << 8) + tx%, _TE__TE_TILE_TUP_MSK%, _TE_TILE_TUP_SHFT%
          mapWidth% = MAX(mx%+1, mapWidth%)
          mapHeight% = MAX(my%+1, mapHeight%)
        ENDIF          
        INC mx%
      ENDIF
    LOOP UNTIL nextRow%
    INC my%
  LOOP UNTIL EOF(#1)
  CLOSE #1
  
  TE_setMapDim(mapWidth%, mapHeight%)
  LM_writeLog("Tile Map Width = "+STR$(mapWidth%))
  LM_writeLog("Tile Map Height = "+STR$(mapHeight%))
END SUB

'Tile Page at given position. .4 subpixel units.
'Also generates TE_TRIG events if the trigger map is enabled.
SUB TE_tilePage(viewX%, viewY%)  
  LOCAL viewXpixels% = viewX%\16
  LOCAL viewYpixels% = viewY%\16
  
  'Don't update if the view difference is less than a pixel
  IF (viewXpixels% = _te_prevViewXpixels%) AND (viewYpixels% = _te_prevViewYpixels%) THEN
    EXIT SUB
  ENDIF
  
  _te_deltaXpixels% = viewXpixels% - _te_prevViewXpixels%
  _te_deltaYpixels% = viewYpixels% - _te_prevViewYpixels%

  'LM_writeLog("deltaXpixels = "+STR$(_te_deltaXpixels%))
  'LM_writeLog("deltaYpixels = "+STR$(_te_deltaYpixels%))

  _te_startTileX% = viewXpixels%>>TE_TILE_PIXEL_W_SHIFT%
  _te_startTileY% = viewYpixels%>>TE_TILE_PIXEL_H_SHIFT%
  
  'The rightmost/lowest visible tile.  
  _te_endTileX% = _te_startTileX% + TE_TILES_PER_ROW%
  _te_endTileY% = _te_startTileY% + TE_TILES_PER_COL%
    
  'The screen position of the first BLIT destination.
  'Note that generally these will be negative/offscreen values.
  _te_screenStartX% = (_te_startTileX%<<TE_TILE_PIXEL_W_SHIFT%) - viewXpixels%
  _te_screenStartY% = (_te_startTileY%<<TE_TILE_PIXEL_H_SHIFT%) - viewYpixels%
    
  IF te_trigMapEnabled% THEN
    _TE_trigMapUpdate(_te_startTileX%-box_cornerX%(te_trigMapView%), _te_startTileY%-box_cornerX%(te_trigMapView%))
  ENDIF

  'Set page write to the tile page
  DM_setWritePage TE_PAGE_ID%
  
  'For smaller-than-one-tile deltas, we scroll and retile the delta.    
  IF ((ABS(_te_deltaXpixels%) < TE_TILE_PIXEL_W%) AND (ABS(_te_deltaYpixels%) < TE_TILE_PIXEL_H%)) THEN
    'Scroll page to the new offset.
    PAGE SCROLL TE_PAGE_ID%, -_te_deltaXpixels%, _te_deltaYpixels%, -1

    IF _te_deltaXpixels% < 0 THEN
      _te_endTileX% = _te_startTileX% + 1
    ENDIF  

    IF _te_deltaXpixels% > 0 THEN
      _te_startTileX% = _te_endTileX% - 1
      _te_screenStartX% = (_te_startTileX%<<TE_TILE_PIXEL_W_SHIFT%) - viewXpixels%
    ENDIF
    
    IF _te_deltaYpixels% < 0 THEN
      _te_endTileY% = _te_startTileY% + 1
    ENDIF  

    IF _te_deltaYpixels% > 0 THEN
      _te_startTileY% = _te_endTileY% - 1
      _te_screenStartY% = (_te_startTileY%<<TE_TILE_PIXEL_H_SHIFT%) - viewYpixels%
    ENDIF        
  ENDIF

  'LM_writeLog("viewX = "+STR$(viewX%))
  'LM_writeLog("viewY = "+STR$(viewY%))
  'LM_writeLog("te_startTileX = "+STR$(_te_startTileX%))
  'LM_writeLog("te_startTileY = "+STR$(_te_startTileY%))
  'LM_writeLog("te_endTileX = "+STR$(_te_endTileX%))
  'LM_writeLog("te_endTileY = "+STR$(_te_endTileY%))  
  'LM_writeLog("te_screenStartX = "+STR$(_te_screenStartX%))
  'LM_writeLog("te_screenStartY = "+STR$(_te_screenStartY%))
  'LM_writeLog("te_SheetPageId% = "+STR$(te_sheetPageId%))
  'LM_writeLog("_te_mapXmsk% = "+STR$(_te_mapXmsk%))
  'LM_writeLog("_te_mapYmsk% = "+STR$(_te_mapYmsk%))

  _TE_tilePageCSUB _te_screenStartY%, _te_screenStartX%, _te_startTileX%, _te_startTileY%, _te_endTileX%, _te_endTileY%, "TE_TILE_PAGE_CALLOUT"+CHR$(0), _te_tilePageCallOutArgs%(0)
   
  'Restore write page
  DM_restoreDefWritePage  
  
  _te_prevViewXpixels% = viewXpixels%
  _te_prevViewYpixels% = viewYpixels%
END SUB

'Check for collisions against the decor.
'Returns 1 if collision, 0 if not.
FUNCTION TE_checkColMap%(objId%, xx%, yy%)
  _TE_checkColMapCSUB objId%, xx%, yy%, TE_checkColMap%
END FUNCTION

'Match world box with map edges, multiplied with given integer multiplier.
SUB TE_matchWorldWithMap(xScaleF%, yScaleF%)
  box_width%(wm_boundary%) = xScaleF%*_te_table%(_TE_TABLE_MAP_WIDTH%)*TE_TILE_SUBPIXEL_W%
  box_height%(wm_boundary%) = yScaleF%*_te_table%(_TE_TABLE_MAP_HEIGHT%)*TE_TILE_SUBPIXEL_H%
END SUB

