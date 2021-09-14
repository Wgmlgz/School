#include "canvas.h"

int main() {
  struct Canvas* canvas = makeCanvas(20, 20);

  putDot(canvas, 10, 10);
  putDot(canvas, 10, 5);
  putPlanet(canvas, 15, 15);

  for (int i = 0; i < 10000; ++i) {
    clrscr();
    printf("%d\n", i);
    printCanvas(canvas);
  }
}
