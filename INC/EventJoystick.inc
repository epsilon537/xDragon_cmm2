'Public constants
'----------------

'Set of recognized joystick actions
#define "EVT_JOY_ACT_UNDEFD%","0"
#define "EVT_JOY_ACT_CLICKED%","1"  'Button went from not pressed to pressed.
#define "EVT_JOY_ACT_PRESSED%","2"  'Button is currently pressed.
#define "EVT_JOY_ACT_MOVED%","3"    'Move action
#define "EVT_JOY_ACT_MOVE_END%","4" 'When the move action ends

'Set of recognize buttons
#define "EVT_JOY_BUTTON_UNDEFD%","0"
#define "EVT_JOY_BUTTON_Z%","1"
#define "EVT_JOY_BUTTON_C%","2"

'Private variables and constants
'-------------------------------

#define "_EVT_JOY_ACT_MSK%","(7<<10)" 'Event data starts at bit 10
#define "_EVT_JOY_ACT_SHFT%","10"
#define "_EVT_JOY_BUTTON_MSK%", "(3<<13)"
#define "_EVT_JOY_BUTTON_SHFT%","13"

#define "_EVT_JOY_POSY_MSK%","(&HFFFF<<15)"
#define "_EVT_JOY_POSY_SHFT%","14"

#define "_EVT_JOY_POSX_MSK%","(&HFFFF<<31)"
#define "_EVT_JOY_POSX_SHFT%","30"

'Public subs and functions
'-------------------------

'Create a joystick event object.
FUNCTION EVT_JOY_create%()
  EVT_JOY_create% = EVT_JOY% 'Event type
END FUNCTION

'Set joystick event action
SUB EVT_JOY_setAction(ev%, action%)
  setBitField ev%, _EVT_JOY_ACT_MSK%, _EVT_JOY_ACT_SHFT%, action%
END SUB

'Get joystick event action
FUNCTION EVT_JOY_getAction%(ev%)
  getBitField ev%, _EVT_JOY_ACT_MSK%, _EVT_JOY_ACT_SHFT%, EVT_JOY_getAction%
END FUNCTION

'Set joystick event button
SUB EVT_JOY_setButton(ev%, button%)
  setBitField ev%, _EVT_JOY_BUTTON_MSK%, _EVT_JOY_BUTTON_SHFT%, button%
END SUB

'Get joystick event button
FUNCTION EVT_JOY_getButton%(ev%)
  getBitField ev%, _EVT_JOY_BUTTON_MSK%, _EVT_JOY_BUTTON_SHFT%, EVT_JOY_getButton%
END FUNCTION

'Set joystick event position. x and y are 12.4 fixed point values
SUB EVT_JOY_setPos(ev%, xx%, yy%)
  ev% = (ev% AND (INV (_EVT_JOY_POSX_MSK% OR _EVT_JOY_POSY_MSK%)))
  ev% = ev% OR ((xx%<<_EVT_JOY_POSX_SHFT%) AND _EVT_JOY_POSX_MSK%) OR ((yy%<<_EVT_JOY_POSY_SHFT%) AND _EVT_JOY_POSY_MSK%)
END SUB

'Get joystick event x-position. .4 fixed point values
FUNCTION EVT_JOY_getPosX%(ev%)
  'This is the reverse of the set operation above, with sign extension.
  EVT_JOY_getPosX% = (ev%<<(48-_EVT_JOY_POSX_SHFT%))>>>48
END FUNCTION

'Get joystick event y-position. .4 fixed point values
FUNCTION EVT_JOY_getPosY%(ev%)
  'This is the reverse of the set operation above, with sign extension.
  EVT_JOY_getPosY% = (ev%<<(48-_EVT_JOY_POSY_SHFT%))>>>48
END FUNCTION

'Get joystick event x/y-position. .4 fixed point values
SUB EVT_JOY_getPos(ev%, xx%, yy%)
  xx% = (ev%<<(48-_EVT_JOY_POSX_SHFT%))>>>48
  yy% = (ev%<<(48-_EVT_JOY_POSY_SHFT%))>>>48
END SUB

