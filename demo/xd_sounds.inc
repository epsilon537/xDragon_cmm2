'MOD file channels holding the sounds effects used.
CONST XD_SOUND_FIRE% = 27
CONST XD_SOUND_EXPLODE% = 28
CONST XD_SOUND_NUKE% = 29
CONST XD_SOUND_SPEEDUP% = 30

'Load the MOD file and start playing the music.
SUB xd_loadSounds
  PLAY MODFILE "../resources/alchemist2fx.mod"
END SUB

'Play back a sound effect (see CONST definitions above)
SUB xd_playSound(sound%)
  PLAY MODSAMPLE sound%, sound%-XD_SOUND_FIRE%+1
END SUB

'Stop the MOD. To be called before switching back to WAVs.
SUB xd_unloadSounds
  PLAY STOP
END SUB


