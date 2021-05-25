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

DIM obj1% = Obj_create%()
DIM obj2% = Obj_create%()

DIM lid% = ObjLst_create%()
WM_getAllObjs(lid%, 0)

IF NOT ObjLst_eq%(lid%, sg_activesLid%) THEN
  ERROR "WM_getAllObjs error."
ENDIF

DIM index%=0
IF objLst_list%(index%, lid%) <> obj1% THEN
  ERROR "SceneGraph active obj error."
ENDIF
INC index%
IF objLst_list%(index%, lid%) <> obj2% THEN
  ERROR "SceneGraph active obj error."
ENDIF
INC index%
IF index% <> objLst_numElems%(lid%) THEN
  ERROR "SceneGraph active obj error."
ENDIF

DO WHILE index% < objLst_numElems%(lid%)
  cur_obj_id% = objLst_list%(index%, lid%)

  IF Obj_getType%(cur_obj_id%) = obj_type% THEN
    ObjLst_insert(lid%, cur_obj_id%)
  ENDIF
  
  INC index%
LOOP

SG_removeObj(obj1%)

index%=0
IF objLst_list%(index%, sg_activesLid%) <> obj2% THEN
  ERROR "SceneGraph active obj error."
ENDIF
INC index%
IF index% <> objLst_numElems%(sg_activesLid%) THEN
  ERROR "SceneGraph active obj error."
ENDIF

SG_removeObj(obj2%)

SG_insertObj(obj1%)
SG_insertObj(obj2%)

Obj_setColGroup(obj1%, 1)
Obj_setAlt(obj1%, 1)

Obj_setColGroup(obj2%, 2)
Obj_setAlt(obj2%, 2)

index%=0
IF objLst_list%(index%, sg_collidablesLid%(1)) <> obj1% THEN
  ERROR "SceneGraph collidable obj error."
ENDIF
INC index%
IF index% <> objLst_numElems%(sg_collidablesLid%(1)) THEN
  ERROR "SceneGraph collidable obj error."
ENDIF

index%=0
IF objLst_list%(index%, sg_collidablesLid%(2)) <> obj2% THEN
  ERROR "SceneGraph solid obj error."
ENDIF
INC index%
IF index% <> objLst_numElems%(sg_collidablesLid%(2)) THEN
  ERROR "SceneGraph solid obj error."
ENDIF

index%=0
IF objLst_list%(index%, sg_visiblesLid%(1)) <> obj1% THEN
  ERROR "SceneGraph visible obj error."
ENDIF
INC index%
IF index% <> objLst_numElems%(sg_visiblesLid%(1)) THEN
  ERROR "SceneGraph solid obj error."
ENDIF

index%=0
IF objLst_list%(index%, sg_visiblesLid%(2)) <> obj2% THEN
  ERROR "SceneGraph visible obj error."
ENDIF
INC index%
IF index% <> objLst_numElems%(sg_visiblesLid%(2)) THEN
  ERROR "SceneGraph solid obj error."
ENDIF

SG_removeObj(obj2%)
SG_removeObj(obj1%)

IF NOT ObjLst_isEmpty%(sg_visiblesLid%(1)) THEN
  ERROR "SceneGraph visible obj error"
ENDIF

IF NOT ObjLst_isEmpty%(sg_visiblesLid%(2)) THEN
  ERROR "SceneGraph visible obj error"
ENDIF

IF NOT ObjLst_isEmpty%(sg_collidablesLid%(2)) THEN
  ERROR "SceneGraph collidable obj error"
ENDIF

IF NOT ObjLst_isEmpty%(sg_collidablesLid%(1)) THEN
  ERROR "SceneGraph collidable obj error"
ENDIF

IF NOT ObjLst_isEmpty%(sg_activesLid%) THEN
  ERROR "SceneGraph active obj error"
ENDIF

GM_shutDown

PRINT "Test Done."
END

