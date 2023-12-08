/**
 * @file utils.h
 * @author Caleb Andreano (andreanoc@msoe.edu)
 * @class CPE2600-121
 * @brief Utilities for wutype
 * 
 * Course: CPE2600-121
 * Assignment: Lab Wk 15
 * @date 2023-12-07
 */
#ifndef UTILS
#define UTILS

#include "types.h"

void print_box(int y, int x, int height, int width);
void draw_logo(int y, int x);
window_size get_window_size();
void startcard(int my, int mx, gamemode mode);
void endcard(float time, int length, int incorrect, int misses, int mx, int my);
int compute_accuracy(character_state *state, int length);
void print_help();

#endif // !UTILS
