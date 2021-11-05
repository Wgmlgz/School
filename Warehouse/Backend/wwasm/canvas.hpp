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
#include <map>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <cmath>
#include <memory>
#include <stdlib.h>
#include <string.h>
#include <queue>
#include "em_header.hpp"
#include "io.hpp"
#include "animations.hpp"

using namespace std::string_literals;
namespace wwasm {

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

namespace Dracula {
  auto red = 0xff5555ff;
  auto black = 0x282a36ff;
  auto gray = 0x44475aff;
  auto green = 0x50fa7bff;
  auto purple = 0xbd93f9ff;
  auto pink = 0xff79c6ff;
}

struct Col {
  uint8_t r = 0, g = 0, b = 0, a = 255;
  Col(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ = 255) : r(r_), g(g_), b(b_), a(a_) {};
  Col(int hex) {
    r = ((hex >> 24) & 0xff);
    g = ((hex >> 16) & 0xff);
    b = ((hex >> 8) & 0xff);
    a = ((hex)&0xff);
  }

  /**
   * rgba
   * abgr 
   */
  static u32 make(u32 col) {
    return (col & 0xff000000) >> 24 |
           (col & 0x00ff0000) >> 8  |
           (col & 0x0000ff00) << 8  |
           (col & 0x000000ff) << 24;
  }

  template <typename I>
  static std::string n2hexstr(I w, size_t hex_len = sizeof(I) << 1) {
    static const char* digits = "0123456789ABCDEF";
    std::string rc(hex_len, '0');
    for (size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4)
      rc[i] = digits[(w >> j) & 0x0f];
    return rc;
  }

  std::string toHexStr() {
    return "#" + n2hexstr(r) + n2hexstr(g) + n2hexstr(b) + n2hexstr(a);
  }
};

struct Canvas {
  typedef float CT;
  typedef std::complex<float> Pt;
  typedef Canvas& CTR;

  struct Entity {
    virtual void applyFrm(const frm& f) = 0;
    virtual void render(CTR canvas) = 0;
    virtual std::string renderJSON(CTR canvas) = 0;
    virtual ~Entity() {};
  };

  Canvas(std::string id, size_t w, size_t h) {
    id_ = id;
    data_ = new u8[w * h * 10 + 1024];
    data_ += 1024;
    reset(w, h);
  }
  Canvas() {}

  void reset(int w, int h, bool do_fill = false) {
    h_ = h;
    w_ = w;
    if (do_fill) fill(Dracula::gray);
  }

  Pt transform(Pt p) {
    p.real(p.real() - x_);
    p.imag(p.imag() - y_);

    p.real(p.real() * zoom_);
    p.imag(p.imag() * zoom_);

    p.real(p.real() + w_ / 2);
    p.imag(p.imag() + h_ / 2);

    return p;
  }

  Pt transformBack(Pt p) {
    p.real(p.real() - w_ / 2);
    p.imag(p.imag() - h_ / 2);


    p.real(p.real() / zoom_);
    p.imag(p.imag() / zoom_);

    p.real(p.real() + x_);
    p.imag(p.imag() + y_);

    return p;
  }

  void invert() {
    for (int i = 0; i < w_ * h_ * 4; i += 4) {
      data_[i] = 255 - data_[i];
      data_[i + 1] = 255 - data_[i + 1];
      data_[i + 2] = 255 - data_[i + 2];
    }
  }

  void fill(uint32_t col) {
    col = Col::make(col);
    auto ptr = data_;
    for (int i = 0; i < w_; ++i) {
      for (int j = 0; j < h_; ++j) {
        *(uint32_t*)ptr = col;
        ptr += 4;
      }
    }
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
    if (x >= w_) return;
    if (y >= h_) return;
    pixel(x, y)[0] = r;
    pixel(x, y)[1] = g;
    pixel(x, y)[2] = b;
    pixel(x, y)[3] = a;
  }

  void pushEntity(int z, std::string id, Entity* entity) {
    entities_[z] = std::shared_ptr<Entity>(entity);
    entities_str_[id] = z;
  }

