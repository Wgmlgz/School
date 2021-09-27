#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>

// canvas settings
#define CANVAS_MAX 10000
#define CANVAS_DEFAULT_CHAR ' '

// canvas stuct
typedef struct {
  int w, h;
  HANDLE hConsole;
  DWORD dwBytesWritten;
  char canvas[CANVAS_MAX];
  char canvas_stable[CANVAS_MAX];
} Canvas;

// Canvas constructor
Canvas* newCanvas(int w, int h) {
  Canvas* canvas = (Canvas*)malloc(sizeof(Canvas));
  memset(canvas, CANVAS_DEFAULT_CHAR, sizeof(Canvas));

  canvas->w = w;
  canvas->h = h;

  // windows.h stuff idk
  canvas->hConsole = CreateConsoleScreenBuffer(
      GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
  SetConsoleActiveScreenBuffer(canvas->hConsole);
  canvas->dwBytesWritten = 0;

  return canvas;
}

// just clear canvas
void canvasReset(Canvas* canvas) {
  memcpy(canvas->canvas, canvas->canvas_stable, canvas->w * canvas->h);
}

// also clears tails
void canvasFullReset(Canvas* canvas) {
  memset(canvas->canvas_stable, CANVAS_DEFAULT_CHAR, canvas->w * canvas->h);
  memcpy(canvas->canvas, canvas->canvas_stable, canvas->w * canvas->h);
}

// safe char draw at [x][y] at canvas
void canvasPutChar(Canvas* canvas, int x, int y, char ch) {
  if (x < 0 || y < 0 || x >= canvas->h || y >= canvas->w) return;
  canvas->canvas[y * canvas->h + x] = ch;
}

// safe char draw at [x][y] at canvas
void canvasPutCharStable(Canvas* canvas, int x, int y, char ch) {
  if (x < 0 || y < 0 || x >= canvas->h || y >= canvas->w) return;
  canvas->canvas[y * canvas->h + x] = ch;
  canvas->canvas_stable[y * canvas->h + x] = ch;
}

// safe char draw at [x][y] at canvas
void canvasPutStr(Canvas* canvas, int x, int y, char* str) {
  while (*str) canvasPutChar(canvas, x++, y, *str++);
}

// print canvas to stdout
void printCanvas(Canvas* canvas) {
  // draw corners only
  canvas->canvas[0] = '#';
  canvas->canvas[canvas->h - 1] = '#';
  canvas->canvas[canvas->h * (canvas->w - 1)] = '#';
  canvas->canvas[canvas->h * canvas->w - 1] = '#';

  // windows.h stuff idk
  COORD here;
  here.X = 0;
  here.Y = 0;
  WriteConsoleOutputCharacter(canvas->hConsole, canvas->canvas,
                                canvas->w * canvas->h, here,
                                &canvas->dwBytesWritten);
}
