#pragma once
#include "../../Lib/Wasm/WasmLib.hpp"

template<typename T>
struct RenderObject {
  virtual void renderToCanvas(wasm::Canvas<T>& canvas) {}
};