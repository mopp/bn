#define _BSD_SOURCE

#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>


enum {
    MAX_AA_HEIGHT   = 7,
    MAX_AA_WIDTH    = 29,
};


// 表示用のAA
static const char* bird[][MAX_AA_HEIGHT] = {
    {
        "   .--.                      ",
        "__/ o  \".                   ",
        "  |,    \"-._                ",
        "  |;;,      \"-._            ",
        "  ';;;,,    \",_ \"=------.  ",
        "    ':;;;;,,..-``\"-----'\"` ",
        "       _|_|                  ",
    },
    {
        "   .--.                      ",
        "__/ ^  \".                   ",
        "  |,    \"-._                ",
        "  |;;,      \"-._            ",
        "  ';;;,,    \",_ \"=-._      ",
        "    ':;;;;,,..-``\"-._`\"-.  ",
        "     _/   _\\          `'\"` ",
    },
};


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

    // 基準面
    const int base_line = LINES / 2;

    // 基準面描画
    mvhline(base_line, 0, '^', COLS - 1);
    refresh();

    int cnt = 0;
    const char** draw = bird[0];

    // 右から左へ
    for (int x = COLS - MAX_AA_WIDTH - 1; 0 <= x; --x) {
        // for debug
        mvprintw(0, 0, "%d\n", x);

        // 2フレームごとにAAを切り替え
        if (++cnt == 2) {
            cnt = 0;
            draw = (draw == bird[0]) ? (bird[1]) : (bird[0]);
        }

        // AAの上方から描画
        for (int y = 0; y < MAX_AA_HEIGHT; ++y) {
            mvprintw(base_line - MAX_AA_HEIGHT + y, x, "%s\n", draw[y]);
        }

        refresh();
        usleep(100000); // 0.1s
    }

    endwin();

    return 0;
}
