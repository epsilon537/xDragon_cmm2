'Public constants
'----------------

'Collision group ids. Collision group ids are associated with a cg_map bitmap which specifies with which other
'Collision groups an object can collide.

CONST CG_ID_SPECTRAL% = 0 'Collision group id for objects that don't collide.
CONST CG_ID_SPECTAL_MSK% = 1

CONST CG_ID_1% = 1
CONST CG_ID_1_MSK% = 2
CONST CG_ID_2% = 2
CONST CG_ID_2_MSK% = 4
CONST CG_ID_3% = 3
CONST CG_ID_3_MSK% = 8
CONST CG_ID_4% = 4
CONST CG_ID_4_MSK% = 16
CONST CG_ID_5% = 5
CONST CG_ID_5_MSK% = 32
CONST CG_ID_6% = 0
CONST CG_ID_6_MSK% = 64
CONST CG_ID_TILES% = 7 'A pre-defined collision group id for tiles.
CONST CG_ID_TILES_MSK% = 128

CONST WM_X_CLAMP_MSK% = 1 'See WM_setViewPos
CONST WM_Y_CLAMP_MSK = 2 'See WM_setViewPos

'Read-Only variables
'-------------------
DIM wm_isStarted% = 0 'Flag indicating that the WorldManager has started.
'Player view of world (Use WM_get/setView... accessors to access). .4 subpixel units
DIM wm_view% = Box_create%(0,0,0)

'Read-Write variables
'--------------------

'Per-collision group bitmaps where each bit position corresponds with the group id of 
'collision groups to check against for collisions.
'Bit position 7 corresponds to the tiles group.
DIM cg_maps%(OBJ_MAX_COLLISION_GROUP%)

'Get/Set game world boundary here. .4 subpixel units
DIM wm_boundary% = Box_create%(0,0,0)

DIM wm_viewVelX%=0 'view horizontal velocity. .4 subpixel units.
DIM wm_viewVelY%=0 'view vertical velocity. .4 subpixel units.

'Private variables and constants
'-------------------------------
'LOCALS made global for performance
DIM _wm_moveBox% = Box_create%(0,0,0)
DIM _wm_moveBoxIntersect%, _wm_boxRel%
DIM _wm_colObjEv% = EVT_COL_create%(0,0,0) 'Create one event object. Keep re-using it.
DIM _wm_colTileEv% = EVT_COL_create%(0, EVT_COL_OBJ2_IS_TILE%, 0) 'Create one event object. Keep re-using it.
DIM _wm_deletionsLid%
DIM _wm_collisionLid%
DIM _wm_objCG%

'Private subs and functions
'-------------------------
'void _wm_updateCSUB(long long *active_objects_lidp, char *callout_sub, long long *callout_arg1, long long *callout_arg2, long long *callout_arg3)
CSUB _WM_updateCSUB INTEGER, STRING, INTEGER, INTEGER, INTEGER
  00000000
  4FF0E92D 46984C34 68266807 6448F44F 8B02ED2D B0876933 FB0469F0 9205F407 
  02C7EB03 6A309002 F8D66973 9003902C 92046AB0 E9D29001 2E016700 0200F177 
  1C4DDB45 F04F441C F04F0A00 EE080B00 68205A10 EB039B01 00C305C0 1200E9D5 
  0C03EB09 D103430A 1200E9DC D024430A 17C19E02 18F79A05 44339E03 68364E17 
  0100E9C2 68296838 1841687F EE18686D EB450A10 E9C80707 681D1700 1000F8DC 
  2004F8DC 1869685B 0303EB42 E9C29A12 47B01300 E9D39B04 F11A6700 F1040A01 
  F14B0408 45B20B00 0307EB7B B007DBC1 8B02ECBD 8FF0E8BD 0800031C 08000314 
End CSUB

DIM _wm_updateCalloutArg1%, _wm_updateCalloutArg2%, _wm_updateCalloutArg3%

SUB WM_UPDATE_OBJ_MOVE_CALLOUT
  dummy_i% = WM_moveObj%(_wm_updateCalloutArg1%, _wm_updateCalloutArg2%, _wm_updateCalloutArg3%)
