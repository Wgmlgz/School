typedef struct vec {
  double x, y;
};

void vecAdd(struct vec* a, struct vec* b) {
  a->x += b->x;
  a->y += b->y;
}

void mult(struct vec* v, double time) {
  v->x *= time;
  v->y *= time;
} 