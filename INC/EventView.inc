'Private variables and constants
'-------------------------------

'Event type is in the first 10 bits
CONST _EVT_VIEW_TAG_MSK% = 1023<<10
CONST _EVT_VIEW_TAG_SHFT% = 10
CONST _EVT_VIEW_VAL_MSK% = &HFFFFFF<<20
CONST _EVT_VIEW_VAL_SHFT% = 20
CONST _EVT_VIEW_DELTA_MSK% = 1<<44
CONST _EVT_VIEW_DELTA_SHFT% = 44

'Public subs and functions
'-------------------------
'View Events are events sent to ViewObjects, e.g. to update points.

'EVT_VIEW is a value type. tag is a tag to associate with the event.
'value is associated with event and can be negative.
'delta is a boolean indicating if value is a delta or a replace value.
FUNCTION EVT_VIEW_create%(tag%, value%, delta%)
  EVT_VIEW_create% = EVT_VIEW% OR (tag%<<_EVT_VIEW_TAG_SHFT%) OR ((value%<<_EVT_VIEW_VAL_SHFT%) AND _EVT_VIEW_VAL_MSK%) OR (delta%<<_EVT_VIEW_DELTA_SHFT%)
END FUNCTION

'Set View Event tag to new tag.
SUB EVT_VIEW_setTag(ev%, tag%)
  ev% = (ev% AND (INV _EVT_VIEW_TAG_MSK%)) OR (tag%<<_EVT_VIEW_TAG_SHFT%)
END SUB

'Get View Event tag.
FUNCTION EVT_VIEW_getTag%(ev%)
  EVT_VIEW_getTag% = (ev% AND _EVT_VIEW_TAG_MSK%) >> _EVT_VIEW_TAG_SHFT%
END FUNCTION

'Set View Event value to new value. Value can be negative.
SUB EVT_VIEW_setValue(ev%, value%)
  ev% = (ev% AND (INV _EVT_VIEW_VAL_MSK%)) OR ((value%<<_EVT_VIEW_VAL_SHFT%) AND _EVT_VIEW_VAL_MSK%)
END SUB

'Get View Event value. Can be negative.
FUNCTION EVT_VIEW_getValue%(ev%)
  'Shift all the way to the left, then all the way to the right for sign extension.
  EVT_VIEW_getValue% = (ev%<<20) >>> 40
END FUNCTION

'Set View Event delta flag to new value (1 or 0)
SUB EVT_VIEW_setDelta(ev%, delta%)
  ev% = (ev% AND (INV _EVT_VIEW_DELTA_MSK%)) OR (delta%<<_EVT_VIEW_DELTA_SHFT%)
END SUB

'Get View Event delta flag
FUNCTION EVT_VIEW_getDelta%(ev%)
  EVT_VIEW_getDelta% = (ev% AND _EVT_VIEW_DELTA_MSK%) >> _EVT_VIEW_DELTA_SHFT%
END FUNCTION



