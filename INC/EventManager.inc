'Private variables and constants
'-------------------------------
'Per-event-type object lists
DIM _evt_mgr_evObjLists%(MAX_NUM_EVT_TYPES%-1)
DIM _evt_mgr_evtType%, _evt_mgr_onEvtLid%

'Public subs and functions
'-------------------------

'Start Event Manager. Done once at start-up of the game engine.
SUB EVT_MGR_startUp
  LOCAL ii%
  FOR ii%=0 TO MAX_NUM_EVT_TYPES%-1
    _evt_mgr_evObjLists%(ii%) = ObjLst_create%()
  NEXT ii%
END SUB

'Shut down the event manager.
SUB EVT_MGR_shutDown
  LOCAL ii%
  FOR ii%=0 TO MAX_NUM_EVT_TYPES%-1
    ObjLst_destroy(_evt_mgr_evObjLists%(ii%))
  NEXT ii%
END SUB

'Object indicates interest in event.
SUB EVT_MGR_registerInterest(obj_id%, evType%)
  ObjLst_insert(_evt_mgr_evObjLists%(evType%), obj_id%)
  
  LOCAL lid% = _evt_mgr_evObjLists%(evType%)
END SUB

'Object indicates no longer interested in event
'Returns 0 if success, -1 if object not found
FUNCTION EVT_MGR_unregisterInterest%(obj_id%, evType%)
  EVT_MGR_unregisterInterest% = ObjLst_remove%(_evt_mgr_evObjLists%(evType%), obj_id%)
END FUNCTION

'Returns count of number of events sent.
FUNCTION EVT_MGR_onEvent%(ev%)
  LOCAL index%, objId%
  'EVT getType
  getBitField ev%, EVT_TYPE_MSK%, EVT_TYPE_SHFT%, _evt_mgr_evtType%
  _evt_mgr_onEvtLid% = _evt_mgr_evObjLists%(_evt_mgr_evtType%)
  
  FOR index%=0 TO objLst_numElems%(_evt_mgr_onEvtLid%) - 1 'Yes, this works if numElems = 0
    objId% = objLst_list%(index%, _evt_mgr_onEvtLid%)
    IF (obj_flags%(objId%) AND OBJ_FLG_ACT_MSK%) <> 0 THEN: dummy_i% = Obj_eventHandler%(objId%, ev%): ENDIF
  NEXT index%
  
  EVT_MGR_onEvent% = index%
END FUNCTION

