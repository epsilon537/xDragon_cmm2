OPTION EXPLICIT
OPTION DEFAULT NONE
OPTION BASE 0
OPTION CONSOLE BOTH

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

DIM objLid% = ObjLst_create%()

IF ObjLst_isAllocated%(objLid%) = 0 THEN
  Error "ObjList not allocated."
ENDIF

IF objLst_numElems%(objLid%) <> 0 THEN
  Error "ObjList numElems incorrect."
ENDIF

IF NOT ObjLst_isEmpty%(objLid%) THEN
  Error "ObjList isEmpty error."
ENDIF

IF ObjLst_isFull%(objLid%) THEN
  Error "ObjList isFull error."
ENDIF

DIM obj1% = Obj_create%()

ObjLst_insert(objLid%, obj1%)

IF ObjLst_isEmpty%(objLid%) THEN
  Error "ObjList isEmpty error (2)."
ENDIF

IF ObjLst_isFull%(objLid%) THEN
  Error "ObjList isFull error.(2)"
ENDIF

IF objLst_numElems%(objLid%) <> 1 THEN
  Error "ObjList numElems incorrect (2)."
ENDIF

DIM obj2% = Obj_create%()
DIM obj3% = Obj_create%()

ObjLst_insert(objLid%, obj2%)
ObjLst_insert(objLid%, obj3%)

IF ObjLst_isEmpty%(objLid%) THEN
  Error "ObjList isEmpty error (3)."
ENDIF

IF ObjLst_isFull%(objLid%) THEN
  Error "ObjList isFull error.(3)"
ENDIF

IF objLst_numElems%(objLid%) <> 3 THEN
  Error "ObjList numElems incorrect (3)."
ENDIF

DIM objLid2% = ObjLst_create%()

ObjLst_copy(objLid%, objLid2%) 'From objLid to objLid2
IF objLst_numElems%(objLid2%) <> 3 THEN
  Error "ObjList numElems incorrect (4)."
ENDIF

IF NOT ObjLst_eq%(objLid%, objLid2%) THEN
  Error "ObjList copy or compare error."
ENDIF

DIM res% = ObjLst_remove%(objLid%, obj2%)
IF res% = -1 THEN
  Error "ObjList remove error."
ENDIF

IF objLst_numElems%(objLid%) <> 2 THEN
  Error "ObjList numElems incorrect (4)."
ENDIF

IF ObjLst_eq%(objLid%, objLid2%) THEN
  Error "ObjList compare error (2)."
ENDIF

res% = ObjLst_remove%(objLid%, obj2%)
IF res% <> -1 THEN
  Error "ObjList remove error (2)."
ENDIF

ObjLst_append(objLid2%, objLid%)
IF objLst_numElems%(objLid2%) <> 5 THEN
  Error "ObjList numElems incorrect (5)."
ENDIF

IF ObjLst_isEmpty%(objLid2%) THEN
  Error "ObjList isEmpty error (5)."
ENDIF

IF ObjLst_isFull%(objLid2%) THEN
  Error "ObjList isFull error.(5)"
ENDIF

ObjLst_clr(objLid%)
IF NOT ObjLst_isEmpty%(objLid%) THEN
  Error "ObjList isEmpty error (6)."
ENDIF

DIM ii%=0
DIM objX%
DIM objs%(OBJLST_MAX_SIZE%-1)

DO WHILE ii% < OBJLST_MAX_SIZE%-3
  objX% = Obj_create%()
  objs%(ii%) = objX%
  ObjLst_insert(objLid%, objX%)
  INC ii%
LOOP

DIM numObjsInList% = ii%

'IF NOT ObjLst_isFull%(objLid%) THEN
'  Error "ObjList isFull error.(6)"
'ENDIF

IF objLst_numElems%(objLid%) <> numObjsInList% THEN
  Error "ObjList numElems incorrect (6)."
ENDIF

ii%=0
DO WHILE ii% < numObjsInList%
  objX% = objs%(ii%)
  res% = ObjLst_remove%(objLid%, objX%)
  IF res% = -1 THEN
    Error "ObjList remove error (3), ii="+STR$(ii%)+" obj id="+STR$(objX%)
  ENDIF
  INC ii%
LOOP

IF NOT ObjLst_isEmpty%(objLid%) THEN
  Error "ObjList isEmpty error (7)."
ENDIF

ObjLst_destroy(objLid%)
ObjLst_destroy(objLid2%)

IF ObjLst_isAllocated%(objLid%) THEN
  Error "ObjList not allocated.(2)"
ENDIF

IF ObjLst_isAllocated%(objLid2%) THEN
  Error "ObjList not allocated.(3)"
ENDIF

Obj_destroy(obj1%)
Obj_destroy(obj2%)
Obj_destroy(obj3%)

ii%=0
DO WHILE ii% < numObjsInList%
  Obj_destroy(objs%(ii%))
  INC ii%
LOOP

'IF objLst_numAllocated%<> objLst_numAllocated% THEN
'  PRINT "Num ObjLists allocated: "+STR$(objLst_numAllocated%)
'  Error "ObjectList leak."
'ENDIF

IF obj_numAllocated%<> 0 THEN
  PRINT "Num Objs allocated: "+STR$(obj_numAllocated%)
  Error "Object leak."
ENDIF

GM_shutdown

PRINT "Test Completed."

END

