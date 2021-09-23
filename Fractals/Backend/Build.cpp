#include "../../Lib/Wasm/WasmLib.hpp"
#include "GeometryFractal.hpp"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE 
#endif

wasm::Canvas<double> canvas(800, 1500);

GeometryFractal<double> star({30, 0},{
}, {
  {{0, 0}, {10, 0}},
  {{10, 0}, {15, 8.6603}},
  {{15, 8.6603}, {20, 0}},
  {{20, 0}, {30, 0}},
});

GeometryFractal<double> tree({0, 20},{
  {{0, 0}, {0, 10}},
}, {
  {{0, 10}, {8, 20}},
  {{0, 10}, {-8, 20}},
});

GeometryFractal<double> dragon({10, 0},{
}, {
  {{5, 5}, {0, 0}},
  {{5, 5}, {10, 0}},
});

#define FRACTAL star

int last_iters = 0;
extern "C" {
  EMSCRIPTEN_KEEPALIVE uint8_t* getTestData() {
    canvas.fill(0, 0, 0, 0);
    canvas.setX(wasm::ioGetDouble(1));
    canvas.setY(wasm::ioGetDouble(2));
    canvas.setZoom(wasm::ioGetDouble(3));
    if (last_iters != wasm::ioGetInt(4)) {
      last_iters = wasm::ioGetInt(4);
      FRACTAL.recalc(wasm::ioGetInt(4));
    }
    FRACTAL.printToCanvas(canvas);
    return canvas.data();
  }
  EMSCRIPTEN_KEEPALIVE void fillCanvas(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    canvas.fill(r, g, b, a);
  }
  EMSCRIPTEN_KEEPALIVE void randFill() {
    canvas.randFill();
  }
}
