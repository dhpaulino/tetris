/***************************************************************************//**

  @file         tetris.c

  @author       Stephen Brennan

  @date         Created Wednesday, 10 June 2015

  @brief        Tetris game logic.

  @copyright    Copyright (c) 2015, Stephen Brennan.  Released under the Revised
                BSD License.  See LICENSE.txt for details.

*******************************************************************************/
#include "tetris_game.hpp"  
#include <iostream>
#include <array>

namespace tetris{

  #define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
  #define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

  // TODO: add the file handle functions again
  /*******************************************************************************

                                Array Definitions

  *******************************************************************************/





  /*******************************************************************************

                            Helper Functions for Blocks

  *******************************************************************************/


  int tetris_game::get_rows() const{
    return this->rows;
  }
  int tetris_game::get_cols() const{
    return this->cols;
  }

  int tetris_game::get_points() const{
    return this->points;
  }
  int tetris_game::get_level() const{
    return this->level;
  }


  tetris_block tetris_game::get_falling() const{
    return this->falling;
  }
  tetris_block tetris_game::get_next() const{
    return this->next;
  }
  tetris_block tetris_game::get_stored() const{
    return this->stored;
  }

  int tetris_game::get_ticks_till_gravity() const{
    return this->ticks_till_gravity;
  }
  int tetris_game::get_lines_remaining() const{
    return this->lines_remaining;
  }




  /*
    Return the block at the given row and column.
  */
  char tetris_game::tg_get (int row, int column) const
  {
    return board[this->cols * row + column];
  }

  /*
    Set the block at the given row and column.
  */
  void tetris_game::tg_set(int row, int column, char value)
  {
    board[this->cols * row + column] = value;
  }

  /*
    Check whether a row and column are in bounds.
  */
  bool tetris_game::tg_check (int row, int col) const
  {
    return 0 <= row && row < this->rows && 0 <= col && col < this->cols;
  }

  /*
    Place a block onto the board.
  */
  void tetris_game::tg_put(tetris_block block)
  {
    int i;
    for (i = 0; i < TETRIS; i++) {
      tetris_location cell = TETROMINOS[block.typ][block.ori][i];
      tg_set(block.loc.row + cell.row, block.loc.col + cell.col,
            TYPE_TO_CELL(block.typ));
    }
  }

  /*
    Clear a block out of the board.
  */
  void tetris_game::tg_remove(tetris_block block)
  {
    int i;
    for (i = 0; i < TETRIS; i++) {
      tetris_location cell = TETROMINOS[block.typ][block.ori][i];
      tg_set(block.loc.row + cell.row, block.loc.col + cell.col, TC_EMPTY);
    }
  }

  /*
    Check if a block can be placed on the board.
  */
  bool tetris_game::tg_fits (tetris_block block) const
  {
    int i, r, c;
    for (i = 0; i < TETRIS; i++) {
      tetris_location cell = TETROMINOS[block.typ][block.ori][i];
      r = block.loc.row + cell.row;
      c = block.loc.col + cell.col;
      if (!tg_check(r, c) || TC_IS_FILLED(tg_get(r, c))) {
        return false;
      }
    }
    return true;
  }

  /*
    Return a random tetromino type.
  */
  int tetris_game::random_tetromino() {
    return rand() % NUM_TETROMINOS;
  }

  /*
    Create a new falling block and populate the next falling block with a random
    one.
  */
  void tetris_game::tg_new_falling()
  {
    // Put in a new falling tetromino.
    falling = next;
    next.typ = tetris_game::random_tetromino();
    next.ori = 0;
    next.loc.row = 0;
    next.loc.col = cols/2 - 2;
  }

  /*******************************************************************************

                                Game Turn Helpers

  *******************************************************************************/

  /*
    Tick gravity, and move the block down if gravity should act.
  */
  void tetris_game::tg_do_gravity_tick()
  {
    ticks_till_gravity--;
    if (ticks_till_gravity <= 0) {
      tg_remove(falling);
      falling.loc.row++;
      if (tg_fits(falling)) {
        ticks_till_gravity = GRAVITY_LEVEL[level];
      } else {
        falling.loc.row--;
        tg_put(falling);

        tg_new_falling();
      }
      tg_put(falling);
    }
  }


  /*
    Move the falling tetris block left (-1) or right (+1).
  */
  void tetris_game::tg_move(int direction)
  {
    tg_remove(falling);
    falling.loc.col += direction;
    if (!tg_fits(falling)) {
      falling.loc.col -= direction;
    }
    tg_put(falling);
  }

  /*
    Send the falling tetris block to the bottom.
  */
  void tetris_game::tg_down()
  {
    tg_remove(falling);
    while (tg_fits(falling)) {
      falling.loc.row++;
    }
    falling.loc.row--;
    tg_put(falling);
    tg_new_falling();
  }

  /*
    Rotate the falling block in either direction (+/-1).
  */
  void tetris_game::tg_rotate(int direction)
  {
    tg_remove(falling);

    while (true) {
      falling.ori = (falling.ori + direction) % NUM_ORIENTATIONS;

      // If the new orientation fits, we're done.
      if (tg_fits(falling))
        break;

      // Otherwise, try moving left to make it fit.
      falling.loc.col--;
      if (tg_fits(falling))
        break;

      // Finally, try moving right to make it fit.
      falling.loc.col += 2;
      if (tg_fits(falling))
        break;

      // Put it back in its original location and try the next orientation.
      falling.loc.col--;
      // Worst case, we come back to the original orientation and it fits, so this
      // loop will terminate.
    }

    tg_put(falling);
  }

