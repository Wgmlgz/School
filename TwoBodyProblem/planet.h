#pragma once
#include "vec.h"
#include <math.h>

const double G = 6.67;

// material point actually
typedef struct {
  Vec pos;
  Vec vel;
  double m;
} Planet;

// Planet constructor
Planet* newPlanet() {
  Planet* planet = (Planet*)malloc(sizeof(Planet));
  memset(planet, 0, sizeof(Planet));
  return planet;
}

// Apply planet velocity
void planetApplyVel(Planet* a, double time) {
  Vec tmp = a->vel;
  tmp = vecScale(tmp, time);
  tmp = vecAdd(a->pos, tmp);

  a->pos = tmp;
}

// Apply gravity between 2 planets
void planetApplyG(Planet* p1, Planet* p2, double time) {
  double r = vecAbs(vecSub(p1->pos, p2->pos));
  double F = (G * p1->m * p2->m ) / fmax(r * r, 0.001);

  double a1 = F / p1->m;
  double a2 = F / p2->m;

  vecAddEq(&p1->vel, vecScale(vecNorm(vecSub(p2->pos, p1->pos)), a1 * time));
  vecAddEq(&p2->vel, vecScale(vecNorm(vecSub(p1->pos, p2->pos)), a2 * time));
}

