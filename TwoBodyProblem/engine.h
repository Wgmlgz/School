#include "planet.h"
#include "canvas.h"

void putStuff(Canvas* canvas, Planet* a) {
  putPlanet(canvas, a->pos.x, a->pos.y);
  // putPlanet(canvas, b->pos.x, b->pos.y);
}

void sim(Planet* a, double time) {
  vec new_pos = a->pos;
  
}
