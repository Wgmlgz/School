#pragma once
#include <complex>
#include <vector>
#include <iostream>
#include <math.h>
#include <cmath>
#include <functional>
#include "../../Lib/Wasm/WasmLib.hpp"


template<typename T>
class AlgebraicFractal : public RenderObject<T>{
 public:
  AlgebraicFractal(const std::function<T(std::complex<T>)>& equation) :
  equation_(equation) {}
  
  virtual void renderToCanvas(wasm::Canvas<T>& canvas) override {
    for (size_t px = 0; px < canvas.getW(); ++px) {
      for (size_t py = 0; py < canvas.getH(); ++py) {

        if (rand() % 5) continue;
        T x = px;
        T y = py;

        x -= canvas.getW() / 2;
        y -= canvas.getH() / 2;

        x /= canvas.getZoom();
        y /= canvas.getZoom();

        x += canvas.getX();
        y += canvas.getY();

        y = -y;

        auto res = equation_({x, y});

        auto& color_a = colors_[std::floor((colors_.size() - 1) * res)];
        auto& color_b = colors_[std::floor((colors_.size() - 1) * res) +1];

        int r = std::lerp(color_a[0], color_b[0], res);
        int g = std::lerp(color_a[1], color_b[1], res);
        int b = std::lerp(color_a[2], color_b[2], res);
        int a = std::lerp(color_a[3], color_b[3], res);

        canvas.setXYRGBA(px, py, r, g, b, a);
      }
    }
  }
 private:
  std::function<T(std::complex<T>)> equation_;
  std::vector<std::vector<uint8_t>> colors_{
    {  0,   7, 50, 255},
    { 32, 107, 203, 255},
    {237, 255, 255, 255},
    {255, 170,   0, 255},
    {  0,   0,   0, 255},
    {  0,   0,   0, 255},
  };
};
