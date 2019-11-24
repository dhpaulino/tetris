#pragma once
/*
  A row,column pair.  Negative numbers allowed, because we need them for
  offsets.
 */
namespace tetris{
  class tetris_location {
    public:
      int row;
      int col;
  };
}