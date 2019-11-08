#pragma once

#include "tetris_location.h"
/*
  A "block" is a struct that contains information about a tetromino.
  Specifically, what type it is, what orientation it has, and where it is.
 */
class tetris_block {
  public:
    int typ;
    int ori;
    tetris_location loc;
};