 /***************************************************************************//**

  @file         main.cpp

  @author       Stephen Brennan

  @date         Created Wednesday, 10 June 2015

  @brief        Main program for tetris.

  @copyright    Copyright (c) 2015, Stephen Brennan.  Released under the Revised
                BSD License.  See LICENSE.txt for details.

*******************************************************************************/


#include "visual_game.hpp"

int main()
{
  tetris::visual_game tetris;
  tetris.run();
  tetris.run();
  return 0;
}
