#include "visual_game.hpp"
#include <ncurses.h>
#include "tetris_game.hpp"
#include "tetris_location.hpp"
#include "util.hpp"

namespace tetris{
    /*
    Macro to print a cell of a specific type to a window.
    */

    inline void visual_game::ADD_BLOCK(WINDOW* w, char x){
        const chtype character = ' '|A_REVERSE|COLOR_PAIR(x);
        waddch((w), character);     
        waddch((w), character);

    }
    inline void visual_game::ADD_EMPTY(WINDOW* w){
        waddch((w), ' '); 
        waddch((w), ' ');
    }

    void visual_game::display_board(WINDOW *w, tetris_game& tg)
    {
    int i, j;
    box(w, 0, 0);
    for (i = 0; i < tg.get_rows(); i++) {
        wmove(w, 1 + i, 1);
        for (j = 0; j < tg.get_cols(); j++) {
        if (TC_IS_FILLED(tg.tg_get(i, j))) {
            ADD_BLOCK(w, tg.tg_get(i, j));
        } else {
            ADD_EMPTY(w);
        }
        }
    }
    wnoutrefresh(w);
    }

    /*
    Display a tetris piece in a dedicated window.
    */
    void visual_game::display_piece(WINDOW* w, tetris_block block)
    {
        int b;
        tetris_location c;
        wclear(w);
        box(w, 0, 0);
        if (block.typ == -1) {
            wnoutrefresh(w);
            return;
        }
        for (b = 0; b < TETRIS; b++) {
            c = TETROMINOS[block.typ][block.ori][b];
            wmove(w, c.row + 1, c.col * COLS_PER_CELL + 1);
            ADD_BLOCK(w, TYPE_TO_CELL(block.typ));
        }
        wnoutrefresh(w);
    }

    /*
    Display score information in a dedicated window.
    */
    void visual_game::display_score(WINDOW* w, tetris_game& tg)
    {
    wclear(w);
    box(w, 0, 0);
    wprintw(w, "Score\n%d\n", tg.get_points());
    wprintw(w, "Level\n%d\n", tg.get_level());
    wprintw(w, "Lines\n%d\n", tg.get_lines_remaining());
    wnoutrefresh(w);
    }

    /*
    Do the NCURSES initialization steps for color blocks.
    */
    void visual_game::init_colors()
    {
    start_color();
    init_pair(TC_CELLI, COLOR_CYAN, COLOR_BLACK);
    init_pair(TC_CELLJ, COLOR_BLUE, COLOR_BLACK);
    init_pair(TC_CELLL, COLOR_WHITE, COLOR_BLACK);
    init_pair(TC_CELLO, COLOR_YELLOW, COLOR_BLACK);
    init_pair(TC_CELLS, COLOR_GREEN, COLOR_BLACK);
    init_pair(TC_CELLT, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(TC_CELLZ, COLOR_RED, COLOR_BLACK);
    }

    visual_game::visual_game() : tg(22,10){
        // create new game.
        // NCURSES initialization:
        initscr();             // initialize curses
        cbreak();              // pass key presses to program, but not signals
        noecho();              // don't echo key presses to screen
        keypad(stdscr, TRUE);  // allow arrow keys
        timeout(0);            // no blocking on getch()
        curs_set(0);           // set the cursor to invisible
        init_colors();         // setup tetris colors

        // Create windows for each section of the interface.
        board = newwin(tg.get_rows() + 2, 2 * tg.get_cols() + 2, 0, 0);
        next  = newwin(6, 10, 0, 2 * (tg.get_cols() + 1) + 1);
        hold  = newwin(6, 10, 7, 2 * (tg.get_cols() + 1) + 1);
        score = newwin(6, 10, 14, 2 * (tg.get_cols() + 1 ) + 1);
    }

    //TODO: enable the game to run multiple times
    void visual_game::run(){
        tetris_move move = TM_NONE;
        bool running = true;
        // Game loop
        while (running) {
            running = tg.tg_tick(move);
            display_board(board, tg);
            display_piece(next, tg.get_next());
            display_piece(hold, tg.get_stored());
            display_score(score, tg);
            doupdate();
            sleep_milli(10);

            switch (getch()) {
                case KEY_LEFT:
                    move = TM_LEFT;
                    break;
                case KEY_RIGHT:
                    move = TM_RIGHT;
                    break;
                case KEY_UP:
                    move = TM_CLOCK;
                    break;
                case KEY_DOWN:
                    move = TM_DROP;
                    break;
                case 'q':
                    running = false;
                    move = TM_NONE;
                    break;
                case 'p':
                    wclear(board);
                    box(board, 0, 0);
                    wmove(board, tg.get_rows()/2, (tg.get_cols()*COLS_PER_CELL-6)/2);
                    wprintw(board, "PAUSED");
                    wrefresh(board);
                    timeout(-1);
                    getch();
                    timeout(0);
                    move = TM_NONE;
                    break;
                case ' ':
                    move = TM_HOLD;
                    break;
                default:
                    move = TM_NONE;
            }
        }

        // Output ending message.
        printf("Game over!\n");
        printf("You finished with %d points on level %d.\n", tg.get_points(), tg.get_level());

    }
    visual_game::~visual_game(){
        // Deinitialize NCurses
        wclear(stdscr);
        endwin();
    }
}