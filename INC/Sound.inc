'Private variables and constants
'-------------------------------

CONST _SOUND_STOPPED%=0
CONST _SOUND_PLAYING%=1

DIM _sound_curFile$ = ""
DIM _sound_playInLoop% = 0
DIM _sound_stop_ack% = 0
DIM _sound_stop_req% = 0

'Private subs and functions
'-------------------------

'Sound play completed/stopped interrupt routine.
SUB _Sound_playInt  
  IF _sound_stop_req%=1 THEN
    _sound_stop_req%=0
    PLAY WAV _sound_curFile$ 'Remove interrupt routine
    PLAY STOP 'Immediately stop, before previous play command actual start does anything
    _sound_stop_ack%=1
    EXIT SUB
  ENDIF
  
  IF _sound_playInloop% THEN
    PLAY WAV _sound_curFile$, _Sound_playInt
  ELSE
    'Reset _sound_curFile to indicate that nothing's currently playing anymore
    _sound_curFile$=""
  ENDIF
END SUB

'Public subs and functions
'-------------------------

'Play a WAV sound, optionally in a loop.
SUB Sound_play(fname$, loopSound%)
  Sound_stop
  
  _sound_curFile$ = fname$
  _sound_playInLoop% = loopSound%
  
  PLAY WAV _sound_curFile$, _Sound_playInt
END SUB

'Stop playing WAV sound.
SUB Sound_stop
  'Is anything playing?
  IF _sound_curFile$ <> "" THEN
    _sound_stop_req%=1 'Request a sound stop from ISR
    PLAY STOP 'Trigger ISR
    'Wait for ack
    DO WHILE _sound_stop_ack%=0: LOOP
    'Reset flags
    _sound_stop_ack%=0 
    
    _sound_curFile$=""
    _sound_playInLoop% = 0  
  ENDIF
END SUB

