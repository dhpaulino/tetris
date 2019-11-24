#pragma once
#include <ncurses.h>

#include "tetris_game.hpp"

namespace tetris{
    //2 columns per cell makes the game much nicer.
    constexpr unsigned short COLS_PER_CELL = 2;
    class visual_game{
        private:
            WINDOW *board, *next, *hold, *score;
            tetris_game tg;

            //print a cell of a specific type to a window.
            inline void ADD_BLOCK(WINDOW* w, char x);
            inline void ADD_EMPTY(WINDOW* w);
            void display_board(WINDOW *w, tetris_game& tg);
            // Display a tetris piece in a dedicated window.
            void display_piece(WINDOW* w, tetris_block block);
            // Display score information in a dedicated window.
            void display_score(WINDOW* w, tetris_game& tg);
            // Do the NCURSES initialization steps for color blocks.
            void init_colors();
        public:
            visual_game();
            void run();
            ~visual_game();
    };
}