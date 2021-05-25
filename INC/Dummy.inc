'Read-Write variables
'-------------------
DIM dummy_i% 'A global dummy variable.

'Public subs and functions
'-------------------------

'Do nothing sub with 1 integer argument
SUB Dummy_sub_i(ii%)
END SUB

'Do nothing function with 2 integer arguments, returning an interger
FUNCTION Dummy_fun_iii%(ii%, jj%)
  Dummy_fun_iii%=0
END FUNCTION

