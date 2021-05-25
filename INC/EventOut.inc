'Public subs and functions
'-------------------------

'Create an OUT Event object, triggered when an object moves out of view.
FUNCTION EVT_OUT_create%()
  LOCAL ev% = EVT_create%()
  EVT_setType(ev%, EVT_OUT%)
  EVT_OUT_create% = ev%
END FUNCTION
