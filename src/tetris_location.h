#pragma once
/*
  A row,column pair.  Negative numbers allowed, because we need them for
  offsets.
 */
class tetris_location {
  public:
    int row;
    int col;
};