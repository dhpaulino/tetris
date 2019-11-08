/***************************************************************************//**

  @file         tetris_game.h

  @author       Stephen Brennan

  @date         Created Wednesday, 10 June 2015

  @brief        Tetris game declarations.

  @copyright    Copyright (c) 2015, Stephen Brennan.  Released under the Revised
                BSD License.  See LICENSE.txt for details.

*******************************************************************************/
#pragma once

#include "tetris_block.h"
#include <string>

//TODO:create namespace
/*
  Convert a tetromino type to its corresponding cell.
 */
#define TYPE_TO_CELL(x) ((x)+1)

/*
  Strings for how you would print a tetris board.
 */
#define TC_EMPTY_STR " "
#define TC_BLOCK_STR "\u2588"

/*
  Questions about a tetris cell.
 */
#define TC_IS_EMPTY(x) ((x) == TC_EMPTY)
#define TC_IS_FILLED(x) (!TC_IS_EMPTY(x))

/*
  How many cells in a tetromino?
 */
#define TETRIS 4
/*
  How many tetrominos?
 */
#define NUM_TETROMINOS 7
/*
  How many orientations of a tetromino?
 */
#define NUM_ORIENTATIONS 4

/*
  Level constants.
 */
#define MAX_LEVEL 19
#define LINES_PER_LEVEL 10

/*
  A "cell" is a 1x1 block within a tetris board.
 */
typedef enum {
  TC_EMPTY, TC_CELLI, TC_CELLJ, TC_CELLL, TC_CELLO, TC_CELLS, TC_CELLT, TC_CELLZ
} tetris_cell;

/*
  A "type" is a type/shape of a tetromino.  Not including orientation.
 */
typedef enum {
  TET_I, TET_J, TET_L, TET_O, TET_S, TET_T, TET_Z
} tetris_type;



/*
  All possible moves to give as input to the game.
 */
typedef enum {
  TM_LEFT, TM_RIGHT, TM_CLOCK, TM_COUNTER, TM_DROP, TM_HOLD, TM_NONE
} tetris_move;

/*
  A game object!
 */
class tetris_game {

  private:
    /*
      Game board stuff:
    */
    int rows;
    int cols;
    std::string board; 
     /*
      Scoring information:
    */
    int points;
    int level;
      /*
      Falling block is the one currently going down.  Next block is the one that
      will be falling after this one.  Stored is the block that you can swap out.
    */
    tetris_block falling;
    tetris_block next;
    tetris_block stored;
    /*
      Number of game ticks until the block will move down.
    */
    int ticks_till_gravity;
    /*
      Number of lines until you advance to the next level.
    */
    int lines_remaining;

    bool tg_fits (tetris_block block) const;
    void tg_set(int row, int column, char value);
    void tg_put(tetris_block block);
    void tg_remove(tetris_block block);
    void tg_move(int direction);
    void tg_down();
    void tg_rotate(int direction);
    void tg_hold();
    void tg_handle_move(tetris_move move);
    bool tg_line_full (int i) const;
    void tg_shift_lines(int r);
    int tg_check_lines();
    void tg_adjust_score(int lines_cleared);
    bool tg_game_over();
    static int random_tetromino(void);

  public:
    int get_rows() const;
    int get_cols() const;
    int get_points() const;
    int get_level() const; 
    tetris_block get_falling() const;
    tetris_block get_next() const;
    tetris_block get_stored() const;
    int get_ticks_till_gravity() const;
    int get_lines_remaining() const;
   
  

    tetris_game(int rows, int cols);
    void tg_new_falling();
    void tg_do_gravity_tick();
    // Data structure manipulation.
    void tg_init(int rows, int cols);
    tetris_game *tg_create(int rows, int cols);
    void tg_destroy();
    void tg_delete();
    // tetris_game *tg_load(FILE *f);
    // void tg_save(FILE *f);

    // Public methods not related to memory:
    char tg_get(int row, int col) const;
    bool tg_check(int row, int col) const;
    bool tg_tick(tetris_move move);
    // void tg_print(FILE *f);

};


/*
  This array stores all necessary information about the cells that are filled by
  each tetromino.  The first index is the type of the tetromino (i.e. shape,
  e.g. I, J, Z, etc.).  The next index is the orientation (0-3).  The final
  array contains 4 tetris_location objects, each mapping to an offset from a
  point on the upper left that is the tetromino "origin".
 */
 constexpr tetris_location TETROMINOS[NUM_TETROMINOS][NUM_ORIENTATIONS][TETRIS] = {
  // I
  {{{1, 0}, {1, 1}, {1, 2}, {1, 3}},
   {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
   {{3, 0}, {3, 1}, {3, 2}, {3, 3}},
   {{0, 1}, {1, 1}, {2, 1}, {3, 1}}},
  // J
  {{{0, 0}, {1, 0}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {2, 1}},
   {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
   {{0, 1}, {1, 1}, {2, 0}, {2, 1}}},
  // L
  {{{0, 2}, {1, 0}, {1, 1}, {1, 2}},
   {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
   {{1, 0}, {1, 1}, {1, 2}, {2, 0}},
   {{0, 0}, {0, 1}, {1, 1}, {2, 1}}},
  // O
  {{{0, 1}, {0, 2}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {1, 2}}},
  // S
  {{{0, 1}, {0, 2}, {1, 0}, {1, 1}},
   {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
   {{1, 1}, {1, 2}, {2, 0}, {2, 1}},
   {{0, 0}, {1, 0}, {1, 1}, {2, 1}}},
  // T
  {{{0, 1}, {1, 0}, {1, 1}, {1, 2}},
   {{0, 1}, {1, 1}, {1, 2}, {2, 1}},
   {{1, 0}, {1, 1}, {1, 2}, {2, 1}},
   {{0, 1}, {1, 0}, {1, 1}, {2, 1}}},
  // Z
  {{{0, 0}, {0, 1}, {1, 1}, {1, 2}},
   {{0, 2}, {1, 1}, {1, 2}, {2, 1}},
   {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
   {{0, 1}, {1, 0}, {1, 1}, {2, 0}}},
};

/*
  This array tells you how many ticks per gravity by level.  Decreases as level
  increases, to add difficulty.
 */
constexpr int GRAVITY_LEVEL[MAX_LEVEL+1] = {
// 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
  50, 48, 46, 44, 42, 40, 38, 36, 34, 32,
//10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
  30, 28, 26, 24, 22, 20, 16, 12,  8,  4
};

