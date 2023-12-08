#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

#include "wutype.h"

void print_box(int y, int x, int height, int width) {
    const static wchar_t *top_left = L"╭";
    const static wchar_t *top_right = L"╮";
    const static wchar_t *bottom_left = L"╰";
    const static wchar_t *bottom_right = L"╯";
    const static wchar_t *vertical = L"│";
    const static wchar_t *horizontal = L"─";

    mvprintw(y, x, "%ls", top_left);
    mvprintw(y, x + width, "%ls", top_right);
    mvprintw(y + height, x, "%ls", bottom_left);
    mvprintw(y + height, x + width, "%ls", bottom_right);

    for (int i = x + 1; i < x + width; i++) {
        mvprintw(y, i, "%ls", horizontal);
        mvprintw(y + height, i, "%ls", horizontal);
    }

    for (int i = y + 1; i < y + height; i++) {
        mvprintw(i, x, "%ls", vertical);
        mvprintw(i, x + width, "%ls", vertical);
    }

    refresh();
}

void draw_logo(int y, int x) {
    const wchar_t* logo1 = L" ⢀⣀⡀⠀⠀⠀⠀⠀⠀⢀⣠⣴⣦⡀";
    const wchar_t* logo2 = L"⢀⣿⣿⣿⣿⣷⠂⠀⡐⢾⣿⣿⣿⣿⣷⠀";
    const wchar_t* logo3 = L"⢸⣿⣿⣿⣿⣿⣄⣿⣿⣼⣿⣿⣿⣿⣿⡇";
    const wchar_t* logo4 = L"⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇";
    const wchar_t* logo5 = L"⠀⢻⣿⣿⣿⣿⣿⡟⠉⢻⣿⣿⣿⣿⡿⠁";
    const wchar_t* logo6 = L"⠀ ⠉⠻⢿⣿⣿⣷⡀⢸⣿⣿⡿⠟⠁⠀";
    const wchar_t* logo7 = L"⠀⠀⠀⠀  ⠀⠈⠁⠿⠛⠉⠀⠀⠀⠀";
    mvprintw(y+1, x, "%ls", logo1);
    mvprintw(y+2, x, "%ls", logo2);
    mvprintw(y+3, x, "%ls", logo3);
    mvprintw(y+4, x, "%ls", logo4);
    mvprintw(y+5, x, "%ls", logo5);
    mvprintw(y+6, x, "%ls", logo6);
    mvprintw(y+7, x, "%ls", logo7);
}

window_size get_window_size() {
    int mx = 0, my = 0;
    getmaxyx(stdscr, my, mx);
    window_size s = {mx, my};
    return s;
}


void startcard(int my, int mx, gamemode mode) {
    if(mode == WU) {
        attron(COLOR_PAIR(2)); // Turn on white color for
        print_box(my / 2 - 12, mx/2 - 16, 4, 28);
        mvprintw(my / 2 - 10, mx/2 - 12, "enter the 36 chambers");
        draw_logo(my/2 - 8, mx / 2 - 9);
    } else {
        print_box(my / 2 - 12, mx / 2 - 6, 4, 10);
        mvprintw(my / 2 - 10, mx / 2 - 3, "start");
    }
    refresh();
    while (1) {
        int input = getch();
        if (input == '\n') {
            break;
        }
    }
}
void endcard(float time, int length, int incorrect, int misses, int mx,
             int my) {
    attron(COLOR_PAIR(2)); // Turn on white color for
    attroff(A_UNDERLINE);  // Turn off underline after printing cursor
    double accuracy = 100 - incorrect / (double)(length) * 100;
    double wpm = ((length / time) * 60) / 5;

    mvprintw(my / 2 - 10, mx / 2 - 10, "WPM: %.2lf", wpm);
    mvprintw(my / 2 - 9, mx / 2 - 10, "Accuracy: %.2f%%", accuracy);
    mvprintw(my / 2 - 8, mx / 2 - 10, "Misses: %d", misses);

    print_box(my / 2 - 12, mx / 2 - 13, 6, 22);

    refresh();
    while (1) {
        int input = getch();
        if (input == '\n') {
            break;
        }
    }
}

int compute_accuracy(character_state *state, int length) {
    int incorrect = 0;
    for (int i = 0; i < length; i++) {
        if (state[i] == INCORRECT) {
            incorrect++;
        }
    }
    return incorrect;
}

void print_help() { 
    printf(
        "wutype: a typing speed test\n"
        "press enter to start\n"
        "options:\n"
        "\t-c: use complex dictionary\n"
        "\t-w: use wu-tang dictionary\n"
        "\t-t: use dr. taylor's CS2852 lab 9 dictionary (warning: contains slurs)\n"
        "\t-C: my roomate chris' favorite words\n"
        "\t-n <number>: specify number of words\n"
    ); 
}


