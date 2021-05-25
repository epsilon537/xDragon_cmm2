'SceneGraph is a singleton

'Read-Only variables
'-------------------
DIM sg_activesLid% 'Active objects list id
DIM sg_inactivesLid% 'Inactive objects list id
DIM sg_collidablesLid%(OBJ_MAX_COLLISION_GROUP%) 'Collidable objects per collision group.
DIM sg_visiblesLid%(OBJ_MAX_ALTITUDE%) 'Visible objects per altitude

DIM sg_started%=0 'Flag indicating if SceneGraph module is started.

'Public subs and functions
'-------------------------

'Start the SceneGraph module. Done once at start of Game Engine.
SUB SG_startUp
  sg_activesLid% = ObjLst_create%()
  sg_inactivesLid% = ObjLst_create%()
    
  LOCAL ii%
  FOR ii%=0 TO OBJ_MAX_ALTITUDE%
    sg_visiblesLid%(ii%) = ObjLst_create%()
  NEXT ii%
  
  FOR ii%=0 TO OBJ_MAX_COLLISION_GROUP%
    sg_collidablesLid%(ii%) = ObjLst_create%()
  NEXT ii%

  sg_started% = 1
END SUB

'Shut down SceneGraph module.
SUB SG_shutDown
  ObjLst_destroy(sg_activesLid%)
  ObjLst_destroy(sg_inactivesLid%)
  
  LOCAL ii%
  FOR ii%=0 TO OBJ_MAX_ALTITUDE%
    ObjLst_destroy sg_visiblesLid%(ii%)
  NEXT ii%
  
  FOR ii%=0 TO OBJ_MAX_COLLISION_GROUP%
    ObjLst_destroy sg_collidablesLid%(ii%)
  NEXT ii%
  
  sg_started% = 0
END SUB

'Insert Object into SceneGraph.
SUB SG_insertObj(objId%)
  'Add to active or inactive list
  IF Obj_isActive%(objId%) THEN
    ObjLst_insert sg_activesLid%, objId%

    'Add to appropriate collision group
    ObjLst_insert sg_collidablesLid%(Obj_getColGroup%(objId%)), objId%
    
    'Add to visible objects at right altitude.
    ObjLst_insert sg_visiblesLid%(Obj_getAlt%(objId%)), objId%
  ELSE
    ObjLst_insert sg_inactivesLid%, objId%
  ENDIF  
END SUB

'Remove Object from SceneGraph.
SUB SG_removeObj(objId%)
  'Remove from active or inactive list
  IF Obj_isActive%(objId%) THEN
    IF ObjLst_remove%(sg_activesLid%, objId%) = -1 THEN
      ERROR "SG_removeObj: Failed to remove object "+STR$(objId%)+" from SceneGraph active objects list."
    ENDIF

    'Remove from collidable objects list.
    IF ObjLst_remove%(sg_collidablesLid%(Obj_getColGroup%(objId%)), objId%) = -1 THEN
      ERROR "SG_removeObj: Failed to remove object "+STR$(objId%)+" from SceneGraph collidable objects list."
    ENDIF
    
    'Remove from visible objects at right altitude.
    IF ObjLst_remove%(sg_visiblesLid%(Obj_getAlt%(objId%)), objId%) = -1 THEN
      ERROR "SG_removeObj: Failed to remove object "+STR$(objId%)+" from SceneGraph visible objects list."
    ENDIF
  ELSE
    IF ObjLst_remove%(sg_inactivesLid%, objId%) = -1 THEN
      ERROR "SG_removeObj: Failed to remove object "+STR$(objId%)+" from SceneGraph inactive objects list."
    ENDIF
  ENDIF
END SUB

'Re-position Object in SceneGraph to new altitude.
SUB SG_updateAlt(objId%, alt%)
  'Remove from old altitude.
  IF ObjLst_remove%(sg_visiblesLid%(Obj_getAlt%(objId%)), objId%) = -1 THEN
    ERROR "Failed to remove object from SceneGraph visible objects list"
  ENDIF

  'Add to new altitude.
  ObjLst_insert sg_visiblesLid%(alt%), objId%    
END SUB

'Re-position Object in SceneGraph to new collision group
SUB SG_updateColGroup(objId%, colGroup%)
  'Remove from old group.
  IF ObjLst_remove%(sg_collidablesLid%(Obj_getColGroup%(objId%)), objId%) = -1 THEN
    ERROR "SG_removeObj: Failed to remove object "+STR$(objId%)+" from SceneGraph collidable objects list."
  ENDIF

  'Add to new group.
  ObjLst_insert sg_collidablesLid%(colGroup%), objId%    
END SUB

'Re-position Object in SceneGraph to new activeness.
SUB SG_updateActive(objId%, activeVal%)
  'If activeness unchanged, nothing to do.
  LOCAL isActive% = Obj_isActive%(objId%)
  
  IF isActive% = activeVal% THEN
    EXIT SUB
  ENDIF
  
  'If was active, then now inactive, so remove from lists.
  IF isActive% THEN
    IF ObjLst_remove%(sg_activesLid%, objId%) = -1 THEN
      ERROR "Failed to remove object from SceneGraph active objects list"
    ENDIF
    
    IF ObjLst_remove%(sg_visiblesLid%(Obj_getAlt%(objId%)), objId%) = -1 THEN
      ERROR "Failed to remove object from SceneGraph visible objects list"
    ENDIF
    
    IF ObjLst_remove%(sg_collidablesLid%(Obj_getColGroup%(objId%)), objId%) = -1 THEN
      ERROR "SG_removeObj: Failed to remove object "+STR$(objId%)+" from SceneGraph collidable objects list."
    ENDIF
    
    'Add to inactive list.
    ObjLst_insert sg_inactivesLid%, objId% 
  
  ELSE 'Was inactive, so add to lists
    ObjLst_insert sg_activesLid%, objId%'

    'Add to collidable objects list.
    ObjLst_insert sg_collidablesLid%(Obj_getColGroup%(objId%)), objId%

    'Add to visible objects at right altitude.
    ObjLst_insert sg_visiblesLid%(Obj_getAlt%(objId%)), objId%

    'Remove from inactive list.
    IF ObjLst_remove%(sg_inactivesLid%, objId%) = -1 THEN
      ERROR "Failed to remove object from SceneGraph inactive objects list"
    ENDIF      
  ENDIF
END SUB

'Clear all objects from the SceneGraph
SUB SG_clr
  ObjLst_clr sg_activesLid%
  ObjLst_clr sg_inactivesLid%

  LOCAL ii%
  FOR ii%=0 TO OBJ_MAX_ALTITUDE%
    ObjLst_clr sg_visiblesLid%(ii%)
  NEXT ii%  
  
    FOR ii%=0 TO OBJ_MAX_COLLISION_GROUP%
    ObjLst_clr sg_collidablesLid%(ii%)
  NEXT ii%  
END SUB

