'Public subs and functions
'-------------------------

'Step event is a value type. Step events are generate each frame.

'Create step event object.
FUNCTION EVT_STEP_create%(stepCount%)
  EVT_STEP_create% = EVT_STEP% OR (stepCount% << EVT_DATA_SHFT%)
END FUNCTION

'Set the step (frame) count in the step event object.
SUB EVT_STEP_setStepCount(ev%, stepCount%)
  setBitField ev%, EVT_DATA_MSK%, EVT_DATA_SHFT%, stepCount%
END SUB

'Get the step (frame) count in the step event object.
FUNCTION EVT_STEP_getStepCount%(ev%)
  getBitField ev%, EVT_DATA_MSK%, EVT_DATA_SHFT%, EVT_STEP_getStepCount%
END FUNCTION
