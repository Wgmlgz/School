#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CANVAS_W 10000
#define CANVAS_H 10000
#define clrscr() printf("\e[1;1H\e[2J")

struct Canvas {
  int w, h;
  char canvas[CANVAS_H][CANVAS_W];
};


struct Canvas* makeCanvas(int w, int h) {
  struct Canvas* canvas = malloc(sizeof(struct Canvas));
  canvas->w = w;
  canvas->h = h;
  memset(canvas->canvas, '.', CANVAS_H * CANVAS_W);
  return canvas;
}

void putDot(struct Canvas* canvas, int x, int y) {
  canvas->canvas[x][y] = '*';
}
void putPlanet(struct Canvas* canvas, int x, int y) {
  canvas->canvas[x][y] = '@';
}

void printCanvas(struct Canvas* canvas) {
  for (int i = 0; i < canvas->h; ++i) {
    for (int j = 0; j < canvas->w; ++j) {
      printf("%c", canvas->canvas[i][j]);
    }
    printf("\n");
  }
}
