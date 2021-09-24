#include <complex>
#include "../../Lib/Wasm/WasmLib.hpp"
#include "RenderObject.hpp"
#include "GeometryFractal.hpp"
#include "AlgebraicFractal.hpp"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE 
#endif

wasm::Canvas<double> canvas(800, 1500);

std::vector<RenderObject<double>*> fractals{
  // star
  new GeometryFractal<double>({30, 0},{
  }, {
    {{0, 0}, {10, 0}},
    {{10, 0}, {15, 8.6603}},
    {{15, 8.6603}, {20, 0}},
    {{20, 0}, {30, 0}},
  }),
  // tree
  new GeometryFractal<double>({0, 20},{
    {{0, 0}, {0, 10}},
  }, {
    {{0, 10}, {8, 20}},
    {{0, 10}, {-8, 20}},
  }),
  // dragon
  new GeometryFractal<double>({10, 0},{
  }, {
    {{5, 5}, {0, 0}},
    {{5, 5}, {10, 0}},
  }),
  // mand
  new AlgebraicFractal<double>([](auto c) -> double {
    size_t max_iters = 100;
    std::complex<double> z;

    for (int i = 0; i < max_iters; ++i) {
      // z = {std::abs(z.real()), std::abs(z.imag())};
      z = z * z + c;
      if (std::abs(z) > 2) return max_iters / (double) i;
    }
    return 0;
    })
};

// #define FRACTAL star

int last_iters = 0;
int last_selected = 0;

extern "C" {
  EMSCRIPTEN_KEEPALIVE uint8_t* getTestData() {
    canvas.fill(0, 0, 0, 0);
    canvas.setX(wasm::ioGetDouble(1));
    canvas.setY(wasm::ioGetDouble(2));
    canvas.setZoom(wasm::ioGetDouble(3));

    if (last_iters != wasm::ioGetInt(4) or last_selected != wasm::ioGetInt(5)) {
      last_iters = wasm::ioGetInt(4);
      last_selected = wasm::ioGetInt(5);

      if (auto fractal = dynamic_cast<GeometryFractal<double>*>(fractals[ wasm::ioGetInt(5)]))
        fractal->recalc(wasm::ioGetInt(4));
    }
    fractals[ wasm::ioGetInt(5)]->renderToCanvas(canvas);
    return canvas.data();
  }
  EMSCRIPTEN_KEEPALIVE void fillCanvas(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    canvas.fill(r, g, b, a);
  }
  EMSCRIPTEN_KEEPALIVE void randFill() {
    canvas.randFill();
  }
}
