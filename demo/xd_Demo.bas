OPTION EXPLICIT
OPTION DEFAULT NONE
OPTION BASE 0
OPTION CONSOLE SERIAL

'Game Engine includes
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

'Demo Game includes
#INCLUDE "xd_ObjectTypes.inc"
#INCLUDE "xd_ViewEventTags.inc"
#INCLUDE "xd_EventTypes.inc"
#INCLUDE "xd_Enemy.inc"
#INCLUDE "xd_Explosion.inc"
#INCLUDE "xd_Bullet.inc"
#INCLUDE "xd_Ship.inc"
#INCLUDE "xd_Points.inc"
#INCLUDE "xd_Intro.inc"
#INCLUDE "xd_GameOver.inc"
#INCLUDE "xd_ColGroups.inc"
#INCLUDE "xd_Sounds.inc"

'Command line handling. Currently just used to turn on the loop time display.
CONST MAX_NUM_CMDLINE_ARGS% = 20
DIM cmdLineArgs$(MAX_NUM_CMDLINE_ARGS%)
DIM nArgs%, argIdx%

parseCmdLine(MM.CMDLINE$, cmdLineArgs$(), nArgs%)

FOR argIdx%=0 TO nArgs%-1
  SELECT CASE UCASE$(cmdLineArgs$(argIdx%))
    CASE "L"
      gm_displayLoopTime%=1
  END SELECT
NEXT argIdx%

GM_startUp 'Start game engine.

xd_ColGroupsInit 'Set up collision groups.

loadResources 'Load game resources.

xd_Ship_init 'Initialize ship module (not ship object constructor. That comes later).

'Create the Intro Object
dummy_i% = xd_Intro_create%()

GM_run 'Start the Game Loop. We stay in this loop until GM_setGameOver() is called.

GM_shutDown 'Shutdown game engine.
END

'Load the game specific resources used by this demo game.
SUB loadResources
  'Set up the palette
  PAL_clr
  'This is a palette file in JASC file format, generated by GraphicsGale.
  PAL_load("../resources/xDragonDemo.pal") 
  PAL_allocStdColors 'Add standard colors to the loaded palette (xDragonDemo.pal does not use all 256 colors).
  PAL_commitToCLUT 'Install the palette in the CLUT.

  'Load the background image. This is wraparound tileable image.
  BG_load("../resources/background.bmp")

  'Load the tile sheet used by the Tile Engine
  Sheet_load("../resources/cave_tiles.bmp")

  'Load the Tile Map, Collision Map and Trigger Map, all created using the Tiled Level Editor.
  'Map containing the position of the cave tiles.
  TE_loadTileMap("../resources/xDragon_Tile_Layer.csv", "cave_tiles")
  'Map specifying which tiles generate collisions.
  TE_loadColMap("../resources/xDragon_Col_Layer.csv")
  'Map specifying where enemies spawn.
  TE_loadTrigMap("../resources/xDragon_Trigger_Layer.csv")
  
  'Set up wrap-around indexing into tile map. The demo game just keep scrolling (faster and faster).
  LOCAL mapW%, mapH%
  TE_getMapDim(mapW%, mapH%)
  TE_setMapMask(mapW%-1, mapH%-1) 'Set the map mask to match the map dimensions.

  'Match the world boundary with the TileMap, but 32 times wider.
  TE_matchWorldWithMap 32, 1
  
  'Make the triggerMapView 4 tiles wide than the screen to create some space for spawning enemies.
  INC box_width%(te_trigMapView%), 4

  'Load the Sprite Sheets used by the animations
  Sheet_load("../resources/spritesheet.BMP")
  Sheet_load("../resources/intro0000.BMP")
  Sheet_load("../resources/intro0001.BMP")
  Sheet_load("../resources/intro0002.BMP")
  Sheet_load("../resources/intro0003.BMP")
  Sheet_load("../resources/gameover0000.BMP")
  Sheet_load("../resources/gameover0001.BMP")
  Sheet_load("../resources/gameover0002.BMP")
  Sheet_load("../resources/gameover0003.BMP")
  Sheet_load("../resources/gameover0004.BMP")
  Sheet_load("../resources/gameover0005.BMP")
  Sheet_load("../resources/gameover0006.BMP")
  Sheet_load("../resources/gameover0007.BMP")
  
  'Load the Animation meta-data generated by GraphicsGale.
  Anim_load("../resources/enemy.csv", "Enemy")
  Anim_load("../resources/bullet.csv", "Bullet")
  Anim_load("../resources/ship.csv", "Ship")
  Anim_load("../resources/intro.csv", "Intro")
  Anim_load("../resources/gameover.csv", "GameOver")  
  Anim_load("../resources/explosion.csv", "Explosion")
END SUB

SUB parseCmdLine(cmdLine$, cmdLineArgs$(), nArgs%)
  LOCAL curPos%=1, startPos%
  LOCAL inWhiteSpace%=1
  LOCAL curArg%=0
  
  DO WHILE (curPos%<=LEN(cmdLine$)) AND (curArg%<MAX_NUM_CMDLINE_ARGS%)
    IF inWhiteSpace% THEN
      IF MID$(cmdLine$, curPos%, 1) <> " " THEN
        startPos% = curPos%
        inWhiteSpace% = 0
      ENDIF
    ELSE
      IF MID$(cmdLine$, curPos%, 1) = " " THEN
        cmdLineArgs$(curArg%) = MID$(cmdLine$, startPos%, curPos%-startPos%)
        INC curArg%
        inWhiteSpace% = 1
      ENDIF
    ENDIF
    INC curPos%
  LOOP
  
  IF (inWhiteSpace%=0) AND (curArg% < MAX_NUM_CMDLINE_ARGS%) THEN
    cmdLineArgs$(curArg%) = MID$(cmdLine$, startPos%)
    INC curArg%
  ENDIF
  
  nArgs% = curArg%
END SUB
