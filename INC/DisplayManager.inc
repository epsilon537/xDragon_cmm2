'Public constants
'----------------
CONST DM_DISPLAY_PAGE_ID_MAIN% = 0 'Double buffering: Display page alterates between these two pages.
CONST DM_DISPLAY_PAGE_ID_ALT% = 1

'Read-Write variables
'--------------------
DIM dm_shakeSeverity%=0 'Screen shake severity in pixels.
DIM dm_shakeDuration%=0 'Screen shake duration in frames.
DIM dm_saveImage%=0 'Boolean. Set to one to take an screenshot.
DIM dm_saveImageName$="xd_screenshot" 'File name used to save screenshot.
DIM dm_updateCLUT%=0 'Semaphore indicating that the CLUT should be updated.

'Read-Only variables
'-------------------
DIM dm_displayPageId%=0 'Current display page id.
DIM dm_isStarted%=0 'Flag indicating that the Display Manager is active.
DIM dm_writePage%=0 'This variable tracks the current write page.

'Public subs and functions
'-------------------------

'Draw character at world location with color.
'world_x/y are in .4 subpixel units.
SUB DM_drawCh(worldX%, worldY%, c$, colr%)
  LOCAL viewPosX% = worldX% - box_cornerX%(wm_view%)
  LOCAL viewPosY% = worldY% - box_cornerY%(wm_view%)

  'Convert to pixel units
  TEXT viewPosX%\16, viewPosY%\16, c$, LT,,, colr%
END SUB

'Draw string centered at world location with color
'world_x/y are in .4 subpixel units
SUB DM_drawString(worldX%, worldY%, s$, colr%)
  LOCAL viewPosX% = worldX% - box_cornerX%(wm_view%)
  LOCAL viewPosY% = worldY% - box_cornerY%(wm_view%)
  
  'Convert to pixel units
  TEXT viewPosX%\16, viewPosY%\16, s$, CM,,, colr%, 0
END SUB

'Use this instead of PAGE WRITE directly so we can track which page is currently
'written to.
SUB DM_setWritePage(pageId%)
  dm_writePage% = pageId%
  PAGE WRITE pageId%
END SUB

'Restore the default write page (dm_displayPageId)
SUB DM_restoreDefWritePage
  dm_writePage% = dm_displayPageId%
  PAGE WRITE dm_displayPageId%
END SUB

'Start the Display Manager. Should be done once, at start-up of the game engine.
SUB DM_startUp
  OPTION CONSOLE SERIAL
  MODE 7, 8
  DM_setWritePage dm_displayPageId%
  BOX 0, 0, MM.HRES, MM.VRES, 0,0,0

  dm_isStarted%=1
END SUB

'Shut down the Display Manager.
SUB DM_shutDown
  PAGE DISPLAY 0
  PAGE WRITE 0
  dm_isStarted%=0  
END SUB

'Block in this SUB until Vsync.
SUB DM_Vsync
  IF dm_updateCLUT% THEN
    dm_updateCLUT%=0
    MAP SET
  ENDIF
  
  IF dm_saveImage% THEN
    SAVE IMAGE dm_saveImageName$
    dm_saveImage% = 0
  ENDIF  

  'Toggle display page
  PAGE DISPLAY dm_displayPageId%
END SUB

'Refresh background and tile pages and update write page.
SUB DM_refreshPages
  dm_displayPageId% = dm_displayPageId% XOR DM_DISPLAY_PAGE_ID_ALT%
  DM_setWritePage dm_displayPageId%
    
  'Not syncing to Vsync at this point. We'll do that separately.

  'Copy background page
  PAGE COPY BG_PAGE_ID% TO dm_displayPageId%
     
  'Check tile engine page
  IF te_enabled% THEN
    PAGE COPY TE_PAGE_ID% TO dm_displayPageId%,, t 'Don't copy black.
  ENDIF
  
  IF dm_shakeDuration%>0 THEN
    INC dm_shakeDuration%, -1
    PAGE SCROLL dm_displayPageId%, dm_shakeSeverity%*(RND-0.5), dm_shakeSeverity%*(RND-0.5), 0
  ENDIF  
END SUB