  /*
    Swap the falling block with the block in the hold buffer.
  */
  void tetris_game::tg_hold()
  {
    tg_remove(falling);
    if (stored.typ == -1) {
      stored = falling;
      tg_new_falling();
    } else {
      int typ = falling.typ, ori = falling.ori;
      falling.typ = stored.typ;
      falling.ori = stored.ori;
      stored.typ = typ;
      stored.ori = ori;
      while (!tg_fits(falling)) {
        falling.loc.row--;
      }
    }
    tg_put(falling);
  }

  /*
    Perform the action specified by the move.
  */
  void tetris_game::tg_handle_move(tetris_move move)
  {
    switch (move) {
    case TM_LEFT:
      tg_move(-1);
      break;
    case TM_RIGHT:
      tg_move(1);
      break;
    case TM_DROP:
      tg_down();
      break;
    case TM_CLOCK:
      tg_rotate(1);
      break;
    case TM_COUNTER:
      tg_rotate(-1);
      break;
    case TM_HOLD:
      tg_hold();
      break;
    default:
      // pass
      break;
    }
  }

  /*
    Return true if line i is full.
  */
  bool tetris_game::tg_line_full (int i) const
  {
    int j;
    for (j = 0; j < cols; j++) {
      if (TC_IS_EMPTY(tg_get(i, j)))
        return false;
    }
    return true;
  }

  /*
    Shift every row above r down one.
  */
  void tetris_game::tg_shift_lines(int r)
  {
    int i, j;
    for (i = r-1; i >= 0; i--) {
      for (j = 0; j < cols; j++) {
        tg_set(i+1, j, tg_get(i, j));
        tg_set(i, j, TC_EMPTY);
      }
    }
  }

  /*
    Find rows that are filled, remove them, shift, and return the number of
    cleared rows.
  */
  int tetris_game::tg_check_lines()
  {
    int i, nlines = 0;
    tg_remove(falling); // don't want to mess up falling block

    for (i = rows-1; i >= 0; i--) {
      if (tg_line_full(i)) {
        tg_shift_lines(i);
        i++; // do this line over again since they're shifted
        nlines++;
      }
    }

    tg_put(falling); // replace
    return nlines;
  }

  /*
    Adjust the score for the game, given how many lines were just cleared.
  */
  void tetris_game::tg_adjust_score(int lines_cleared)
  {
    static std::array<int, 5> line_multiplier = {0, 40, 100, 300, 1200};
    points += line_multiplier[lines_cleared] * (level + 1);
    if (lines_cleared >= lines_remaining) {
      level = MIN(MAX_LEVEL, level + 1);
      lines_cleared -= lines_remaining;
      lines_remaining = LINES_PER_LEVEL - lines_cleared;
    } else {
      lines_remaining -= lines_cleared;
    }
  }

  /*
    Return true if the game is over.
  */
  bool tetris_game::tg_game_over()
  {
    int i, j;
    bool over = false;
    tg_remove(falling);
    for (i = 0; i < 2; i++) {
      for (j = 0; j < cols; j++) {
        if (TC_IS_FILLED(tg_get(i, j))) {
          over = true;
        }
      }
    }
    tg_put(falling);
    return over;
  }

  /*******************************************************************************

                              Main Public Functions

  *******************************************************************************/

  /*
    Do a single game tick: process gravity, user input, and score.  Return true if
    the game is still running, false if it is over.
  */
  bool tetris_game::tg_tick(tetris_move move)
  {
    int lines_cleared;
    // Handle gravity.
    tg_do_gravity_tick();

    // Handle input.
    tg_handle_move(move);

    // Check for cleared lines
    lines_cleared = tg_check_lines();

    tg_adjust_score(lines_cleared);

    // Return whether the game will continue (NOT whether it's over)
    return !tg_game_over();
  }

  tetris_game::tetris_game(int rows, int cols){
      this->rows = rows;
      this->cols = cols;
      board = std::string(this->rows * this->cols, TC_EMPTY);
      points = 0;
      level = 0;
      ticks_till_gravity = GRAVITY_LEVEL[level];
      lines_remaining = LINES_PER_LEVEL;
      //TODO: change random generator
      srand(time(nullptr));
      this->tg_new_falling();
      this->tg_new_falling();
      stored.typ = -1;
      stored.ori = 0;
      stored.loc.row = 0;
      next.loc.col = this->cols/2 - 2;
      std::cout << falling.loc.col;
  }

  /*void tg_destroy()
  {
    // Cleanup logic
    // free(board);
  }

  void tg_delete(tetris_game *obj) {
    // tg_destroy();
    // free();
  }
  */
  // /*
  //   Load a game from a file.
  //  */
  // tetris_game *tg_load(FILE *f)
  // {
  //   tetris_game *obj = (tetris_game*) malloc(sizeof(tetris_game));
  //   fread(sizeof(tetris_game), 1, f);
  //   board = (char*) malloc(rows * cols);
  //   fread(board, sizeof(char), rows * cols, f);
  //   return obj;
  // }

  // /*
  //   Save a game to a file.
  //  */
  // void tg_save(tetris_game *obj, FILE *f)
  // {
  //   fwrite(obj, sizeof(tetris_game), 1, f);
  //   fwrite(board, sizeof(char), rows * cols, f);
  // }

  /*
    Print a game board to a file.  Really just for early debugging.
  */
  // void tg_print(tetris_game *obj, FILE *f) {
  //   int i, j;
  //   for (i = 0; i < rows; i++) {
  //     for (j = 0; j < cols; j++) {
  //       if (TC_IS_EMPTY(tg_get(obj, i, j))) {
  //         fputs(TC_EMPTY_STR, f);
  //       } else {
  //         fputs(TC_BLOCK_STR, f);
  //       }
  //     }
  //     fputc('\n', f);
  //   }
  // }
}