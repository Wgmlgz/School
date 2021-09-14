typedef struct {
  double x, y;
} Vec;

// Vec constructor
Vec* newVec(double x, double y) {
  Vec* vec = (Vec*)malloc(sizeof(Vec));
  vec->x = x;
  vec->y = y;
  return vec;
}

// -v
Vec vecInv(Vec v) {
  v.x = -v.x;
  v.y = -v.y;
  return v;
}

// a *= time
Vec vecScale(Vec v, double time) {
  v.x *= time;
  v.y *= time;
  return v;
}

// |v|
double vecAbs(Vec v) {
  return sqrt(v.x * v.x + v.y * v.y);
}

// normalize vector
Vec vecNorm(Vec v) {
  double len = vecAbs(v);
  v.x /= len;
  v.y /= len;
  return v;
}

// a + b
Vec vecAdd(Vec a, Vec b) {
  a.x += b.x;
  a.y += b.y;
  return a;
}

// a + b
void vecAddEq(Vec* a, Vec b) {
  a->x += b.x;
  a->y += b.y;
}

// a - b
Vec vecSub(Vec a, Vec b) {
  return vecAdd(a, vecInv(b));
}
