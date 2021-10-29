#include <complex>
#include "../../Lib/Wasm/WasmLib.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE 
#endif

wasm::Canvas<double> canvas(600, 1800);

int last_iters = 0;
int last_selected = 0;

extern "C" {
  EMSCRIPTEN_KEEPALIVE uint8_t* getTestData() {
    // if (auto fractal = dynamic_cast<GeometryFractal<double>*>(fractals[wasm::ioGetInt(5)]))
      canvas.fill(255, 0, 100, 255);

    canvas.setX(wasm::ioGetDouble(1));
    canvas.setY(wasm::ioGetDouble(2));
    canvas.setZoom(wasm::ioGetDouble(3));

    // if (last_iters != wasm::ioGetInt(4) or last_selected != wasm::ioGetInt(5)) {
    //   last_iters = wasm::ioGetInt(4);
    //   last_selected = wasm::ioGetInt(5);
    //   canvas.fill(0, 0, 0, 255);

    //   if (auto fractal = dynamic_cast<GeometryFractal<double>*>(fractals[ wasm::ioGetInt(5)]))
    //     fractal->recalc(wasm::ioGetInt(4));
    // }
    // fractals[ wasm::ioGetInt(5)]->renderToCanvas(canvas);
    return canvas.data();
  }
  EMSCRIPTEN_KEEPALIVE void fillCanvas(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    canvas.fill(r, g, b, a);
  }
  EMSCRIPTEN_KEEPALIVE void randFill() {
    canvas.randFill();
  }
}
