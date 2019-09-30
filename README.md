This is a simplified version of the code by Stephen Brennan.
All copyrights belong to him!!



Tetris
======

![tetris.gif](tetris.gif)

A tetris game in C using NCURSES.  It's pretty feature complete, except for
stretch goals I may pick up in my free time.


Building
--------

My dependencies are:

* `libsdl` and `libsdl_mixer` 1.2 for sound.
* `ncurses` for terminal manipulation.

To install them on Arch Linux:

    sudo pacman -S sdl_mixer ncurses

To install them on Ubuntu:

    sudo apt-get install libsdl-mixer1.2-dev libncurses5-dev

To compile:

    make

To run:

    bin/release/main

Instructions
------------

The controls are typical of Tetris:
* `left` and `right`: Move the tetromino,
* `up`: Rotate (clockwise?) the tetromino,
* `down`: Immediately drop the tetromino (not a fast drop, an immediate drop),
* `q`: Exit the game prematurely,
* `p`: Pause the game (any key to resume)