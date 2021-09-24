#pragma once
#include <stdlib.h>

#include <algorithm>
#include <complex>
#include <cstdint>
#include <functional>
#include <memory>
#include <random>
#include <utility>
#include <vector>

namespace wasm {
template <typename T>
struct Line {
  std::complex<T> a, b;
};

template <typename T>
class Canvas {
 public:
  Canvas(size_t h, size_t w) : h_(h),  w_(w) {
    data_ = reinterpret_cast<uint8_t*>(malloc(w * h * 4));
    fill(68, 71, 90, 54);
    // fill(138, 43, 226, 255);
  }

  void invert() {
    for (int i = 0; i < w_ * h_ * 4; i += 4) {
      data_[i] = 255 - data_[i];
      data_[i + 1] = 255 - data_[i + 1];
      data_[i + 2] = 255 - data_[i + 2];
    }
  }

  void drawLine(const Line<T>& line) {
    drawLine(line.a.real(), line.a.imag(), line.b.real(), line.b.imag());
  }

  void fill(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    for (int i = 0; i < w_; ++i) {
      for (int j = 0; j < h_; ++j) {
        pixel(i, j)[0] = r;
        pixel(i, j)[1] = g;
        pixel(i, j)[2] = b;
        pixel(i, j)[3] = a;
      }
    }
  }

  void randFill() {
    for (int i = 0; i < w_; ++i) {
      for (int j = 0; j < h_; ++j) {
        pixel(i, j)[0] = rng();
        pixel(i, j)[1] = rng();
        pixel(i, j)[2] = rng();
        pixel(i, j)[3] = 255;
      }
    }
  }

  uint8_t* pixel(size_t x, size_t y) {
    return data_ + (h_ - y - 1) * w_ * 4 + x * 4;
  }

  void setXYRGBA(size_t x, size_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    pixel(x, y)[0] = r;
    pixel(x, y)[1] = g;
    pixel(x, y)[2] = b;
    pixel(x, y)[3] = a;
  }

  uint8_t* data() { return data_; }

  
  T getX() { return x_; }
  T getY() { return y_; }
  size_t getW() { return w_; }
  size_t getH() { return h_; }
  T getZoom() { return zoom_; }
  void setX(T val) {x_ = val;}
  void setY(T val) {y_ = val;}
  void setZoom(T val) {zoom_ = val;}
  
  ~Canvas() { free(data_); }

 private:
  void addPixel(size_t x, size_t y, uint8_t r, uint8_t g, uint8_t b,
                uint8_t a) {
    if (x >= w_) return;
    if (y >= h_) return;
    pixel(x, y)[0] = std::max(pixel(x, y)[0], r);
    pixel(x, y)[1] = std::max(pixel(x, y)[1], g);
    pixel(x, y)[2] = std::max(pixel(x, y)[2], b);
    pixel(x, y)[3] = std::max(pixel(x, y)[3], a);
  }

  void setPixel(size_t x, size_t y, float brightness, uint8_t r = 238,
                uint8_t g = 130, uint8_t b = 238) {
    addPixel(x, y, r * brightness, g * brightness, b * brightness, 255);
  }

  void drawLine(T x0, T y0, T x1, T y1, T size = 0.5, size_t iterations = 3) {
    x0 -= x_;
    x1 -= x_;
    y0 -= y_;
    y1 -= y_;

    x0 *= zoom_;
    x1 *= zoom_;
    y0 *= zoom_;
    y1 *= zoom_;

    x0 += w_ / 2;
    x1 += w_ / 2;
    y0 += h_ / 2;
    y1 += h_ / 2;

    for (float i = -size; i < size; i += 2 * size / iterations) {
      for (float j = -size; j < size; j += 2 * size / iterations) {
        putLine(x0 + i, y0 + j, x1 + i, y1 + j);
        putLine(x0, y0, x1, y1);
      }
    }
  }

  // https://rosettacode.org/wiki/Xiaolin_Wu%27s_line_algorithm#C.2B.2B
  void putLine(T x0, T y0, T x1, T y1) {
    auto ipart = [](float x) -> int { return int(std::floor(x)); };
    auto round = [](float x) -> float { return std::round(x); };
    auto fpart = [](float x) -> float { return x - std::floor(x); };
    auto rfpart = [=](float x) -> float { return 1 - fpart(x); };

    const bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
      std::swap(x0, y0);
      std::swap(x1, y1);
    }
    if (x0 > x1) {
      std::swap(x0, x1);
      std::swap(y0, y1);
    }

    const float dx = x1 - x0;
    const float dy = y1 - y0;
    const float gradient = (dx == 0) ? 1 : dy / dx;

    int xpx11;
    float intery;
    {
      const float xend = round(x0);
      const float yend = y0 + gradient * (xend - x0);
      const float xgap = rfpart(x0 + 0.5);
      xpx11 = int(xend);
      const int ypx11 = ipart(yend);
      if (steep) {
        setPixel(ypx11, xpx11, rfpart(yend) * xgap);
        setPixel(ypx11 + 1, xpx11, fpart(yend) * xgap);
      } else {
        setPixel(xpx11, ypx11, rfpart(yend) * xgap);
        setPixel(xpx11, ypx11 + 1, fpart(yend) * xgap);
      }
      intery = yend + gradient;
    }

    int xpx12;
    {
      const float xend = round(x1);
      const float yend = y1 + gradient * (xend - x1);
      const float xgap = rfpart(x1 + 0.5);
      xpx12 = int(xend);
      const int ypx12 = ipart(yend);
      if (steep) {
        setPixel(ypx12, xpx12, rfpart(yend) * xgap);
        setPixel(ypx12 + 1, xpx12, fpart(yend) * xgap);
      } else {
        setPixel(xpx12, ypx12, rfpart(yend) * xgap);
        setPixel(xpx12, ypx12 + 1, fpart(yend) * xgap);
      }
    }

    if (steep) {
      for (int x = xpx11 + 1; x < xpx12; x++) {
        setPixel(ipart(intery), x, rfpart(intery));
        setPixel(ipart(intery) + 1, x, fpart(intery));
        intery += gradient;
      }
    } else {
      for (int x = xpx11 + 1; x < xpx12; x++) {
        setPixel(x, ipart(intery), rfpart(intery));
        setPixel(x, ipart(intery) + 1, fpart(intery));
        intery += gradient;
      }
    }
  }

  size_t w_, h_;
  T x_ = -18, y_ = -10, zoom_ = 10;
  uint8_t* data_;
  std::mt19937 rng;
};

}  // namespace wasm