END SUB

DIM _wm_drawCalloutArg%

SUB WM_DRAW_OBJ_DRAW_CALLOUT
  Obj_draw _wm_drawCalloutArg%
END SUB

'void _WM_drawCSUB(long long *visible_objects_lids, long long *wm_viewp, char *callout_sub, long long *callout_arg)
CSUB _WM_drawCSUB INTEGER, INTEGER, STRING, INTEGER
  00000000
  4FF0E92D B0956809 68244C58 00CB930B 9000F8D4 F1A0930C F8D40308 930FB004 
  F85968A3 930E7031 0318F100 A031F85B 68E39311 9308920A F8CD6923 9310904C 
  93126963 930569A3 930669E3 93076A23 F44F9B0F F8536248 930F4F08 EB039B10 
  FB0203C4 9309F404 2300E9D3 E9CD2A01 F1732300 DB560300 F04F9B12 9A0C0800 
  0900F04F 9B0E441C 44139E13 6822930D 99079B05 C032F853 F3CC9B06 F85330C9 
  F8515032 F85B3032 93041030 3030F856 462B441D 429F9D04 DD3E4429 F8559D0E 
  442B5030 BFB442BB 23012300 DD3D458A F8559D08 44295030 DA2B4551 5300F00C 
  23009302 E9DD9303 EA500102 D00E0301 17D14610 9A0B4B1E E9C2681B 9A0A0100 
  47981C50 E9D39B09 E9CD2300 F1182300 F1040801 E9DD0408 F1492300 45900900 
  0303EB79 9B11DBB3 42939A0F B015D190 8FF0E8BD D1DB2B00 9D0DE7D0 443D682D 
  DB0E429D F04F458A DCC10301 46AE9D08 F85E9D0C 44555005 DAEB428D D1BD458A 
  429FE7B6 2300D0AA BF00E7B0 0800031C 08000314 
End CSUB

'void _WM_getCollisionsCSUB(long long *obj_idp, long long *xxp, long long *yyp, long long *collidable_objects_lids, long long *collisions_lidp)
CSUB _WM_getCollisionsCSUB INTEGER, INTEGER, INTEGER, INTEGER, INTEGER
  00000000
  4FF0E92D B0914C5C F44F6805 68246848 F8D49501 F8D4A018 F85AC000 F8D46035 
  F3C6E004 930F35C9 68A02301 F3C66809 68126642 7035F85C F85E9303 440F3035 
  441368E1 2035F850 18BA900A 6A639304 9A1A9206 3036F853 98046812 F851910B 
  69251035 1841930D 03C2EB05 9108950E 69639305 901CF8D4 FB089307 F8D4F302 
  930C8020 9A039B0D F0134113 93090301 9B03D107 2B073301 D1F39303 E8BDB011 
  9B0F8FF0 0032F853 6248F44F F8539B0E FB023030 2B00F000 DDEA9302 26009B07 
  E0174418 F8549C0A 441D503B DB2542BD 9C099D04 DB2D428D F8539B0B EB03303B 
  45AB0B01 428DDA29 3601DD24 30089B02 D0CE42B3 9B016802 D0F64293 3032F85A 
  5032F859 3BC9F3C3 1032F858 303BF85C 403BF85E 4421442B DAD3429F 429F9C06 
  BFCC4625 24012400 9D04429D 2400BFB8 DAD1428D 428B9B08 2C00DBD7 9B05D0D5 
  F5B4681C DA0D7FC8 9D0C9B07 01C4EB03 17D33401 17E54429 2300E9C1 E9C39B05 
  E7C24500 32FFF04F 33FFF04F E9C1991A B0112300 8FF0E8BD 0800031C 
End CSUB

'Public subs and functions
'-------------------------

'Start all WorldManager services.
SUB WM_startUp
  _wm_deletionsLid% = ObjLst_create%()
  _wm_collisionLid% = ObjLst_create%()
  
  LOCAL ii%5
  FOR ii%=0 TO OBJ_MAX_COLLISION_GROUP%
    cg_maps%(ii%) = 0
  NEXT ii%
  
  SG_startUp
  wm_isStarted% = 1
