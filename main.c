#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

#define CORNER 4
#define WIDTH 60

typedef enum {
    UNTYPED = 0,
    INCORRECT,
    CORRECT,
} character_state;

typedef enum {
    EASY,
    HARD,
    WU,
    TAYLOR,
    CHRIS,
} gamemode;

typedef struct {
    int mx;
    int my;
} window_size;

window_size get_window_size() {
    int mx = 0, my = 0;
    getmaxyx(stdscr, my, mx);
    window_size s = {mx, my};
    return s;
}

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

int readFileToString(const char *filename, char *buffer, size_t bufferSize) {
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

int main(int argc, char *argv[]) {
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

    srand(time(NULL));


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
