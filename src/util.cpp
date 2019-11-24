/***************************************************************************//**

  @file         util.c

  @author       Stephen Brennan

  @date         Created Wednesday, 10 June 2015

  @brief        Utilities for Tetris

  @copyright    Copyright (c) 2015, Stephen Brennan.  Released under the Revised
                BSD License.  See LICENSE.txt for details.

*******************************************************************************/
#include <chrono>
#include <thread>
#include "util.hpp"

void sleep_milli(int milliseconds)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}
