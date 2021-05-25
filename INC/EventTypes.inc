'Public constants
'----------------

'Enumeration of Event Types
CONST EVT_UNDEFD% = 0 'Undefined event
CONST EVT_TEST% = 1 'Test event
CONST EVT_STEP% = 2 'Step event
CONST EVT_KBD% = 3 'Keyboard event
CONST EVT_COL% = 4 'Collision event
CONST EVT_OUT% = 5 'Out event
CONST EVT_VIEW% = 6 'View Event
CONST EVT_JOY% = 7 'Joystick Event
CONST EVT_TE_TRIG% = 8 'TileEngine Trigger Event
CONST EVT_USR_START% = 9 'User defined events start from this value onwards.

CONST MAX_NUM_EVT_TYPES% = 32

'Read-Only variables
'-------------------

DIM EVT_NAMES$(MAX_NUM_EVT_TYPES%) LENGTH 16
EVT_NAMES$(EVT_UNDEFD%) = "Undefined Evt."
EVT_NAMES$(EVT_TEST%) = "Test Evt."
EVT_NAMES$(EVT_STEP%) = "Step Evt."
EVT_NAMES$(EVT_KBD%) = "Keyboard Evt."
EVT_NAMES$(EVT_COL%) = "Collision Evt."
EVT_NAMES$(EVT_OUT%) = "Out Evt."
EVT_NAMES$(EVT_JOY%) = "Joystick Evt."
EVT_NAMES$(EVT_TE_TRIG%) = "TileEngTrig Evt."


