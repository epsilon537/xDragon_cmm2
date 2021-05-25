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

CONST CG_SOLID% = CG_ID_1%
CONST CG_SOLID_MSK% = CG_ID_1_MSK%

cg_maps%(CG_SOLID%) = CG_ID_TILES_MSK%

te_enabled% = 1

PAL_clr
PAL_load("../resources/xDragonDemo.pal")
PAL_allocStdColors
PAL_commitToCLUT

BG_load("../resources/background.bmp")

Sheet_load("../resources/cave_tiles.bmp")

TE_loadTileMap("../resources/xDragon_Tile_Layer.csv", "cave_tiles")
TE_loadColMap("../resources/xDragon_Col_Layer.csv")
TE_loadTrigMap("../resources/xDragon_Trigger_Layer.csv")

DIM mapWidth%, mapHeight%
TE_getMapDim(mapWidth%, mapHeight%)
IF mapWidth% <> 128 THEN
  ERROR "_TE_loadMap error. Incorrect map width."
ENDIF

IF mapHeight% <> 16 THEN
  ERROR "_TE_loadMap error. Incorrect map height."
ENDIF

Sheet_load("../resources/spritesheet.BMP")

Anim_load("../resources/enemy.csv", "Enemy")
Anim_load("../resources/ship.csv", "Ship")

TE_matchWorldWithMap 1, 1

INC box_width%(te_trigMapView%), 4

'Using an offset that's not a tile multiple.
dummy_i% = WM_setViewPos%((MM.HRES+8)*16,0)

DIM dxm% =DXM_create%()

DIM shipObj% = TestObj_create%()
Obj_registerInterest(shipObj%, EVT_KBD%)
Obj_setSolidness(shipObj%, OBJ_HARD%)
Obj_setColGroup(shipObj%, CG_SOLID%)
Obj_setAnim(shipObj%, "Ship")

obj_posX%(shipObj%) = 16*(MM.HRES+MM.HRES*2/3)
obj_posY%(shipObj%) = 16*MM.VRES/2

te_trigMapEnabled%=1

bg_enabled%=1
BG_setPos 0, 0

GM_run

LM_writeLog("Test Complete.")

GM_shutdown

END

FUNCTION DXM_create%()
  LOCAL obj_id% = Obj_create%()
  Obj_setType(obj_id%, OBJ_TYPE_DXM%)
  Obj_registerInterest(obj_id%, EVT_STEP%)
  Obj_registerInterest(obj_id%, EVT_KBD%)  
  Obj_registerInterest(obj_id%, EVT_TE_TRIG%)
  obj_drawSub$(obj_id%) = "Dummy_sub_i"
  obj_eventH$(obj_id%) = "DXM_eventHandler%"
  obj_destroySub$(obj_id%) = "DXM_destroy"
  DXM_create% = obj_id%
END FUNCTION

SUB DXM_destroy(obj_id%)
  Obj_defaultDestroy obj_id%
  PRINT "DXM destroyed: "+STR$(obj_id%)
END SUB

FUNCTION DXM_eventHandler%(obj_id%, ev%)
  IF EVT_getType%(ev%) = EVT_STEP% THEN
    bg_velX% = wm_viewVelX%*0.5
    bg_velY% = wm_viewVelY%*0.5  
  ENDIF

  IF EVT_getType%(ev%) = EVT_TE_TRIG% THEN  
    LOCAL tileX%, tileY%, trigId%
    EVT_TE_TRIG_get(ev%, tileX%, tileY%, trigId%)
    LM_writeLog("TE Trigger: ("+STR$(tileX%)+","+STR$(tileY%)+"): "+STR$(trigId%))

    LOCAL enemyObj% = TestObj_create%()

    Obj_setSolidness(enemyObj%, OBJ_HARD%)
    Obj_setColGroup(enemyObj%, CG_SOLID%)
    Obj_setAnim(enemyObj%, "Enemy")
    
    obj_posX%(enemyObj%) = tileX%*TE_TILE_SUBPIXEL_W%
    obj_posY%(enemyObj%) = tileY%*TE_TILE_SUBPIXEL_H%
  ENDIF
    
  IF EVT_getType%(ev%) = EVT_KBD% THEN
    LOCAL action% = EVT_KBD_getAction%(ev%)
    
    LOCAL key% = EVT_KBD_getKey%(ev%)
  
    IF action% = EVT_KBD_ACT_KEY_PRESSED% THEN
      IF key%=Q_KEY% THEN
        GM_setGameOver(1)
      ENDIF    

      IF key%=D_KEY% THEN
        INC wm_viewVelX%, 4
      ENDIF
      IF key%=A_KEY% THEN
        INC wm_viewVelX%, -4
      ENDIF
      IF key%=W_KEY% THEN
        INC wm_viewVelY%, -4
      ENDIF
      IF key%=S_KEY% THEN
        INC wm_viewVelY%, 4        
      ENDIF
      IF key%=T_KEY% THEN
        TE_setMapMask(127, 15)
        LM_writeLog("te_mapXYmask = (127,15)")

        TE_matchWorldWithMap 2, 2

        dm_shakeSeverity%=10 'Screen shake severity in pixels
        dm_shakeDuration%=20 'Screen shake duration in frames
      ENDIF
      IF key%=U_KEY% THEN
        TE_setMapMask(-1, -1)
        LM_writeLog("te_mapXYmask = (-1,-1)")
        TE_matchWorldWithMap 1, 1
      ENDIF          
    ENDIF
  ENDIF

  DXM_eventHandler% = 1  
END FUNCTION

FUNCTION TestObj_create%()
  LOCAL obj_id% = Obj_create%()
  Obj_setType(obj_id%, OBJ_TYPE_TEST%)
  Obj_registerInterest(obj_id%, EVT_COL%)
  obj_eventH$(obj_id%) = "TestObj_eventHandler%"
  obj_destroySub$(obj_id%) = "TestObj_destroy"
  TestObj_create% = obj_id%
END FUNCTION

SUB TestObj_destroy(obj_id%)
  Obj_defaultDestroy obj_id%
  PRINT "TestObject destroyed: "+STR$(obj_id%)
END SUB

FUNCTION TestObj_eventHandler%(obj_id%, ev%)

  IF EVT_getType%(ev%) = EVT_COL% THEN
    LOCAL x%, y%
    EVT_COL_getPos(ev%, x%, y%)
    
    LM_writeLog("Collision! at: "+STR$(x%)+" "+STR$(y%))
    
    IF EVT_COL_getObj2%(ev%) <> EVT_COL_OBJ2_IS_TILE% THEN
      ERROR "EvtCol obj2 error"
    ENDIF
  ENDIF
  
  IF EVT_getType%(ev%) = EVT_KBD% THEN
    LOCAL action% = EVT_KBD_getAction%(ev%)
    
    LOCAL key% = EVT_KBD_getKey%(ev%)
  
    IF action% = EVT_KBD_ACT_KEY_PRESSED% THEN      
      IF key% = RIGHT_KEY% THEN
        INC obj_velX%(obj_id%), 4
      ENDIF
      IF key% = LEFT_KEY% THEN
        INC obj_velX%(obj_id%), -4
      ENDIF
      IF key% = UP_KEY% THEN
        INC obj_velY%(obj_id%), -4
      ENDIF
      IF key% = DOWN_KEY% THEN
        INC obj_velY%(obj_id%), 4        
      ENDIF
      
      IF key% = SPC_KEY% THEN
        obj_velX%(obj_id%) = 0
        obj_velY%(obj_id%) = 0          
      ENDIF
    ENDIF
  ENDIF
      
  TestObj_eventHandler% = 1
END FUNCTION

