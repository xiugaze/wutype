#include <bits/time.h>
#include <locale.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>
#include <time.h>
// #define TOP_LEFT L'\u256D'
// #define TOP_RIGHT L'\u256E'
// #define BOTTOM_RIGHT L'\u256F'
// #define BOTTOM_LEFT L'\u2570'
// #define VERTICAL L'\u2503'
// #define lHORIZONTAL L'\u2500'

typedef enum {
    UNTYPED = 0,
    INCORRECT,
    CORRECT,
} character_state;

void print_line(const char *text, character_state *state, int cursorPosition) {
    move(0, 0); // Move to the start of the line
    for (int i = 0; text[i] != '\0'; i++) {
        if (state[i] == INCORRECT) {
            if (text[i] == ' ') {

                attron(A_UNDERLINE); // Underline the cursor position
            }
            attron(COLOR_PAIR(1)); // Turn on red color for incorrect characters
        } else if (state[i] == CORRECT) {
            attron(COLOR_PAIR(2)); // Turn on white color for
        } else {
            attron(COLOR_PAIR(3)); // turn on gray color for untyped
        }

        if (i == cursorPosition) {
            attron(A_UNDERLINE); // Underline the cursor position
        }

        addch(text[i]);

        if (i == cursorPosition) {
            attroff(A_UNDERLINE); // Turn off underline after printing cursor
                                  // position
        }

        if (text[i] == ' ') {
        }
    }
}

// void print_box(int x, int y, int width, int height) {
//     for(int i = 0; i < width; i++) {
//         mvaddch(y, x + i, HORIZONTAL);
//         mvaddch(y+width, x + i, HORIZONTAL);
//     }
//
// }

int main(int argc, char *argv[]) {
    initscr();            // Initialize NCurses
    start_color();        // Enable color support
    cbreak();             // Disable line buffering
    noecho();             // Don't echo input characters
    keypad(stdscr, TRUE); // Enable special keys
    curs_set(
        0); // Set cursor visibility (0: invisible, 1: visible, 2: very visible)
    // Initialize color pair for red text
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);

    const char *originalText = "Hello, this is a sample line!";
    int length = strlen(originalText);
    character_state *state = calloc(length, sizeof(character_state));

    int cursorPosition = 0;

    time_t t;
    struct tm *tm;
    tm=localtime(&t);

    while (1) {
        clear(); // Clear the screen

        // Print the line with cursor and highlight
        print_line(originalText, state, cursorPosition);

        refresh(); // Update the screen

        int userInput = getch(); // Get user input

        if (userInput == KEY_LEFT && cursorPosition > 0) {
            cursorPosition--; // Move the cursor left
        } else if (userInput == KEY_RIGHT &&
                   cursorPosition < strlen(originalText)) {
            cursorPosition++; // Move the cursor right
        } else if (userInput == 27) { // 27 is the ASCII code for the escape key
            break; // Exit the loop if the escape key is pressed
        } else if (userInput == KEY_BACKSPACE) {
            cursorPosition--;
            state[cursorPosition] = UNTYPED;
        } else {
            if (cursorPosition < strlen(originalText) &&
                originalText[cursorPosition] != userInput) {
                state[cursorPosition] = INCORRECT;
            } else {
                state[cursorPosition] = CORRECT;
            }
            cursorPosition++;

            print_line(originalText, state, cursorPosition);
        }

        if(cursorPosition == length) {
            break;
        }
    }
    clear();

    struct tm* tm_2 = localtime(&t);
    // double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
    //                       (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
    int elapsed_time = tm_2->tm_sec - tm->tm_sec;

    int num_incorrect = 0;
    for(int i = 0; i < length; i++) {
        if(state[i] == INCORRECT) {
            num_incorrect++;
        }
    }


    double accuracy = 100 - num_incorrect/(double)(length)*100;
    mvprintw(4, 4, "Time: %d seconds\nAccuracy: %.2f%%\nMisses: %d", elapsed_time, accuracy, num_incorrect);
    refresh();
    sleep(10);



    endwin(); // End NCurses
}
