// file used to plan the program
//
/*

outline of editing capabilities:

1. Move / Expand Data
    a. Rom Expansion (X)
    b. Overworld
    c. Dungeons (X)
    d. Graphics

2. Editing various HM unsupported things
    a. Weather Vane
    b. Dungeon Opening Sequences (misery mire etc)

3. Tile editing
    a. Standard Decompression (X)
    b. Other Decompression (X)
    c. Conversion to Bitmaps (X)
    d. palettes? (X)

4. Standard Editing
    a. Dungeons
        -dungeon headers (X)
        -displaying graphics



general discussion: with myself

  Q. how many files do I want open at a time?
  A.  -I want a main window. 
      -Opening a rom will generate a child window. (unlimited)
      -Closing the program will close all child windows.
      -This allows for less clutter in the client region and frees up room
      to add help and documentation to the main module.

  hierarchy of includes:

  Black Magic.cpp -> Black Magic.h -> Strings.h
                  -> Strings.h     ->Strings.cpp
                  -> Windows.h

  // Notes: CreateWindowEx() is a bitch. 
  // Remind me to defecate upon it then have a dog eat it.

  // Have to use SetWindowText() for captions
  // Have to use DrawMenuBar() for the initial shit to show up.

  // Okay... how to handle allocating new game structures?
  // User opens the first game and it brings up a new window
  // every subsequent game brings up a new edit window.
  // The main window is used for export data from one rom to another

  4bpp graphics format for SNES, planar. Basically a dual 2bpp image stacked together.

  Each Line consists of two words. The first word is in the first 2bpp image. 2bpp images
  are 16 bytes long, just so you know. So that means the next word is 14 bytes offset from the
  end of the first word, in it's relative position in the second 2bpp image.

  In a hex editor it will look like this:

  $000000: XX YY -- -- -- -- -- -- -- -- -- -- -- -- -- --
  $000010: ZZ WW -- -- -- -- -- -- -- -- -- -- -- -- -- --
  
  NOTE: In this context when I refer to "first", "second" and so on bits, I mean reading the byte
  from left to right as it appears in the hex editor. This means in 0x40, the second bit is set, but all others
  are unset.

  The first bit of each byte XX, YY, ZZ, and WW will combine for form a color index

  Let's say XX = 0x00, YY = 0x80, ZZ = 0x80, WW = 0x00.

  To form the color index, we take the first bit of XX, which is 0, and put it in the lowest
  bit position (the 1's place). We then take the first bit of YY, which is 1, and put it
  in the next position (the 2's place).  Continuing in order, we obtain:
  
  0 0 0 0 0 1 1 0 = 0x03
          | | | |
          | | | \ from XX
          | | |
          | | \-- from YY
          | |   
          | \---- from ZZ
          |
          \------ from WW

  What does this mean? It means this tile's first pixel will use color number 0x03.
  We don't really know what color that is in terms of RBG, b/c the SNES determines
  that later by loading a palette. But when this graphic is used, color number three
  will get used for this pixel.

Notes on dungeon editing:

^-First the header is loaded from $27502.

-Next a pointer to data for the room is loaded from $1F:8000, X
which in the rom is at $F8000, X. The offset is stored at $B7. The first two
bytes contain information about the room. The remainder is a series of objects 
to be drawn into the room. This is described in the Data Offsets Guide fairly thoroughly

-What we'll need to draw these rooms:
    -One large buffer that represents VRAM
    -Two Backgrounds, each with a graphical tile set of 0x200 tiles
    -Two tile maps, each 0x2000 bytes long. (64 tiles by 64 tiles)
    -That means 512 pixels x 512 pixels
    -So uh... actual graphics data can be as large as :
        - 0x20 bytes per 4bpp tile
        - Tilemaps let you select up to $400 different tiles
        - Do the math, that's $8000 bytes of VRAM (half of VRAM) for the graphics
        - Data. That's a fuckload, considering that's just for one tilemap.
    -A buffer for CGRAM (512 bytes)
    -2 large DIBs to generate the screen image.
    -A set of graphical tile data for the sprites
    -A way to display each sprite over the backgrounds.
  

**************************************************************

  So how am I going to handle data importing and exporting, not to mention allocating
  it into a spare bank? On top of that I might have to expand a users rom...

  CheckList:

Detailed list of progress on editing goals:

1. Move / Expand Data
    a. Rom Expansion
       -User selectable, but meant to be safe enough to avoid data corruption
       -Learn how to undo HM's expansion scheme, but at the same time avoid it getting messed up again if HM is used to open the rom later.

    b. Overworld
       - stock objects (finished)
       - custom objects (not started)
       - holes (finished)
       - entrances (finished)
       - exits (finished)
       - items (wip)
       - sprites (wip)
       - whirlpool locations (not started)
       - bird transport locations (not started)
       - arbitrary markers with data (not started)
       - overlays (not started)
  
    c. Dungeons
       - Note: The dungeon's map graphic will be editable from the room itself.
       - RATS is a go!
       - File name will be the rom's name with a .bmd extension.
       - Optionally ask to load all or a range / certain rooms.
       - Headers
       - Entrance Data (should I allow for the creation of extra entrances? yes)
       - Object and Layout Data (expand)
       - Sprites (also expand)

    d. Graphics
       - Graphics are exported as individual files for now.
       - Importing (wip)

2. Editing various HM unsupported things
    a. Weather Vane
    b. Dungeon Opening Sequences (misery mire etc)

3. Tile editing
    a. Standard Decompression (X)
    b. Other Decompression (X)
    c. Conversion to Bitmaps (X)
    d. palettes? (X)

4. Standard Editing
    a. Dungeons
        -dungeon headers (X)
        -displaying graphics ()
        ABANDON SCROLLING CONTROLS for now... ;(
        -dungeon Objects (wip)
         
        Discussion:

        After reflecting on some prior attempts I've concluded I should map out the life of an object

        When an object is created, its tiles are mapped into a scratch ram area which
        corresponds to BG0 and BG1 (two scratch ram areas you should conclude)

        Also, I will need an object registry not just for BG1 but also for BG0 (some objects use both)

        As the tile is being loaded, a maximum and a minimum tilemap address is updated
        This is used to form a rectangle. doObj->height and doObj->width are also initialized

        After we know the rect and its dimensions, we can initialize a miniature tilemap
        for each of the BGs the object is present on. We also need to set which tiles that
        are copied over from the scratch ram area belong to the object. Otherwise you might
        get garbage.

5. Graphics

    We need to figure out what all of the different graphics packs are compressed with and how they're handled:

    c denotes "compressed"
    4bpp means that once the data is decompressed it is availble for use as 4bpp graphics data
    3bpp data will have to be upconverted to 4bpp graphics before being used. There are two
    methods for doing this, the "other" method corresponding to DecompressOther
    and without an extra label we just use Convert3bppTo4bpp()

    sourceType1:

        0x00        : nothing
        0x01        : 3bpp other
        0x02 - 0x05 : c 3bpp
        
        0x06        : 3bpp

        0x07 - 0x09 : c 3bpp
        
        0x0A - 0x0B : 3bpp other

        0x0C - 0x51 : c 3bpp

        0x52        : c 3bpp other
        0x53        : c 3bpp other

        0x54 - 0x59 : c 3bpp

        0x5A        : c 3bpp other
        0x5B        : c 3bpp other
        0x5C        : c 3bpp other
        0x5E        : c 3bpp other
        0x5F        : c 3bpp other       

        0x60 - ???  : c 3bpp

    sourceType2:

*/