'Public constants
'----------------
CONST DQ$ = CHR$(34) 'Double Quote character.

'Public subs and functions
'-------------------------

'This is a collection of utility subs and functions.


'void boxIntersectBox(long long* box_a_p, long long *box_b_p, long long *resp)
'box_a and box_b are box ids, resp is a boolean indicating if the given boxs intersect.
CSUB boxIntersectBox INTEGER, INTEGER, INTEGER
  00000000
  B5F04B1E 6806681C E9D4680D E9D41000 F851E402 F8513036 F850C035 45637035 
  1036F850 F85EDA1B 44180036 BFB44560 20012000 DA0942B9 3036F854 42B94419 
  2000DA09 E9C22100 BDF00100 3035F854 428B443B 17C1DB0C 0100E9C2 F85EBDF0 
  44600035 DA064298 D0DC4563 E7E12000 D1E642B9 2001E7E0 BF00E7DC 0800031C 
End CSUB

'Convert relative bounding box for object to absolute world box at given position.
SUB getWorldBox(objId%, worldBoxId%)
  'Box_copy(worldBoxId%, Obj_getBox%(objId%))
  Box_copy(worldBoxId%, (obj_flags%(objId%) AND OBJ_FLG_BOX_MSK%) >> OBJ_FLG_BOX_SHFT%)
  INC box_cornerX%(worldBoxId%), obj_posX%(objId%) 
  INC box_cornerY%(worldBoxId%), obj_posY%(objId%)   
END SUB

'Convert relative bounding box for Object to absolute world box at given position
'where is a vector.
SUB getWorldBoxAtPos(objId%, xx%%, yy%, worldBoxId%)
  LOCAL from_box% = (obj_flags%(objId%) AND OBJ_FLG_BOX_MSK%) >> OBJ_FLG_BOX_SHFT%)
  box_cornerX%(worldBoxId%) = box_cornerX%(from_box%) + xx%%
  box_cornerY%(worldBoxId%) = box_cornerY%(from_box%) + where_t%
  box_width%(worldBoxId%) = box_width%(from_box%)
  box_height%(worldBoxId%) = box_height%(from_box%)
END SUB

'Return true if Box contains position xx/yy.
FUNCTION boxContainsPosition%(boxId%, xx%, yy%)
  LOCAL corner_x% = box_cornerX%(boxId%)
  LOCAL corner_y% = box_cornerY%(boxId%)

  LOCAL bx1% = corner_x%
  LOCAL by1% = corner_y%
  LOCAL bx2% = bx1% + box_width%(boxId%)
  LOCAL by2% = by1% + box_height%(boxId%)
  
  'Test horizontal overlap
  LOCAL x_overlap% = (xx%>= bx1%) AND (xx% <= bx2%)
  'Test vertical overlap
  LOCAL y_overlap% = (yy%>= by1%) AND (yy% <= by2%)

  boxContainsPosition% = x_overlap% AND y_overlap%
END FUNCTION

'Return true of Box a completely contains Box b.
FUNCTION boxContainsBox%(boxA%, boxB%)
  LOCAL aCorner_x% = box_cornerX%(boxA%)
  LOCAL aCorner_y% = box_cornerY%(boxA%)

  LOCAL bCorner_x% = box_cornerX%(boxB%)
  LOCAL bCorner_y% = box_cornerY%(boxB%)
  
  LOCAL ax1% = aCorner_x%
  LOCAL bx1% = bCorner_x%
  LOCAL ay1% = aCorner_y%
  LOCAL by1% = bCorner_y%
  LOCAL ax2% = ax1% + box_width%(boxA%)
  LOCAL bx2% = bx1% + box_width%(boxB%)
  LOCAL ay2% = ay1% + box_height%(boxA%)
  LOCAL by2% = by1% + box_height%(boxB%)
  
  'Test horizontal overlap
  LOCAL x_overlap% = (ax1%<= bx1%) AND (ax2% >= bx2%)
  'Test vertical overlap
  LOCAL y_overlap% = (ay1%<= by1%) AND (ay2% >= by2%)
  
  boxContainsBox% = x_overlap% AND y_overlap%
END FUNCTION

'Convert world position to view position. x component.
FUNCTION worldToViewX%(worldPosX%)
  worldToViewX% = worldPosX% - box_cornerX%(wm_view%)
END FUNCTION

'Convert world position to view position. y component.
FUNCTION worldToViewY(worldPosY%)
  worldToViewY = worldPosY% - box_cornerY%(wm_view%)
END FUNCTION

'Convert world position to view position. x and y components.
SUB worldToView(xx%, yy%)
  INC xx%, -box_cornerX%(wm_view%)
  INC yy%, -box_cornerY%(wm_view%)
END SUB

'Convert view position to world. x component.
FUNCTION viewToWorldX%(viewPosX%)
  viewToWorldX% = viewPosX% + box_cornerX%(wm_view%)
END FUNCTION

'Convert view position to world. x component. y component.
FUNCTION viewToWorldY%(view_pos_y%)
  viewToWorldY% = view_pos_y% + box_cornerY%(wm_view%)
