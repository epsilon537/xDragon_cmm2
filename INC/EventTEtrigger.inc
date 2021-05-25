'Public constants
'----------------

'TileEngine Trigger Event bitfield definitions.
CONST EVT_TE_TRIG_ID_MSK%=(&HFF<<42)
CONST EVT_TE_TRIG_ID_SHFT%=42
CONST EVT_TE_TRIG_TILEX_MSK%=(&HFFFF<<26)
CONST EVT_TE_TRIG_TILEX_SHFT%=26
CONST EVT_TE_TRIG_TILEY_MSK%=(&HFFFF<<10)
CONST EVT_TE_TRIG_TILEY_SHFT%=10

'Public subs and functions
'-------------------------

'Tile Trigger Events are placed in the Tile Map at the Trigger Layer.
'EVT_TE_TRIG is a value type object

'Create an Tile Engine Trigger Event Object with given params.
'tileX and Y are x and y indices in the tile map (not screen or world coordinates).
'trigId is the ID of the Tile Trigger Event.
FUNCTION EVT_TE_TRIG_create%(tileX%, tileY%, trigId%)
  EVT_TE_TRIG_create% = (trigId%<<EVT_TE_TRIG_ID_SHFT%) OR (tileX%<<EVT_TE_TRIG_TILEX_SHFT%) OR (tileY%<<EVT_TE_TRIG_TILEY_SHFT%) OR EVT_TE_TRIG%
END FUNCTION

'Retrieve TileEngine Trigger properties.
'TileX and Y are indices into the tile map. trigId is the ID of the Tile Trigger Event.
SUB EVT_TE_TRIG_get(ev%, tileX%, tileY%, trigId%)
  trigId% = (ev% AND EVT_TE_TRIG_ID_MSK%)>>EVT_TE_TRIG_ID_SHFT%
  tileX% = (ev% AND EVT_TE_TRIG_TILEX_MSK%)>>EVT_TE_TRIG_TILEX_SHFT%
  tileY% = (ev% AND EVT_TE_TRIG_TILEY_MSK%)>>EVT_TE_TRIG_TILEY_SHFT%
END SUB

'Get x-index of trigger event in the Tilemap
FUNCTION EVT_TE_TRIG_getTileX%(ev%)
  EVT_TE_TRIG_getTileX% = (ev% AND EVT_TE_TRIG_TILEX_MSK%)>>EVT_TE_TRIG_TILEX_SHFT%
END FUNCTION

'Get y-index of trigger event in the Tilemap
FUNCTION EVT_TE_TRIG_getTileY%(ev%)
  EVT_TE_TRIG_getTileY% = (ev% AND EVT_TE_TRIG_TILEY_MSK%)>>EVT_TE_TRIG_TILEY_SHFT%
END FUNCTION

'Get ID of the trigger event.
FUNCTION EVT_TE_TRIG_getID%(ev%)
  EVT_TE_TRIG_getID% = (ev% AND EVT_TE_TRIG_ID_MSK%)>>EVT_TE_TRIG_ID_SHFT%
END FUNCTION

