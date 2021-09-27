#pragma once
#include <time.h>
#include <math.h>
#include <string.h>
// #include "canvas.h"
#include "planet.h"

// State management
double last_time = 0;
int pos = 0;
int paused = 0;

// Get time from start
double curTime() { return clock() * 1.0 / CLOCKS_PER_SEC; }

// Reset last time stamp
void resetTime() { last_time = curTime(); }

// main sim
void simulate(Planet* planets[], int n) {
  double time = curTime() - last_time;

  for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
    planetApplyG(planets[i], planets[j], time);
  for (int i = 0; i < n; ++i)
    planetApplyVel(planets[i], time);
    
  resetTime();
}

// Old ascii version (((

// Draws planet on canvas
// void canvasPutPlanet(Canvas* canvas, Planet* a, char* name) {
//   int x = round(a->pos.x), y = round(a->pos.y / 2.0);
//   int vx = round(a->pos.x + a->vel.x), vy = round((a->pos.y +  a->vel.y) / 2.0);

//   if ((x % 2 != (y % 2))) canvasPutCharStable(canvas, x, y, '.');
//   canvasPutChar(canvas, x, y, '+');
//   canvasPutChar(canvas, vx, vy, '*');
//   x -= strlen(name) / 2;
//   canvasPutStr(canvas, x, y - 1, name);
// }



// void input(Canvas* canvas, Planet* a, Planet* b) {
//   double time = curTime() - last_time;
//   double diff = 0;
//   if (GetAsyncKeyState((unsigned short)'C') & WM_APP) canvasFullReset(canvas);
//   if (GetAsyncKeyState((unsigned short)'W') & WM_KEYUP) --pos;
//   if (GetAsyncKeyState((unsigned short)'S') & WM_KEYUP) ++pos;
//   if (GetAsyncKeyState((unsigned short)'A') & WM_APP) diff = -10;
//   if (GetAsyncKeyState((unsigned short)'D') & WM_APP) diff = 10;
//   if (GetAsyncKeyState((unsigned short)' ') & WM_KEYUP) paused ^= 1;

//   pos += 10;
//   pos %= 10;

//   double* to_change;

//   if (pos == 0) to_change = &a->pos.x;
//   if (pos == 1) to_change = &a->pos.y;
//   if (pos == 2) to_change = &a->vel.x;
//   if (pos == 3) to_change = &a->vel.y;
//   if (pos == 4) to_change = &a->m, diff *= 100;

//   if (pos == 5) to_change = &b->pos.x;
//   if (pos == 6) to_change = &b->pos.y;
//   if (pos == 7) to_change = &b->vel.x;
//   if (pos == 8) to_change = &b->vel.y;
//   if (pos == 9) to_change = &b->m, diff *= 100;

//   *to_change = *to_change + diff * time;
// }

// void print(Canvas* canvas, Planet* a, Planet* b) {
//   canvasReset(canvas);

//   canvasPutPlanet(canvas, a, "Amogus");
//   canvasPutPlanet(canvas, b, "Bruh");

//   char str[32];
//   snprintf(str, 32, "Frame time: %.20lf", time);
//   canvasPutStr(canvas, 3, 2, str);

//   snprintf(str, 32, "    Planet Amogus:");
//   canvasPutStr(canvas, 3, 4, str);
//   snprintf(str, 32, "  1 pos x: %lf", a->pos.x);
//   canvasPutStr(canvas, 3, 5, str);
//   snprintf(str, 32, "  2     y: %lf", a->pos.y);
//   canvasPutStr(canvas, 3, 6, str);
//   snprintf(str, 32, "  3 vel x: %lf", a->vel.x);
//   canvasPutStr(canvas, 3, 7, str);
//   snprintf(str, 32, "  4     y: %lf", a->vel.y);
//   canvasPutStr(canvas, 3, 8, str);
//   snprintf(str, 32, "  5  mass: %lf", a->m);
//   canvasPutStr(canvas, 3, 9, str);

//   snprintf(str, 32, "    Planet Bruh:");
//   canvasPutStr(canvas, 3, 11, str);
//   snprintf(str, 32, "  6 pos x: %lf", b->pos.x);
//   canvasPutStr(canvas, 3, 12, str);
//   snprintf(str, 32, "  7     y: %lf", b->pos.y);
//   canvasPutStr(canvas, 3, 13, str);
//   snprintf(str, 32, "  8 vel x: %lf", b->vel.x);
//   canvasPutStr(canvas, 3, 14, str);
//   snprintf(str, 32, "  9     y: %lf", b->vel.y);
//   canvasPutStr(canvas, 3, 15, str);
//   snprintf(str, 32, "  10 mass: %lf", b->m);
//   canvasPutStr(canvas, 3, 16, str);

//   snprintf(str, 32, "C - clear tails");
//   canvasPutStr(canvas, 3, canvas->w - 5, str);
//   snprintf(str, 32, "Space - play/pause");
//   canvasPutStr(canvas, 3, canvas->w - 4, str);
//   snprintf(str, 32, "W/S - up/down");
//   canvasPutStr(canvas, 3, canvas->w - 3, str);
//   snprintf(str, 32, "A/D - in/decrease");
//   canvasPutStr(canvas, 3, canvas->w - 2, str);


//   canvasPutChar(canvas, 3, pos + 5 + (pos / 5 * 2), '>');
//   printCanvas(canvas);
// }

// void update(Canvas* canvas, Planet* a, Planet* b) {
//   input(canvas, a, b);

//   if (!paused) simulate(a, b);
//   else resetTime();
  
//   print(canvas, a, b);
// }
