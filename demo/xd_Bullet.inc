'Bullet Object constructor
FUNCTION xd_Bullet_create%(shipPosX%, shipPosY%)
  LOCAL objId% = Obj_create%()
  
  Obj_setAnim(objId%, "bullet") 'Link to animation
  Obj_setType(objId%, OBJ_TYPE_XD_BULLET%)
  obj_posX%(objId%) = shipPosX%+16*16 'Position bullet a bit ahead of the ship
  obj_posY%(objId%) = shipPosY%
  obj_velX%(objId%) = 10*16 'Bullet speed is 10 pixels per frame.
  obj_velY%(objId%) = 0
  
  Obj_setSolidness(objId%, OBJ_HARD%) 'Bullets can't move through other objects.
  Obj_setColGroup(objId%, CG_BULLET%) 'Bullets belong to bullet collision group.

  obj_eventH$(objId%) = "xd_Bullet_eventHandler%" 'Register Bullet event handler function.
  
  xd_Bullet_create% = objId%
END FUNCTION

FUNCTION xd_Bullet_eventHandler%(objId%, ev%)
  LOCAL eType% = EVT_getType%(ev%)
  IF eType% = EVT_OUT% THEN 'Bullet leaves the screen. 
    WM_markForDelete(objId%) 'Remove bullet.
    xd_Bullet_eventHandler% = 1
    EXIT FUNCTION
  ELSEIF eType% = EVT_COL% THEN 'Collision.
    WM_markForDelete(objId%) 'Remove bullet.
    xd_Bullet_eventHandler% = 1
    EXIT FUNCTION
  ENDIF
  
  xd_Bullet_eventHandler% = 0
END FUNCTION

