#define _BSD_SOURCE

#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <signal.h>

/* 0~255を0~1000に変換する */
#define CONVERT_RANGE(c) ((int)(((double)c / 255.0) * 1000.0))


enum {
    BN_COLOR_BLACK = COLOR_BLACK,
    BN_COLOR_RED,
    BN_COLOR_BLUE,
    BN_COLOR_GREEN,
    BN_COLOR_PAIR_WG = 1,
    BN_COLOR_PAIR_BG,
    BN_COLOR_PAIR_GG,
    BN_COLOR_PAIR_DEFAULT = BN_COLOR_PAIR_BG,
    MAX_AA_HEIGHT = 7,
    MAX_AA_WIDTH = 29,
};


/* 表示用のAA */
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


void signal_handler(int);
static inline void init_bn(void);
static inline void quit_bn(void);


int main(int argc, char const* argv[]) {
    init_bn();

    /* 基準面 */
    const int base_line = LINES / 2;

    /* 基準面描画 */
    attrset(COLOR_PAIR(BN_COLOR_PAIR_GG));
    mvhline(base_line, 0, '^', COLS - 1);
    refresh();

    int cnt = 0;
    const char** draw = bird[0];

    attrset(COLOR_PAIR(BN_COLOR_PAIR_DEFAULT));

    /* 右から左へ */
    for (int x = COLS - MAX_AA_WIDTH - 1; 0 <= x; --x) {
        /* for debug */
        mvprintw(0, 0, "%d\n", x);

        /* 2フレームごとにAAを切り替え */
        if (++cnt == 2) {
            cnt = 0;
            draw = (draw == bird[0]) ? (bird[1]) : (bird[0]);
        }

        /* AAの上方から描画 */
        for (int y = 0; y < MAX_AA_HEIGHT; ++y) {
            mvprintw(base_line - MAX_AA_HEIGHT + y, x, "%s\n", draw[y]);
        }
        refresh();

        usleep(100000); // 0.1s
    }

    quit_bn();

    return 0;
}


/* 初期化 */
static inline void init_bn(void) {
    if (initscr() == NULL){
        fprintf(stderr, "initscr failure\n");
        exit(EXIT_FAILURE);
    }

    /* 端末が色の使用と変更が可能なら設定 */
    start_color();

    /* 色の初期化 */
    /* 色番号(1 ~ COLORSの間とする) r, g, b, それぞれ0~1000の範囲 */
    init_color(BN_COLOR_RED,    CONVERT_RANGE(215), CONVERT_RANGE(0),   CONVERT_RANGE(58));
    init_color(BN_COLOR_GREEN,  CONVERT_RANGE(56),  CONVERT_RANGE(180), CONVERT_RANGE(139));
    init_color(BN_COLOR_BLUE,   CONVERT_RANGE(44),  CONVERT_RANGE(169), CONVERT_RANGE(225));
    init_color(BN_COLOR_BLACK,   CONVERT_RANGE(43),  CONVERT_RANGE(43),  CONVERT_RANGE(43));

    /* 前景と背景のペアを作成 */
    /* 色番号(1 ~ COLOR_PAIRS-1の範囲であること) 前景 背景 */
    init_pair(BN_COLOR_PAIR_WG, COLOR_WHITE, BN_COLOR_BLACK);
    init_pair(BN_COLOR_PAIR_BG, BN_COLOR_BLUE, BN_COLOR_BLACK);
    init_pair(BN_COLOR_PAIR_GG, BN_COLOR_GREEN, BN_COLOR_BLACK);

    if (SIG_ERR == signal(SIGQUIT, signal_handler)) {
        fprintf(stderr, "setting signal handler failure\n");
        endwin();
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
    /* カーソルを非表示に */
    curs_set(0);
}


/* 終了処理 */
static inline void quit_bn(void) {
    curs_set(1);
    endwin();
}


/* シグナルを受け取った時に呼ばれる */
void signal_handler(int sig) {
    if (sig != SIGINT) {
        return;
    }

    /* 既にendwinは呼ばれたか */
    if (isendwin()) {
        return;
    }

    /* 中断されたときでも終了処理を行う */
    quit_bn();
}
