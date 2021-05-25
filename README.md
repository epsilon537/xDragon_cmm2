xDragon Game Engine for CMM2
----------------------------
Author: Epsilon

Credits
-------
- Mark Claypool: Author of the original Dragonfly engine on which this engine is based.
A CMM2 port of the Dragonfly engine can be found here: https://github.com/epsilon537/df_cmm2
- Nautilus: The topics covered on the MAPSTER thread on TBS were a great source of inspiration:
Tiles, Palettes, Level Editors, Pixel Editors... The xDragon in its current form is essentially
DragonFly plus everything discussed in that thread.
- jirsoft: Some BMP decoding logic to establish BMP dimensions, taken from NC commander.
- Kauzz/itch.io: xDragon Demo Cave Tiles
- Ansimuz/itch.io: xDragon Demo parallax background graphics.
- Ansimux/itch.io: xDragon Demo Intro soundtrack.
- !!!alchemist2/modarchive.org: xDragon Demo in-game soundtrack.

Changelog
---------
0.1: Initial version

Description
-----------
xDragon is a 2D game engine in tune with the CMM2 feature set. The engine is geared towards 
arcade style action games such as shooters, platformers, top-down racers etc.

Demo Game:
---------
To start the Demo mini-game, navigate to the DEMO/ directory and run "xd_demo.bas".
The demo game is maybe a bit sparse. The focus is on the tutorial aspect of the demo code.

It probably wouldn't be a bad idea to create a showcase demo as well. It would be cool if
people would add some bells and whistles and post the result on TBS.

Features:
--------
* GraphicsGale compatible frame (sprite) based animations.
* Tiled compatible Tile Engine.
* Tile Map based trigger events (Trigger Layer in Tile Map)
* Bounding Box based collision detection, between Objects and between Objects and tiles
* Collision Groups.
* Parallax background layer.
* Palette Management, effects such as fading, support for JASC palette file format exported 
  from GraphicsGale.
* World/View representation
* Object Management
* Nunchuk support
* Event Dispatching (E.g. collisions, keyboard/nunchuk input events, user defined events...)
* Sound support
* HUD-type view objects (e.g. points, lives...)
* Text Entry objects (e.g. for entering high scores).

Tiled and GraphicsGale
----------------------
Tiled and GraphicsGale are key tools used to create game content.

Tiled is a free tile based level editor. It supports multiple layers. The xDragon engine currently
supports a tiling layer, a collision layer and an even trigger layer.
URL: https://www.mapeditor.org/

GraphicsGale is a free pixel editor with great support for creating frame based animations.
GraphicsGale exports spritesheets and metadata in .csv files that describe the animation
parameters (dimensions, position in spritesheet etc.)
URL: https://graphicsgale.com/us/

Sound support
-------------
The code includes a small SOUND.INC module intended for .WAVs, but by far the best way to
handle sound in games is the excellent PLAY MODFILE/MODSAMPLE MMBasic API. 
The Game Engine has nothing to add here.

Vegipete did a great write-up on creating sounds with MODS using Audacity and OpenMPT:
http://www.thebackshed.com/forum/ViewTopic.php?TID=13412&PID=163703#163703

Audacity: https://www.audacityteam.org/
OpenMPT: https://openmpt.org/

Directory Layout:
----------------
* INC/ contains the game engine implementation
* TEST/ contain game engine test cases. The test code is messy but all test cases work.
  It's a good repository to check to see how certain parts of the API work.
* DEMO/ A demo mini-game used to demonstrate most of the features of the game engine.
* RESOURCES/ Resource files such as bitmaps and sounds.
* DOCS/ Documentation. Currently just some odds and ends. I can add actual documentation if
  there is any interest.
* CSUBS/ CSUBs and their source code.

To-Dos:
------
- Support animated tiles.
- Add nicer ViewObjects for HUD display.
- Add High Score to demo game as an illustration of the TextEntry object.
- Add support for path definitions created in Tiled Object Layer.
- Add support for small viewports surrounded by more elaborate HUD display.
- Add more info to OUT Event Object.
- Add support for diagonal scrolling (currently only horizontal and vertical).
- Improve log handling.
- Improve documentation.
- Reduce global variable count.
- Write a game.

Required CMM2 FW:
----------------
V5.07.00b30 or later.

GitHub:
------
https://github.com/epsilon537/xDragon_cmm2