END SUB

'Shut down WorldManager services.
SUB WM_shutDown
  LOCAL ol% = ObjLst_create%()

  WM_getAllObjs(ol%, 1)

  LOCAL index%=0
  DO WHILE index% < objLst_numElems%(ol%)
    Obj_destroy objLst_list%(index%, ol%)    
    INC index%
  LOOP

  ObjLst_destroy(ol%)
  
  SG_shutDown
  ObjLst_destroy(_wm_deletionsLid%)
  ObjLst_destroy(_wm_collisionLid%)
  wm_isStarted% = 0
END SUB

'Return list of all Objects in world.
SUB WM_getAllObjs(allObjsLid%, inclInActive%)
  IF inclInActive% THEN
    ObjLst_copy(sg_activesLid%, allObjsLid%) 'From to  
    ObjLst_append(allObjsLid%, sg_inactivesLid%) 'To From
  ELSE
    ObjLst_copy(sg_activesLid%, allObjsLid%) 'From to  
  ENDIF
END SUB

'Return list of all Objects in world matching type. Note: list ownership transfers to caller.
'List must be deleted to prevent resource leaks.
FUNCTION WM_objsOfType%(objType%)
  LOCAL lid% = ObjLst_create%()
  LOCAL cur_objId%, index%=0

  DO WHILE index% < objLst_numElems%(sg_activesLid%)
    cur_objId% = objLst_list%(index%, sg_activesLid%)

    IF Obj_getType%(cur_objId%) = objType% THEN
      ObjLst_insert(lid%, cur_objId%)
    ENDIF
    
    INC index%
  LOOP

  WM_objsOfType% = lid%
END FUNCTION

'Indicate Object is to be deleted at end of current game loop.
SUB WM_markForDelete(objId%)
  'Only add once
  LOCAL index%=0

  DO WHILE index% < objLst_numElems%(_wm_deletionsLid%)
    IF objId% = objLst_list%(index%, _wm_deletionsLid%) THEN
      EXIT SUB 'Already marked for delete
    ENDIF
    INC index%
  LOOP

  'Not in list yet, so add:
  ObjLst_insert(_wm_deletionsLid%, objId%)
  
  'LM_writeLog("Marked for delete: "+STR$(objId%))
END SUB

'Update World, including view position. Delete objects marked for deletion.
SUB WM_update    
  IF (wm_viewVelX%<>0) OR (wm_viewVelY%<>0) THEN
    'LM_writeLog("view velocity x = "+HEX$(wm_viewVelX%))
    'LM_writeLog("view velocity y = "+HEX$(wm_viewVelY%))

    'Update view position    
    LOCAL res% = WM_setViewPos%(box_cornerX%(wm_view%) + wm_viewVelX%, box_cornerY%(wm_view%) + wm_viewVelY%)
    IF res% AND WM_X_CLAMP_MSK% THEN
      'reset view x-velocity
      wm_viewVelX% = 0
    ENDIF
    IF res% AND WM_Y_CLAMP_MSK THEN
      'reset view y-velocity
      wm_viewVelY% = 0
    ENDIF      
  ENDIF
   
  LOCAL index%=0
  'Careful here: It's possible that an Object destructor adds items to the deletions list
  'in which case the number of elements changes. A FOR loop does not re-evaluate the end
  'condition at every pass so a WHILE loop is necessary here.  
  DO WHILE index% < objLst_numElems%(_wm_deletionsLid%)
    Obj_destroy objLst_list%(index%, _wm_deletionsLid%)
    INC index%
  LOOP
  
  'Clear deletions list for next update
  objLst_numElems%(_wm_deletionsLid%) = 0

  _WM_updateCSUB sg_activesLid%, "WM_UPDATE_OBJ_MOVE_CALLOUT"+CHR$(0), _wm_updateCalloutArg1%, _wm_updateCalloutArg2%, _wm_updateCalloutArg3%
END SUB

'Draw all objects
SUB WM_draw  
  _WM_drawCSUB sg_visiblesLid%(0), wm_view%, "WM_DRAW_OBJ_DRAW_CALLOUT"+CHR$(0), _wm_drawCalloutArg%
