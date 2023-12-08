#ifndef WUTYPE
#define WUTYPE

#include "types.h"
#include "utils.h"

void print_char(const char *text, character_state *state, int cursor_position, int position);
void print_frame(const char *text, character_state *state, int cursor_position,
                 int cursor_start, int cursor_end);
int readFileToString(const char *filename, char *buffer, int bufferSize);
int get_line(char *filename, char *buffer, int size, int dict_size);
window_size init_curses();

#endif // !WUTYPE
