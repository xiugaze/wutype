#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

#include "wutype.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));
    int dict_size = 0;
    int num_words = 30;
    gamemode mode = EASY;
    char *filename = malloc(sizeof(char) * 100);

    int opt;
    while ((opt = getopt(argc, argv, "n:hcwtCD")) != -1) {
        switch (opt) {
            case 'n':
                num_words = atoi(optarg);
                break;
            case 'h':
                print_help();
                exit(0);
            case 'w':
                mode = WU;
                break;
            case 'c':
                mode = HARD;
                break;
            case 'C':
                mode = CHRIS;
                break;
            case 't':
                mode = TAYLOR;
                break;
        }
    }

    switch(mode) {
        case EASY:
            strcpy(filename, "./words/simple.txt");
            dict_size = 1000;
            break;
        case HARD: 
            strcpy(filename, "./words/complex.txt");
            dict_size = 1000;
            break;
        case TAYLOR: 
            strcpy(filename, "./words/taylor.txt");
            dict_size = 113809;
            break;
        case CHRIS: 
            strcpy(filename, "./words/chris.txt");
            dict_size = 26;
            break;
        case WU:
            num_words = 100;
            break;
    }

    window_size size = init_curses();
    int cursor_start = (int)(0.15 * size.mx);
    int cursor_end = (int)(0.70 * size.mx);



    // const char *originalText = "Hello, this is a sample line!";
    char *display_text = calloc(30 * num_words, sizeof(char));

    if(mode != WU) {
        for (int i = 0; i < num_words; i++) {
            char *word = malloc(sizeof(char) * 30);
            get_line(filename, word, 30, dict_size);
            strcat(display_text, word);
            free(word);
        }
    } else {
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);
        int file_number = rand() % 5;
        switch (file_number) {
            case 0:
                readFileToString("./words/ruckus.txt", display_text, num_words*30);
                break;
            case 1:
                readFileToString("./words/cream.txt", display_text, num_words*30);
                break;
            case 2:
                readFileToString("./words/method.txt", display_text, num_words*30);
                break;
            case 3:
                readFileToString("./words/clan.txt", display_text, num_words*30);
                break;
            case 4:
                readFileToString("./words/liquid.txt", display_text, num_words*30);
                break;
        }
    }

    int length = strlen(display_text);
    character_state *state = calloc(length, sizeof(character_state));

    startcard(size.my, size.mx, mode);

    int cursorPosition = 0;
    time_t start_t, end_t;
    double diff;

    int started = 0;
    int misses = 0;
    while (1) {
        clear(); // Clear the screen
        // Print the line with cursor and highlight
        print_frame(display_text, state, cursorPosition, cursor_start,
                    cursor_end);

        int userInput = getch(); // Get user input

        if (!started) {
            time(&start_t);
            started = !started;
        }

        if (userInput == KEY_BACKSPACE && cursorPosition >= 0) {
            cursorPosition--;
            state[cursorPosition] = UNTYPED;
        } else {
            if (cursorPosition < strlen(display_text) &&
                display_text[cursorPosition] != userInput) {
                state[cursorPosition] = INCORRECT;
                misses++;
            } else {
                state[cursorPosition] = CORRECT;
            }
            cursorPosition++;

            print_frame(display_text, state, cursorPosition, cursor_start,
                        cursor_end);
        }

        if (cursorPosition == length - 1) {
            break;
        }
    }
    clear();

    time(&end_t);
    diff = difftime(end_t, start_t);

    int wrong = compute_accuracy(state, length);
    endcard(diff, length, wrong, misses, size.mx, size.my);

    free(filename);
    free(state);
    free(display_text);
    endwin(); // End NCurses
}
