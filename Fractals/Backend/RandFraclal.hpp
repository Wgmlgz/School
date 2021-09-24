#pragma once
#include <complex>
#include <iostream>
#include <vector>
#include <random>
#include "../../Lib/Wasm/WasmLib.hpp"

template <typename T>
class RandFractal : public RenderObject<T> {
 public:
  void calc(wasm::Canvas<T>& canvas, size_t x0, size_t y0, size_t x1, size_t y1) {

    if (x1 - x0 < 2 and y1 - y0 < 2) return;
    int x = (x0 + x1) / 2;
    int y = (y0 + y1) / 2;

    int r = ((int)canvas.pixel(x0, y0)[0] + canvas.pixel(x1, y1)[0] + canvas.pixel(x0, y1)[0] + canvas.pixel(x1, y0)[0]) / 4;
    int g = ((int)canvas.pixel(x0, y0)[1] + canvas.pixel(x1, y1)[1] + canvas.pixel(x0, y1)[1] + canvas.pixel(x1, y0)[1]) / 4;
    int b = ((int)canvas.pixel(x0, y0)[2] + canvas.pixel(x1, y1)[2] + canvas.pixel(x0, y1)[2] + canvas.pixel(x1, y0)[2]) / 4;

    r += rng() % 40;
    g += rng() % 40;
    b += rng() % 40;

    canvas.setXYRGBA(x, y, r, g, b, 255);

    r = ((int)canvas.pixel(x0, y0)[0] + canvas.pixel(x0, y1)[0]) / 2;
    g = ((int)canvas.pixel(x0, y0)[1] + canvas.pixel(x0, y1)[1]) / 2;
    b = ((int)canvas.pixel(x0, y0)[2] + canvas.pixel(x0, y1)[2]) / 2;

    canvas.setXYRGBA(x, y0, r, g, b, 255);

    r = ((int)canvas.pixel(x1, y0)[0] + canvas.pixel(x1, y1)[0]) / 2;
    g = ((int)canvas.pixel(x1, y0)[1] + canvas.pixel(x1, y1)[1]) / 2;
    b = ((int)canvas.pixel(x1, y0)[2] + canvas.pixel(x1, y1)[2]) / 2;

    canvas.setXYRGBA(x1, y, r, g, b, 255);

    r = ((int)canvas.pixel(x0, y0)[0] + canvas.pixel(x0, y1)[0]) / 2;
    g = ((int)canvas.pixel(x0, y0)[1] + canvas.pixel(x0, y1)[1]) / 2;
    b = ((int)canvas.pixel(x0, y0)[2] + canvas.pixel(x0, y1)[2]) / 2;

    canvas.setXYRGBA(x0, y, r, g, b, 255);

    r = ((int)canvas.pixel(x0, y1)[0] + canvas.pixel(x1, y1)[0]) / 2;
    g = ((int)canvas.pixel(x0, y1)[1] + canvas.pixel(x1, y1)[1]) / 2;
    b = ((int)canvas.pixel(x0, y1)[2] + canvas.pixel(x1, y1)[2]) / 2;

    canvas.setXYRGBA(x, y1, r, g, b, 255);

    calc(canvas, x0, y0, x, y);
    calc(canvas, x, y0, x1, y);
    calc(canvas, x0, y, x, y1);
    calc(canvas, x, y, x1, y1);
  }

  virtual void renderToCanvas(wasm::Canvas<T>& canvas) override {
    rng.seed(666);
    canvas.setXYRGBA(0,                 0, rng(), rng(), rng(), 255);
    canvas.setXYRGBA(canvas.getW() - 1, 0, rng(), rng(), rng(), 255);
    canvas.setXYRGBA(0, canvas.getH() - 1, rng(), rng(), rng(), 255);
    canvas.setXYRGBA(canvas.getW() - 1, canvas.getH() - 1, rng(), rng(), rng(),
                     255);
    calc(canvas, 0, 0, canvas.getW()-1, canvas.getH()-1);
  }

 private:
  std::mt19937 rng;
};
