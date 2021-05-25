'Event Base functionality

'Public constants
'----------------
'Event Object Bitfield definitions
#define "EVT_TYPE_MSK%","1023"
#define "EVT_TYPE_SHFT%","0"
#define "EVT_DATA_MSK%","((-1)<<10)"
#define "EVT_DATA_SHFT%","10"

'Public subs and functions
'-------------------------

'Event base object is a value type.
FUNCTION EVT_create%()
  EVT_create% = EVT_UNDEFD%
END FUNCTION

'Destroy given event object, releasing all resources used by this object.
'Even though the Event base type is a value type, it's derived types, such as collision events,
'might not be, so a destroy function is necessary.
SUB EVT_destroy(ev%)
  'Poor man's virtual destructor
  LOCAL evt_type% = ev% AND EVT_TYPE_MSK%
  
  SELECT CASE evt_type%
    CASE EVT_COL%
      EVT_COL_destroy(ev%)
  END SELECT
END SUB

'Set Event Type. See EventTypes.inc.
SUB EVT_setType(ev%, type%)
  setBitField ev%, EVT_TYPE_MSK%, EVT_TYPE_SHFT%, type%
END SUB

'Get Event Type. See EventTypes.inc.
FUNCTION EVT_getType%(ev%)
  getBitField ev%, EVT_TYPE_MSK%, EVT_TYPE_SHFT%, EVT_getType%
END FUNCTION

'Set data portion of event object.
SUB EVT_setData(ev%, dat%)
  setBitField ev%, EVT_DATA_MSK%, EVT_DATA_SHFT%, dat%
END SUB

'Get data portion of event object.
FUNCTION EVT_getData%(ev%)
  getBitField ev%, EVT_DATA_MSK%, EVT_DATA_SHFT%, EVT_getData%
END FUNCTION

