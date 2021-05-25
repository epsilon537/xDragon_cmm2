'Sheets are associated with a video memory page. One sheet per page
'Important: DM_startUp() must be called before this API can be used.

'Public constants
'----------------
CONST SHEET_TABLE_SIZE%=60 'Number of entries in the Sheet Table.
CONST SHEET_NOT_ALLOC_STR$="SheetNotAllocated" 'Sheet label value indicating that this Sheet Table entry is not allocated.
CONST SHEET_RESERVED_STR$="SheetReserved" 'Sheet label value indicating that this Sheet Table entry is reserved.

'Read-Write variables
'-------------------
DIM sheet_refCount%(SHEET_TABLE_SIZE%-1) 'Sheet reference counter. A sheet object can not be released until this is 0.

'Read-Only variables
'-------------------
DIM sheet_numAllocated%=0 'Used to track resource leaks.
DIM sheet_maxNum%=0 'Max. number of sheets (pages) that can exist. Depends on display MODE.

'Private variables and constants
'-------------------------------
DIM _sheet_label$(SHEET_TABLE_SIZE%-1) LENGTH 32

'Index into tables above used to find the next available (unallocated) object 
DIM _sheet_allocIdx%=4

'Public subs and functions
'-------------------------

'Global startup of the Sheet module.
SUB Sheet_startUp
  'pages 0-3 are reserved
  sheet_maxNum% = MM.INFO(MAX PAGES)-4

  LM_writeLog("Num Sheets available: "+STR$(sheet_maxNum%)+"-4")
    
  _sheet_label$(0) = SHEET_RESERVED_STR$
  _sheet_label$(1) = SHEET_RESERVED_STR$
  _sheet_label$(2) = SHEET_RESERVED_STR$
  _sheet_label$(3) = SHEET_RESERVED_STR$
  
  LOCAL ii%
  FOR ii%=4 TO MM.INFO(MAX PAGES)
    _sheet_label$(ii%) = SHEET_NOT_ALLOC_STR$
  NEXT ii%
END SUB

'Shut down the sheet module
SUB Sheet_shutDown
  LOCAL ii%
  
  FOR ii%=4 TO MM.INFO(MAX PAGES)
    IF _sheet_label$(ii%) <> SHEET_NOT_ALLOC_STR$) THEN
      Sheet_unload _sheet_label$(ii%)
    ENDIF
  NEXT ii%
  
END SUB

'Allocate a sheet (a page). The returned id is a page number.
FUNCTION Sheet_create%()
  IF sheet_maxNum%=0 THEN
    ERROR "Sheet module not initialized."
  ENDIF
  
  LOCAL ii% = _sheet_allocIdx%
  
  DO
    IF _sheet_label$(ii%) = SHEET_NOT_ALLOC_STR$ THEN 'Free entry found
      'Allocate
      _sheet_label$(ii%) = ""
      sheet_refCount%(ii%) = 0
      
      'Update index to next hopefully free entry
      _sheet_allocIdx% = ii% + 1
      IF _sheet_allocIdx% > MM.INFO(MAX PAGES) THEN
        _sheet_allocIdx%=2 'pages 0 and 1 are off limits
      ENDIF

      'Clear the sheet
          
      'Set page write to the allocated sheet
      DM_setWritePage ii%

      BOX 0,0, MM.HRES, MM.VRES, 0,,0

      'Restore write page
      DM_restoreDefWritePage      
    
      INC sheet_numAllocated%  
      Sheet_create% = ii%
      EXIT FUNCTION 
    ENDIF
    
    INC ii%
    IF ii% > MM.INFO(MAX PAGES) THEN
      ii%=4 'pages 0-3 are off limits
    ENDIF
  LOOP UNTIL ii% = _sheet_allocIdx%
  
  ERROR "Sheet Heap exhausted!"
END FUNCTION

'Release sheet (page)
SUB Sheet_destroy(pageId%)
  IF _sheet_label$(pageId%) = SHEET_NOT_ALLOC_STR$ THEN
    ERROR "Attempt to free an Sheet that was not allocated."
  ENDIF
  
  _sheet_label$(pageId%) = SHEET_NOT_ALLOC_STR$
  
  INC sheet_numAllocated%, -1  
END SUB

'Returns non-zero if sheet is allocated.
FUNCTION Sheet_isAllocated%(pageId%)
  Sheet_isAllocated% = _sheet_label$(pageId%) <> SHEET_NOT_ALLOC_STR$
END FUNCTION

'Set label associated with sheet
SUB Sheet_setLabel(pageId%, label$)
  _sheet_label$(pageId%) = label$
END SUB

'Get label associated with sheet
FUNCTION Sheet_getLabel$(pageId%)
  Sheet_getLabel$ = _sheet_label$(pageId%)
END FUNCTION

'Find sheet by label. Returns page id, -1 if not found.
FUNCTION Sheet_find%(label$)
  LOCAL ii%
  
  FOR ii%=4 TO MM.INFO(MAX PAGES)
    IF UCASE$(_sheet_label$(ii%)) = UCASE$(label$)) THEN
      Sheet_find% = ii%
      EXIT FUNCTION
    ENDIF
  NEXT ii%
  
  Sheet_find% = -1
END FUNCTION

'Load sheet from BMP file. Sheet label will be set to file basename.
SUB Sheet_load(filename$)
  'Allocate a sheet
  LOCAL pageId% = Sheet_create%()
  'Switch page write to sheet
  DM_setWritePage pageId%

  LM_writeLog("Loading file: "+filename$+" to Page: "+STR$(pageId%))
  
  'Load BMP
  LOAD BMP filename$
  
  'Restore write page
  DM_restoreDefWritePage  

  LOCAL label$ = fileToBasename$(filename$)  
  Sheet_setLabel(pageId%, label$)
END SUB

'Unload sheet with indicated label.
SUB Sheet_unload(label$)
  LOCAL page_id% = Sheet_find%(label$)
  
  IF sheet_refCount%(page_id%) <> 0 THEN
    ERROR "Can't unload sheet "+label$+". Sheet is still being referenced."
  ENDIF
  
  IF page_id%=-1 THEN
    ERROR "Can't unload sheet "+label$+". Sheet not found."
  ENDIF
  
  'Wipe the sheet:
  
  'Switch page write to sheet
  DM_setWritePage page_id%

  'Using this instead of CLS to avoid continuously clearing the serial console.
  BOX 0,0, MM.HRES, MM.VRES, 0,,0

  'Restore write page
  DM_restoreDefWritePage
  
  Sheet_destroy(page_id%)
END SUB


