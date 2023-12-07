#include <bits/time.h>
#include <locale.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

typedef enum {
    UNTYPED = 0,
    INCORRECT,
    CORRECT,
} character_state;

void endcard(float time, int length, int incorrect, int misses) {
    attron(COLOR_PAIR(2)); // Turn on white color for
    attroff(A_UNDERLINE);  // Turn off underline after printing cursor
    double accuracy = 100 - incorrect / (double)(length) * 100;
    double wpm = ((length / time) * 60) / 5;
    mvprintw(4, 4, "WPM: %.2lf \n\tAccuracy: %.2f%%\n\tMisses: %d", wpm,
             accuracy, misses);
    refresh();
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

void print_line(const char *text, character_state *state, int cursorPosition) {
    move(4, 4); // Move to the start of the line
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

void init_curses() {
    initscr();            // Initialize NCurses
    start_color();        // Enable color support
    cbreak();             // Disable line buffering
    noecho();             // Don't echo input characters
    keypad(stdscr, TRUE); // Enable special keys
    curs_set(0);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
}

int get_line(char *buffer, int size) {

    FILE *file = fopen("./words.txt", "r");

    int line_number = rand() % 1000;

    for (int i = 1; i < line_number; ++i) {
        if (fgets(buffer, (int)size, file) == NULL) {
            fprintf(stderr, "Error reading file.\n");
            return -1;
        }
    }

    if (fgets(buffer, (int)size, file) == NULL) {
        fprintf(stderr, "Error reading file.\n");
        return - 1;
    }
    
    if(buffer[strlen(buffer) - 1] == '\n') {
        buffer[strlen(buffer) - 1] = ' ';
    }

    fclose(file);

    return strlen(buffer);
}

int main(int argc, char *argv[]) {
    init_curses();
    srand(time(NULL));

    // const char *originalText = "Hello, this is a sample line!";
    char* originalText = calloc(300, sizeof(char));

    for(int i = 0; i < 30; i++) {
        char* word = malloc(sizeof(char)*30);
        get_line(word, 30);
        strcat(originalText, word);
        free(word);
    }

    int length = strlen(originalText);
    character_state *state = calloc(length, sizeof(character_state));

    int cursorPosition = 0;
    time_t start_t, end_t;
    double diff;

    int started = 0;
    int misses = 0;
    while (1) {
        clear(); // Clear the screen

        // Print the line with cursor and highlight
        print_line(originalText, state, cursorPosition);

        refresh(); // Update the screen

        int userInput = getch(); // Get user input

        if (!started) {
            time(&start_t);
            started = !started;
        }

        if (userInput == KEY_LEFT && cursorPosition > 0) {
            cursorPosition--; // Move the cursor left
        } else if (userInput == KEY_RIGHT &&
                   cursorPosition < strlen(originalText)) {
            cursorPosition++;         // Move the cursor right
        } else if (userInput == 27) { // 27 is the ASCII code for the escape key
            break; // Exit the loop if the escape key is pressed
        } else if (userInput == KEY_BACKSPACE && cursorPosition >= 0) {
            cursorPosition--;
            state[cursorPosition] = UNTYPED;
        } else {
            if (cursorPosition < strlen(originalText) &&
                originalText[cursorPosition] != userInput) {
                state[cursorPosition] = INCORRECT;
                misses++;
            } else {
                state[cursorPosition] = CORRECT;
            }
            cursorPosition++;

            print_line(originalText, state, cursorPosition);
        }

        if (cursorPosition == length) {
            break;
        }
    }
    clear();

    time(&end_t);
    diff = difftime(end_t, start_t);

    int wrong = compute_accuracy(state, length);
    endcard(diff, length, wrong, misses);

    sleep(10);

    endwin(); // End NCurses
}
