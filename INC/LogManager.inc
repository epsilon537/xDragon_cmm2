OPTION CONSOLE SERIAL

'Read-Write variables
'--------------------
DIM lm_enabled% = 0 'Log Manager enable flag.

'Private variables and constants
'-------------------------------

'3-tuple of log arguments passed in by CSUBs calling the logger.
DIM _lm_logTable%(2) = (0,0,0)

'Public subs and functions
'-------------------------

'Start up the Log Manager.
SUB LM_startUp
  lm_enabled% = 1
END SUB

'Shut down the LogManager.
SUB LM_shutDown
  lm_enabled% = 0
END SUB

'Write to log.
SUB LM_writeLog(msg$)
  IF lm_enabled% THEN
    PRINT msg$
  ENDIF
END SUB

'Private subs and functions
'--------------------------

'Callout for CSUBS
SUB _LM_LOG_CALLOUT
  IF lm_enabled% THEN
    PRINT "CSUB: "+CHR$(_lm_logTable%(0))+": &H"+HEX$(_lm_logTable%(1))+", "+HEX$(_lm_logTable%(2))
  ENDIF
END SUB

