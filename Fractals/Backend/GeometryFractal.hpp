#pragma once
#include <complex>
#include <vector>
#include <iostream>
#include "../../Lib/Wasm/WasmLib.hpp"


template<typename T>
class GeometryFractal : public RenderObject<T>{
 public:
  GeometryFractal(std::complex<T> base, std::vector<wasm::Line<T>> const_lines,
                  std::vector<wasm::Line<T>> change_lines) :
  base_(base), const_lines_(const_lines), change_lines_(change_lines),
  setup_const_lines_(const_lines), setup_change_lines_(change_lines) {}
  
  virtual void renderToCanvas(wasm::Canvas<T>& canvas) override {
    for (auto & i : const_lines_)
      canvas.drawLine(i);
    for (auto & i : change_lines_)
      canvas.drawLine(i);
  }

  void apply(wasm::Line<T>& line, wasm::Line<T>& m) {
    auto t = line.b - line.a;
    auto tm = m.b - m.a;
    auto r = t * tm;
  }

  void iterate() {
    std::vector<wasm::Line<T>> new_change_lines_; 
    while (change_lines_.size()) {
      auto cur_line = change_lines_.back();
      change_lines_.pop_back();

      auto scale = std::abs(cur_line.b - cur_line.a) / std::abs(base_);
      auto rot = std::arg(cur_line.b - cur_line.a) - std::arg(base_);

      for (auto i : setup_change_lines_) {
        auto t = std::polar(scale, rot);
        i.a *= t;
        i.b *= t;
        i.a += cur_line.a;
        i.b += cur_line.a;
        new_change_lines_.push_back(i);
      }
      for (auto i : setup_const_lines_) {
        auto t = std::polar(scale, rot);
        i.a *= t;
        i.b *= t;
        i.a += cur_line.a;
        i.b += cur_line.a;
        const_lines_.push_back(i);
      }
    }
    std::swap(new_change_lines_, change_lines_);
    new_change_lines_.clear();
  }

  void recalc(int iters) {
    const_lines_ = setup_const_lines_;
    change_lines_ = setup_change_lines_;
    for (int i = 0; i < iters; ++i)
      iterate();
  }

 private:
  std::complex<T> base_;
  std::vector<wasm::Line<T>> const_lines_, change_lines_;
  std::vector<wasm::Line<T>> setup_const_lines_, setup_change_lines_;
};
