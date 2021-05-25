
'Collision group definitions
CONST CG_SHIP% = CG_ID_1%
CONST CG_SHIP_MSK% = CG_ID_1_MSK%
CONST CG_BULLET% = CG_ID_2%
CONST CG_BULLET_MSK% = CG_ID_2_MSK%
CONST CG_ENEMY% = CG_ID_3%
CONST CG_ENEMY_MSK% = CG_ID_3_MSK%

SUB xd_ColGroupsInit
  'Ship collides with tiles and enemies
  cg_maps%(CG_SHIP%) = CG_ID_TILES_MSK% OR CG_ENEMY_MSK%
  'Bullets collide with tiles and enemies
  cg_maps%(CG_BULLET%) = CG_ID_TILES_MSK% OR CG_ENEMY_MSK%
  'Enemies collide with bullets and ship
  cg_maps%(CG_ENEMY%) = CG_BULLET_MSK% OR CG_SHIP_MSK%
END SUB