END FUNCTION

'Convert view position to world. x and ycomponents.
SUB viewToWorld(xx%, yy%)
  INC xx%, box_cornerX%(wm_view%)
  INC yy%, box_cornerY%(wm_view%)
END SUB

'Copy a block of 8-byte words from one block with offset to another with offset.
'void blockCopyWords(long long *to, long long *to_offset, long long *from, long long *from_offset, long long *numWords)
CSUB blockCopyWords
  00000000
  9D02B430 6829680C 00C4EB00 EB00681C EB0203C1 429802C4 E8F2D205 E8E04502 
  42834502 BC30D8F9 BF004770 
End CSUB

'Extract a string of given length from a block of bytes, with offset.
'void stringExtract(char *s, long long *from, long long *from_offset, long long *numBytesp)
CSUB stringExtract STRING, INTEGER, INTEGER, INTEGER
  00000000
  6814B410 4421681A 70022A00 440ADD06 3B01F811 F8004291 D1F93F01 4B04F85D 
  BF004770 
End CSUB

'Extract basename from give filename. Filename may include path
FUNCTION fileToBasename$(filename$)
  LOCAL separatorPos%=0, basenameStart%=1, basenameEnd%=LEN(filename$)+1
  
  'Search for rightmost / or \
  DO
    separatorPos%=MAX(INSTR(separatorPos%+1, filename$, "/"), INSTR(separatorPos%+1, filename$, "\"))
    IF separatorPos%=0 THEN
      EXIT DO
    ENDIF
    basenameStart% = separatorPos%+1
  LOOP

  'Now search for rightmost .
  separatorPos%=basenameStart%-1
  DO
    separatorPos%=INSTR(separatorPos%+1, filename$, ".")
    IF separatorPos%=0 THEN
      EXIT DO
    ENDIF
    basenameEnd% = separatorPos%
  LOOP
  
  fileToBasename$ = MID$(filename$, baseNameStart%, baseNameEnd%-baseNameStart%)
END FUNCTION

'Trim any characters in c$ from the start of s$. Taken from CMM2 User Guide.
FUNCTION LTrim$(ss$, cc$)
  LTrim$ = ss$
  DO WHILE INSTR(cc$, LEFT$(LTrim$, 1))
    LTrim$ = MID$(LTrim$, 2)
  LOOP
END FUNCTION

'Convert RGB888 value to RGB332
FUNCTION RGB888toRGB332%(rgbval%)
  LOCAL r% = (rgbval%>>21) AND 7
  LOCAL g% = (rgbval%>>13) AND 7
  LOCAL b% = (rgbval%>>6) AND 3
  
  RGB888toRGB332% = (r%<<5) OR (g%<<2) OR b%
END FUNCTION

'Delete words from a block.
'void delWordsFromBlock(long long *block, long long *blockOffset, long long *blockSizeInWords, long long *numWordsToDelp)
CSUB delWordsFromBlock INTEGER, INTEGER, INTEGER, INTEGER
  00000000
  6814B410 680B681A EB001AA4 EB0003C3 42A304C4 EB03D208 3A0802C2 0102E9F2 
  0102E8E3 D8F9429C 4B04F85D BF004770 
End CSUB

'void setBitField(long long *wordp, long long *bitmaskp, long long *bitshiftp, long long *bitfieldp)
CSUB setBitField INTEGER, INTEGER, INTEGER, INTEGER
  00000000
  41F0E92D 684C680F 68456806 0607EA26 0504EA25 6500E9C0 F8D16812 F1A2E004 
  F8D10C20 E9D38000 FA077300 4093F402 FC0CFA07 0220F1C2 0108EA04 030CEA43 
  F202FA27 43134331 EA036001 432B030E E8BD6043 BF0081F0 
End CSUB

'void getBitField(long long *wordp, long long *bitmaskp, long long *bitshiftp, long long *bitfieldp)
CSUB getBitField INTEGER, INTEGER, INTEGER, INTEGER
  00000000
  6814B4F0 6700E9D1 2000E9D0 0520F1C4 0107EA00 F1A44032 FA010020 40E2F505 
  F000FA21 432A40E1 43026059 601ABCF0 BF004770 
End CSUB

'Byte compare two memory blocks. Returns non-zero in res if equal.
'void blockCompareEQ(long long *lhs, long long *rhs, long long *numBytes, long long *res)
CSUB blockCompareEQ INTEGER, INTEGER, INTEGER, INTEGER
  00000000
  43F0E92D E000F8D2 46746855 F1752C00 DA030200 0407F11E 0500F145 EA4208E2 
  F1BC7C45 DD160F00 0508F1A0 0408F1A1 E0012200 D00E4594 E9F53201 E9F48902 
  45B96702 45B0BF08 2000D0F4 E9C32100 E8BD0100 F01E83F0 D0110E07 0ECCEB0E 
  01CCEB01 040EEB00 02CCEB00 42A2E001 F812D006 F8115B01 42850B01 E7E4D0F7 
  21002001 0100E9C3 83F0E8BD 
End CSUB

