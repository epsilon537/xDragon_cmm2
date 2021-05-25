'Public constants
'----------------

'Key codes to be used with EVT_KBD_get/setKey.

'Modifier key codes. Non-modifier key codes are KEYDOWN(1-6) value
CONST LCTRL_KEY% = 256
CONST RCTRL_KEY% = 257
CONST LALT_KEY% = 258
CONST RALT_KEY% = 259
CONST LSHFT_KEY% = 260
CONST RSHFT_KEY% = 261

CONST SPC_KEY% = 32

CONST PLUS_KEY% = 43 
CONST COMMA_KEY% = 44
CONST MINUS_KEY% = 45
CONST SLASH_KEY% = 47 

CONST NUM1_KEY% = 49
CONST NUM2_KEY% = 50
CONST NUM3_KEY% = 51
CONST NUM4_KEY% = 52
CONST NUM5_KEY% = 53
CONST NUM6_KEY% = 54
CONST NUM7_KEY% = 55
CONST NUM8_KEY% = 56
CONST NUM9_KEY% = 57
CONST NUM0_KEY% = 48

CONST A_KEY% = 97
CONST B_KEY% = 98
CONST C_KEY% = 99
CONST D_KEY% = 100
CONST E_KEY% = 101
CONST F_KEY% = 102
CONST G_KEY% = 103
CONST H_KEY% = 104
CONST I_KEY% = 105
CONST J_KEY% = 106
CONST K_KEY% = 107
CONST L_KEY% = 108
CONST M_KEY% = 109
CONST N_KEY% = 110
CONST O_KEY% = 111
CONST P_KEY% = 112
CONST Q_KEY% = 113
CONST R_KEY% = 114
CONST S_KEY% = 115
CONST T_KEY% = 116
CONST U_KEY% = 117
CONST V_KEY% = 118
CONST W_KEY% = 119
CONST X_KEY% = 120
CONST Y_KEY% = 121
CONST Z_KEY% = 122

CONST BCKSPC_KEY% = 8
CONST TAB_KEY% = 9
CONST ENTER_KEY% = 10
CONST ESC_KEY% = 27

CONST UP_KEY% = 128 
CONST DOWN_KEY% = 129 
CONST LEFT_KEY% = 130 
CONST RIGHT_KEY% = 131 
CONST F1_KEY% = 145
CONST F2_KEY% = 146
CONST F3_KEY% = 147
CONST F4_KEY% = 148 
CONST F5_KEY% = 149 
CONST F6_KEY% = 150 
CONST F7_KEY% = 151 
CONST F8_KEY% = 152 
CONST F9_KEY% = 153 
CONST F10_KEY% = 154 
CONST F11_KEY% = 155 
CONST F12_KEY% = 156

'Types of keyboard actions
CONST EVT_KBD_ACT_UNDEF% = 0
CONST EVT_KBD_ACT_KEY_PRESSED% = 1 'Was down
CONST EVT_KBD_ACT_KEY_RELEASED% = 2 'Was released
CONST EVT_KBD_ACT_KEY_DOWN% = 3 'Is down

'Private variables and constants
'-------------------------------

CONST _EVT_KBD_KEY_MSK% =(1023<<10)
CONST _EVT_KBD_KEY_SHFT% =10
CONST _EVT_KBD_ACT_MSK% = (7<<20)
CONST _EVT_KBD_ACT_SHFT% = 20

'Public subs and functions
'-------------------------

'Create a keyboard event object.
FUNCTION EVT_KBD_create%()
  EVT_KBD_create% = EVT_KBD% 'Event type
END FUNCTION

'Set key in event.
SUB EVT_KBD_setKey(ev%, key%)
  setBitField ev%, _EVT_KBD_KEY_MSK%, _EVT_KBD_KEY_SHFT%, key%
END SUB

'Get key from event.
FUNCTION EVT_KBD_getKey%(ev%)
  getBitField ev%, _EVT_KBD_KEY_MSK%, _EVT_KBD_KEY_SHFT%, EVT_KBD_getKey%
END FUNCTION

'Set keyboard event action.
SUB EVT_KBD_setAction(ev%, action%)
  setBitField ev%, _EVT_KBD_ACT_MSK%, _EVT_KBD_ACT_SHFT%, action%
END SUB

'Get keyboard event action.
FUNCTION EVT_KBD_getAction%(ev%)
  getBitField ev%, _EVT_KBD_ACT_MSK%, _EVT_KBD_ACT_SHFT%, EVT_KBD_getAction%
END FUNCTION

