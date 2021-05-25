'Public constants
'----------------
CONST OBJLST_MAX_SIZE% = 400 'Max number of items an objectList can contain.
CONST OBJLST_MAX_NUM% = 100 'Max. number of objectList that can simultaneously exist in the game.'

'Read-Only variables
'-------------------
'The list of objectLists. 1st index is object id. 2nd index is object list id.
DIM objLst_list%(OBJLST_MAX_SIZE%-1, OBJLST_MAX_NUM%-1)

'Number of elements in the object list. Set to -1 for not allocated entries.
DIM objLst_numElems%(OBJLST_MAX_NUM%-1)
SUB objLst_numElemsInit 'Initializer
  LOCAL ii%
  
  FOR ii%=0 TO OBJLST_MAX_NUM%-1
    objLst_numElems%(ii%) = -1
  NEXT ii%
END SUB
objLst_numElemsInit

DIM objLst_numAllocated%=0 'Used to track resource leaks.

'Private variables and constants
'-------------------------------

'Index into table above used to find the next available (unallocated) objectList
DIM _objLst_allocIdx%=0

'Public subs and functions
'-------------------------

'Allocate an objectList from the objectList heap and initialize it. Returns an objList id.
FUNCTION ObjLst_create%()
  LOCAL ii% = _objLst_allocIdx%

  DO
    IF objLst_numElems%(ii%) = -1 THEN 'Free entry found
      'Allocate, num elems = 0
      objLst_numElems%(ii%) = 0
      
      'Update index to next hopefully free entry
      _objLst_allocIdx% = ii% + 1
      IF _objLst_allocIdx% >= OBJLST_MAX_NUM% THEN
        _objLst_allocIdx%=0
      ENDIF
      
      INC objLst_numAllocated%
      ObjLst_create% = ii%
      EXIT FUNCTION 
    ENDIF
    
    INC ii%
    IF ii% >= OBJLST_MAX_NUM% THEN
      ii%=0
    ENDIF
  LOOP UNTIL ii% = _objLst_allocIdx%
  
  ERROR "ObjectList Heap exhausted!"
END FUNCTION

'Destroy given objList, releasing all resources used by this objList.
SUB ObjLst_destroy(lid%)
  IF objLst_numElems%(lid%) = -1 THEN
    ERROR "Attempt to free an ObjectList that was not allocated."
  ENDIF
  
  INC objLst_numAllocated%, -1
  objLst_numElems%(lid%) = -1
END SUB

'Returns non-zero if objectList is allocated.
FUNCTION ObjLst_isAllocated%(lid%)
  ObjLst_isAllocated% = objLst_numElems%(lid%) <> -1
END FUNCTION

'Copy an objectList
SUB ObjLst_copy(fromLid%, toLid%)  
  objLst_numElems%(toLid%) = objLst_numElems%(fromLid%)
  blockCopyWords(objLst_list%(0, toLid%), 0, objLst_list%(0, fromLid%), 0, objLst_numElems%(fromLid%)*8)
END SUB

'Insert object into an objectList.
SUB ObjLst_insert(lid%, objId%)
  LOCAL numElems% = objLst_numElems%(lid%)
  IF numElems% >= OBJLST_MAX_SIZE% THEN
    ERROR "ObjList id="+STR$(lid%)+" is full."
  ENDIF
  
  objLst_list%(numElems%, lid%) = objId%
  INC objLst_numElems%(lid%)
END SUB

'Returns list index of given object, or -1 if not found.
FUNCTION ObjLst_find%(lid%, objId%)
  LOCAL ii%=0
  
  FOR ii%=0 TO objLst_numElems%(lid%)-1
    IF objLst_list%(ii%, lid%) = objId% THEN
      ObjLst_find% = ii%
      EXIT FUNCTION
    ENDIF
  NEXT ii%      
  ObjLst_find% = -1
END FUNCTION

'Remove object from list. Returns 0 if success, -1 if obj_id not found.
FUNCTION ObjLst_remove%(lid%, objId%)
  LOCAL index% = ObjLst_find%(lid%, objId%)  
  IF index% = -1 THEN
    ObjLst_remove% = -1
    EXIT FUNCTION
  ENDIF

  delWordsFromBlock objLst_list%(0, lid%), index%, objLst_numElems%(lid%), 1
  INC objLst_numElems%(lid%), -1
  ObjLst_remove% = 0
END FUNCTION

'Clear objectList.
SUB ObjLst_clr(lid%)
 objLst_numElems%(lid%) = 0
END SUB

'Check is objectList is empty.
FUNCTION ObjLst_isEmpty%(lid%)
  ObjLst_isEmpty% = objLst_numElems%(lid%) = 0
END FUNCTION

'Check if objectList is full.
FUNCTION ObjLst_isFull%(lid%)
  ObjLst_isFull% = objLst_numElems%(lid%) >= OBJLST_MAX_SIZE%
END FUNCTION

'Compare two objLists for equality.
FUNCTION ObjLst_eq%(lid1%, lid2%)
  IF objLst_numElems%(lid1%) <> objLst_numElems%(lid2%) THEN
    ObjLst_eq% = 0
  ENDIF

  LOCAL numBytes% = objLst_numElems%(lid1%)*8
  LOCAL res%
  
  blockCompareEQ(objLst_list%(0, lid1%), objLst_list%(0, lid2%), numBytes%, res%)

  ObjLst_eq% = res%
END FUNCTION

'Append one objectList to another.
SUB ObjLst_append(toLid%, fromLid%)
  LOCAL from_numWords% = objLst_numElems%(fromLid%)
  LOCAL to_numWords% = objLst_numElems%(toLid%)
  LOCAL numWords% = from_numWords% + to_numWords%
  
  IF numWords% >= OBJLST_MAX_SIZE% THEN
    Error "Can't append to objList "+STR$(toLid%)
  ENDIF
  
  blockCopyWords(objLst_list%(0, toLid%), to_numWords%, objLst_list%(0, fromLid%), 0, from_numWords%)

  objLst_numElems%(toLid%) = numWords%
END SUB

