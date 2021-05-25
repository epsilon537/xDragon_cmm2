option explicit
option base 0

OPEN "sys.pal" FOR OUTPUT AS #1

DIM ii%
DIM r%,g%,b%

FOR ii%=0 TO 255
  r% = ii%>>5
  g% = (ii%>>2) AND 7
  b% = ii% AND 3
  
  r% = r%<<5
  g% = g%<<5
  b% = b%<<6
  
  PRINT #1, STR$(r%)+" "+STR$(g%)+" "+STR$(b%)
NEXT ii%

CLOSE #1

END


