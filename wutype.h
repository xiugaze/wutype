/**
 * @file wutype.h
 * @author Caleb Andreano (andreanoc@msoe.edu)
 * @class CPE2600-121
 * @brief Utilities for wutype
 * 
 * Course: CPE2600-121
 * Assignment: Lab Wk 15
 * @date 2023-12-07
 */
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