END SUB

'Try to move object to xx/yy.
'If collision: send collision events
'If no collision or soft collision, move ok, else don't move
'Return 0 if move ok, else -1
FUNCTION WM_moveObj%(objId%, xx%, yy%)
  WM_moveObj%=0
  
  IF te_enabled% THEN
    'Check against tile collisions?
    'getColGroup
    getBitField obj_flags%(objId%), OBJ_FLG_CG_MSK%, OBJ_FLG_CG_SHFT%, _wm_objCG%
    IF cg_maps%(_wm_objCG%) AND CG_ID_TILES_MSK% THEN
      IF TE_checkColMap%(objId%, xx%, yy%) THEN
       'Setup collision event with obj. 2 a tile.
        EVT_COL_setObj1(_wm_colTileEv%, objId%)
        EVT_COL_setPos(_wm_colTileEv%, xx%, yy%)

        dummy_i% = Obj_eventHandler%(objId%, _wm_colTileEv%)
        
        'If HARD, then can't move
        LOCAL solidness%
        getBitField obj_flags%(objId%), OBJ_FLG_SOLID_MSK%, OBJ_FLG_SOLID_SHFT%, solidness%
        IF solidness% = OBJ_HARD% THEN
          WM_moveObj%=-1
        ENDIF
      ENDIF
    ENDIF
  ENDIF

  'Clear given object list
  objLst_numElems%(_wm_collisionLid%) = 0
  _WM_getCollisionsCSUB objId%, xx%, yy%, sg_collidablesLid%(0), _wm_collisionLid%
  IF _wm_collisionLid%=-1 THEN: ERROR "Collision list is full.": ENDIF
    
  IF objLst_numElems%(_wm_collisionLid%) <> 0 THEN
    LOCAL currentObject%, index%=0
    LOCAL solidness1%, solidness2%

    DO WHILE index% < objLst_numElems%(_wm_collisionLid%)
      currentObject% = objLst_list%(index%, _wm_collisionLid%)

      'Set up collision event
      EVT_COL_setObj1(_wm_colObjEv%, objId%)
      EVT_COL_setObj2(_wm_colObjEv%, currentObject%)
      EVT_COL_setPos(_wm_colObjEv%, xx%, yy%)
  
      'Send to both objects
      dummy_i% = Obj_eventHandler%(objId%, _wm_colObjEv%)
      dummy_i% = Obj_eventHandler%(currentObject%, _wm_colObjEv%)
      
      'If both HARD, then can't move
      getBitField obj_flags%(objId%), OBJ_FLG_SOLID_MSK%, OBJ_FLG_SOLID_SHFT%, solidness1%
      getBitField obj_flags%(currentObject%), OBJ_FLG_SOLID_MSK%, OBJ_FLG_SOLID_SHFT%, solidness2%
      
      IF (solidness1% = OBJ_HARD%) AND (solidness2% = OBJ_HARD%) THEN
        WM_moveObj%=-1
      ENDIF
      
      INC index%
    LOOP 'End iteration
  ENDIF 'No object collisions    

  IF WM_moveObj%=-1 THEN: EXIT FUNCTION: ENDIF 'Move not allowed.
        
  'No collisions between HARD objects, so can move.
  getBitField obj_flags%(objId%), OBJ_FLG_BOX_MSK%, OBJ_FLG_BOX_SHFT%, _wm_boxRel%
  box_cornerX%(_wm_moveBox%) = box_cornerX%(_wm_boxRel%) + obj_posX%(objId%) 
  box_cornerY%(_wm_moveBox%) = box_cornerY%(_wm_boxRel%) + obj_posY%(objId%) 
  box_width%(_wm_moveBox%) = box_width%(_wm_boxRel%)
  box_height%(_wm_moveBox%) = box_height%(_wm_boxRel%)

  obj_posX%(objId%) = xx% 'move object
  obj_posY%(objId%) = yy% 'move object

  'LM_writeLog("Moving object "+STR$(objId%)+" to "+HEX$(xx%)+","+HEX$(yy%))
    
  'If was in bounds but not anymore...
  '  LM_writeLog("bib b4 obj: "+STR$(objId%))
  '  LM_writeLog("bibx b4 obj: "+ STR$(box_cornerx%(_wm_moveBox%)) + " " + STR$(box_width%(_wm_moveBox%)))
  '  LM_writeLog("biby b4 obj: "+ STR$(box_cornery%(_wm_moveBox%)) + " " + STR$(box_height%(_wm_moveBox%)))
  '  LM_writeLog("bibx b4 view: "+ STR$(box_cornerx%(wm_view%)) + " " + STR$(box_width%(wm_view%)))
  '  LM_writeLog("biby b4 view: "+ STR$(box_cornery%(wm_view%)) + " " + STR$(box_height%(wm_view%)))
  
  boxIntersectBox(_wm_moveBox%, wm_view%, _wm_moveBoxIntersect%)
  IF _wm_moveBoxIntersect% THEN
    'Also compensate for view movement here.
    box_cornerX%(_wm_moveBox%) = box_cornerX%(_wm_boxRel%) + xx% - wm_viewVelX%
    box_cornerY%(_wm_moveBox%) = box_cornerY%(_wm_boxRel%) + yy% - wm_viewVelY%
    
    boxIntersectBox(_wm_moveBox%, wm_view%, _wm_moveBoxIntersect%)
    IF NOT _wm_moveBoxIntersect% THEN
      'LM_writeLog("Obj Out EVT")
      dummy_i% = Obj_eventHandler%(objId%, EVT_OUT_create%())
    ENDIF
  ENDIF
