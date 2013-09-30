#define _BSD_SOURCE

#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>


int main(int argc, char const* argv[]) {
    /* ncurses 初期化 */
    if(initscr() == NULL){
        fprintf(stderr, "initscr failure\n");
        exit(EXIT_FAILURE);
    }

    /* シグナルを無視する */
    /* signal(SIGINT, SIG_IGN); */
    /* キー入力された文字を表示しない */
    noecho();
    /* カーソルを表示しない */
    leaveok(stdscr, TRUE);
    /* スクロールしない */
    scrollok(stdscr, FALSE);

    const char* bird1[] = {
        "   .--.                      ",
        "__/ o  \".                   ",
        "  |,    \"-._                ",
        "  |;;,      \"-._            ",
        "  ';;;,,    \",_ \"=------.  ",
        "    ':;;;;,,..-``\"-----'\"` ",
        "       _|_|                  ",
    };

    const char* bird2[] = {
        "   .--.                      ",
        "__/ ^  \".                   ",
        "  |,    \"-._                ",
        "  |;;,      \"-._            ",
        "  ';;;,,    \",_ \"=-._      ",
        "    ':;;;;,,..-``\"-._`\"-.  ",
        "     _/   _\\          `'\"` ",
    };

    const int max_width = 29;
    const int bird_line_size = sizeof(bird1) / sizeof(char*);

    printw("%d\n", bird_line_size);

    const int base_line = LINES / 2;

    // 床
    mvhline(base_line, 0, '^', COLS - 1);

    int cnt = 0;
    const char** draw = bird1;

    for (int x = COLS - max_width - 1; 0 <= x; --x) {
        mvprintw(1, 0, "%d\n", x);
        for (int y = 0; y < bird_line_size; ++y) {
            if (++cnt == 2) {
                cnt = 0;
                draw = (draw == bird1) ? (bird2) : (bird1);
            }

            mvprintw(base_line - bird_line_size + y, x, "%s\n", draw[y]);
        }
        refresh();
        usleep(100000 * 2); // 0.1s
        /* usleep(40000); */
    }

    endwin();

    return 0;
}
