'Public constants
'----------------

'List of possible object types and associated names

CONST OBJ_TYPES_MAX_NUM% = 100 'Max. number of different object types supported.'

CONST OBJ_TYPE_OBJECT%=0 'Base object type.
CONST OBJ_TYPE_VIEW_OBJECT%=1 'ViewObject type.
CONST OBJ_TYPE_TEXT_ENTRY%=2 'TextEntry object type.
'Deus-Ex-Machine object type. Invisible omni-present. Useful for event handling outside
'of object context (e.g. capturing input, step events, etc.).
CONST OBJ_TYPE_DXM%=3 
CONST OBJ_TYPE_TEST%=4 'Test object type.
CONST OBJ_TYPE_TEST2%=5 'Another test object type.
CONST OBJ_TYPE_USR_START%=6 'Starting value of user defined types

'Read-Only variables
'-------------------
DIM OBJ_NAMES$(OBJ_TYPES_MAX_NUM%) LENGTH 16

OBJ_NAMES$(OBJ_TYPE_OBJECT%) = "Object"
OBJ_NAMES$(OBJ_TYPE_VIEW_OBJECT%) = "View Object"
OBJ_NAMES$(OBJ_TYPE_TEXT_ENTRY%) = "Text Entry"
OBJ_NAMES$(OBJ_TYPE_DXM%) = "DXM Object"
OBJ_NAMES$(OBJ_TYPE_TEST%) = "Test Object"
OBJ_NAMES$(OBJ_TYPE_TEST2%) = "Test Object 2"