END FUNCTION

'Set View dimensions. .4 subpixel units.
SUB WM_setViewDim(ww%, hh%)
  box_width%(wm_view%) = ww%
  box_height%(wm_view%) = hh%
END SUB

'Get View dimensions. .4 subpixel units.
SUB WM_getViewDim(ww%, hh%)
  ww% = box_width%(wm_view%)
  hh% = box_height%(wm_view%)
END SUB

'Return view position (upper left corner). .4 subpixel units.
SUB WM_getViewPos(xx%, yy%)
  xx% = box_cornerX%(wm_view%)
  yy% = box_cornerY%(wm_view%)
END SUB

'Set view to given position (upper left corner)
'View edge will not go beyond world boundary, so clamping might occur.
'Updates Tile Engine view as well.
'Returns a bitmask: WM_X_CLAMP_MSK = x clamped, bit 1 = Y_CLAMP_MSK = y clamped.
FUNCTION WM_setViewPos%(viewX%, viewY%)
  WM_setViewPos% = 0
  
  LOCAL xx%=viewX%, yy%=viewY%
  'Make sure horizontal not out of world position
  LOCAL view_hor% = box_width%(wm_view%)
  LOCAL bound_hor% = box_width%(wm_boundary%)
  IF xx% + view_hor% > bound_hor% THEN
    xx% = bound_hor% - view_hor%
    WM_setViewPos% = WM_setViewPos% OR WM_X_CLAMP_MSK% 'x clamp
  ENDIF
  IF xx% < 0 THEN
    xx% = 0
    WM_setViewPos% = WM_setViewPos% OR WM_X_CLAMP_MSK% 'x clamp    
  ENDIF
  
  'Make sure vertical not out of world position
  LOCAL view_ver% = box_height%(wm_view%)
  LOCAL bound_ver% = box_height%(wm_boundary%)
  IF yy% + view_ver% > bound_ver% THEN
    yy% = bound_ver% - view_ver%
    WM_setViewPos% = WM_setViewPos% OR WM_Y_CLAMP_MSK 'y clamp        
  ENDIF
  IF yy% < 0 THEN
    yy% = 0
    WM_setViewPos% = WM_setViewPos% OR WM_Y_CLAMP_MSK 'y clamp        
  ENDIF
  
  'Set view
  box_cornerX%(wm_view%) = xx%
  box_cornerY%(wm_view%) = yy%
  
  'LM_writeLog("wm_view pos = "+HEX$(xx)+" "+HEX$(yy%))

  'Match tile view with camera
  IF te_enabled% THEN
    TE_tilePage xx%, yy%
  ENDIF
END FUNCTION