  void popEntity(const std::string& id) {
    remove_queue.push(id);
  }

  void removeEntities() {
    while(remove_queue.size()) {
      auto id = remove_queue.front();
      remove_queue.pop();
      if (!entities_str_.contains(id)) continue;
      
      entities_.erase(entities_str_[id]);
      entities_str_.erase(id);
      animations_.erase(id);
    }
  }

  void pushEntity(std::string id, Entity* entity) {
    pushEntity(entities_.size() ? entities_.rbegin()->first + 1 : 0, id, entity);
  }

  void playAnim(std::string id, anim animation) {
    animations_.erase(id);
    animations_.insert({id, animation});
  }

  uint8_t* data() { return data_; }

  void updateIO() {
    auto zoom = ioGetDouble(id_ + "_zoom");
    if (zoom == 0) zoom = 1;

    setZoom(zoom);
    setX(ioGetDouble(id_ + "_x"));
    setY(ioGetDouble(id_ + "_y"));
  }

  void clear() {
    entities_.clear();
    animations_.clear();
    entities_str_.clear();
    while(remove_queue.size()) remove_queue.pop();
  }

  uint8_t* render() {
    updateIO();

    for (auto [id, z] : entities_str_) {
      auto& entity = entities_[z]; 
      if (animations_.count(id)) {
        entity->applyFrm(animations_.at(id).get());
      }
      entity->render(*this);
    }

    removeEntities();
    return data();
  }


  std::string renderJSON() {
    updateIO();

    std::string res = R"({"data": [)";
    for (auto [id, z] : entities_str_) {
      auto& entity = entities_[z];
      if (animations_.count(id)) {
        entity->applyFrm(animations_.at(id).get());
      }
      if (res != R"({"data": [)") res += ",";
      res += entity->renderJSON(*this);
    }
    res += "]}";

    removeEntities();
    return res;
  }

  float getX() { return x_; }
  float getY() { return y_; }
  size_t getW() { return w_; }
  size_t getH() { return h_; }
  float getZoom() { return zoom_; }

  float lerp(float a, float b, float f) { return (a * (1.0 - f)) + (b * f); }
  void setX(float val) { x_ = val; lerp(x_, val, 0.1);}
  void setY(float val) { y_ = val; lerp(y_, val, 0.1);}
  void setZoom(float val) { zoom_ = val; lerp(zoom_, val, 0.1);}
  
  ~Canvas() { if(data_) delete data_; }

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

  std::string id_;
  size_t w_ = 500, h_ = 500;
  float x_ = 0, y_ = 0, zoom_ = 10;
  uint8_t* data_ = nullptr;
  std::mt19937 rng;
  std::map<int, std::shared_ptr<Entity>> entities_;
  std::map<std::string, anim> animations_;
  std::map<std::string, int> entities_str_;
  std::queue<std::string> remove_queue;

  static std::map<std::string, Canvas> canvases;

  static Canvas& getCanvas(std::string name) {
    return canvases.at(name);
  }

  static Canvas& regiesterCanvas(std::string name) {
    canvases.emplace(name, Canvas(name, 500, 500));
    return getCanvas(name);
  }
};
std::map<std::string, Canvas> Canvas::canvases = {{"default", Canvas("default", 10, 10)}};

extern "C" {
EMSCRIPTEN_KEEPALIVE uint8_t* getCanvasData(char* ptr, int w, int h) {
  std::string id(ptr);
  auto& canvas = Canvas::getCanvas(id);
  canvas.reset(w, h, true);
  auto res = canvas.render();
  return res;
}

EMSCRIPTEN_KEEPALIVE const char* getCanvasJSON(char* ptr, int w, int h) {
  std::string id(ptr);
  auto& canvas = Canvas::getCanvas(id);
  canvas.reset(w, h, false);
  auto str = canvas.renderJSON();
  
  static std::string buff;
  buff = str;
  return buff.c_str();
}
}
}  // namespace wwasm
