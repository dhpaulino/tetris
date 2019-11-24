#pragma once

#include "tetris_location.hpp"
/*
  A "block" is a struct that contains information about a tetromino.
  Specifically, what type it is, what orientation it has, and where it is.
 */
namespace tetris{
  class tetris_block {
    public:
      int typ;
      int ori;
      tetris_location loc;
  };
}