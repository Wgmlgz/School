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
        // std::cout << x << " " << y << std::endl;
        auto& color_a = colors_[std::floor(colors_.size() * res)];
        auto& color_b = colors_[std::floor(colors_.size() * res) +1];

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
      {0xef, 0x47, 0x6f, 0xff},
      {43, 45, 66, 255},
      {141, 153, 174, 255},
      {237, 242, 244, 255},
      {231, 84, 102, 255},
  };
};
