#include <ncurses.h>
#include <math.h>
#include <stdlib.h>

typedef struct {
    double x;
    double y;
    double z;
} point;

void line(int startXLocation, int startYLocation, int endXLocation, int endYLocation) 
{
  int dx, sx, dy, sy, gradientError, gradientErrorTwo;
  dx = abs(endXLocation - startXLocation);/* Absolute integer value of the closest approximation of dx. */
  sx = startXLocation < endXLocation ? 1 : -1;/* 1 if positive gradient, -1 if not. */
  dy = abs(endYLocation - startYLocation);/* Absolute integer value of the closest approximation of dy. */
  sy = startYLocation < endYLocation ? 1 : -1;/* 1 if positive gradient, -1 if not. */
  gradientError = (dx > dy ? dx : -dy) / 2;
  while(1) {
    mvprintw(startXLocation, startYLocation, ".");/* put a character in the current co-ordinate. */
    if (startXLocation == endXLocation && startYLocation == endYLocation)/* If there's no line to be drawn, just draw a dot. */
      break;
    /* Calculate the position of the next co-ordinate on the line using Bresenham's Line Drawing Algorithm. */
    gradientErrorTwo = gradientError;
    if (gradientErrorTwo > -dx)
    {
      gradientError -= dy;
      startXLocation += sx;
    }
    if (gradientErrorTwo < dy)
    {
      gradientError += dx;
      startYLocation += sy;
    }
  }
}

void square(int x, int y, int length) {
    line(x, y, x+length, y);
	line(x, y, x, y+length);
	line(x, y+length, x+length, y+length);
	line(x+length, y+length, x+length, y);
}

void rotate(point *p, double x, double y, double z) {
	double rad = 0;
    rad = x;
    p->y = cos(rad) * p->y - sin(rad) * p->z;
    p->z = sin(rad) * p->y + cos(rad) * p->z;

    rad = y;
    p->x = cos(rad) * p->x + sin(rad) * p->z;
    p->z = sin(rad) * p->x + cos(rad) * p->z;

    rad = z;
    p->x = cos(rad) * p->x - sin(rad) * p->y;
    p->y = sin(rad) * p->x + cos(rad) * p->y;
}

int main() {
    initscr(); // curses mode
    square(10, 10, 20);
    refresh();
    getch();
    endwin();
    return 0;
}
