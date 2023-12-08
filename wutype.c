#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

#include "wutype.h"
void print_char(const char *text, character_state *state, int cursor_position,
                int position) {

    // FIRST: Choose the color
    if (state[position] == INCORRECT) {
        // Incorrect
        attron(COLOR_PAIR(1)); // incorrect color is red

        // Missed spaces should get underlined
        if (text[position] == ' ') {
            attron(A_UNDERLINE); // Underline the cursor position
        }

    } else if (state[position] == CORRECT) {

        attron(COLOR_PAIR(2)); // correct color is green

    } else { // state[position] == UNTYPED

        attron(COLOR_PAIR(3)); // untyped color is gray
    }

    // if the currently printed character is at the cursor position
    if (position == cursor_position) {
        attron(A_REVERSE); // Underline the cursor position
    }

    // mvprintw(y, x, "%c", text[position]);
    addch(text[position]);

    attroff(A_REVERSE);   // Turn off underline after printing cursor
    attroff(A_UNDERLINE); // Turn off underline after printing cursor
}

void print_frame(const char *text, character_state *state, int cursor_position,
                 int cursor_start, int cursor_end) {
    /*
     * NOTE: cursor_position is the USER cursor, not the ncurses cursor,
     * move(y, x) moves the ncurses "pen"
     */

    int row_start = cursor_start / 2;
    move(row_start, cursor_start); // Move to the top-left corner

    int current_line_length = 0;
    int current_line = 0;
    int start = 0;
    int next = 0;

    while (1) {

        // advance the next pointer until the next space
        while (text[next] != ' ') {
            next++;
        }
        // text[next] == ' '

        int word_length = next - start;

        // if we add the word to the line and it overfills, don't do that
        if (current_line_length + word_length > (cursor_end - cursor_start)) {
            current_line++;
            current_line_length = 0;
            move(row_start + current_line, cursor_start);
        }

        // print the word
        for (int i = start; i < next; i++) {
            print_char(text, state, cursor_position, i);
        }

        current_line_length += word_length;
        start = next + 1;
        next = start;

        if (text[start] == '\0') {
            break;
        } else {
            print_char(text, state, cursor_position, start - 1);
        }
    }
}

int readFileToString(const char *filename, char *buffer, int bufferSize) {
    // Open the file for reading
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return 0;
    }

    size_t bytesRead = fread(buffer, 1, bufferSize - 1, file);
    buffer[bytesRead] = '\0';

    // Close the file
    fclose(file);

    // Remove newlines from the buffer
    for (size_t i = 0; i < bytesRead; ++i) {
        if (buffer[i] == '\n') {
            buffer[i] = ' ';
        }
    }
    return bytesRead;
}

int get_line(char *filename, char *buffer, int size, int dict_size) {

    FILE *file = fopen(filename, "r");
    int line_number = rand() % dict_size;

    for (int i = 1; i < line_number; ++i) {
        if (fgets(buffer, (int)size, file) == NULL) {
            fprintf(stderr, "Error reading file.\n");
            return -1;
        }
    }

    if (fgets(buffer, (int)size, file) == NULL) {
        fprintf(stderr, "Error reading file.\n");
        return -1;
    }

    if (buffer[strlen(buffer) - 1] == '\n') {
        buffer[strlen(buffer) - 1] = ' ';
    }

    fclose(file);

    return strlen(buffer);
}

window_size init_curses() {
    setlocale(LC_ALL, "C.UTF-8");
    initscr();            // Initialize NCurses
    start_color();        // Enable color support
    cbreak();             // Disable line buffering
    noecho();             // Don't echo input characters
    keypad(stdscr, TRUE); // Enable special keys
    curs_set(0);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    return get_window_size();
}
