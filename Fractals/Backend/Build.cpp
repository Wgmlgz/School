#include <complex>
#include "../../Lib/Wasm/WasmLib.hpp"
#include "RenderObject.hpp"
#include "GeometryFractal.hpp"
#include "AlgebraicFractal.hpp"
#include "RandFraclal.hpp"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE 
#endif

wasm::Canvas<double> canvas(800, 1400);

std::vector<RenderObject<double>*> fractals{
    // star
    new GeometryFractal<double>({30, 0}, {},
                                {
                                    {{0, 0}, {10, 0}},
                                    {{10, 0}, {15, 8.6603}},
                                    {{15, 8.6603}, {20, 0}},
                                    {{20, 0}, {30, 0}},
                                }),
    // tree
    new GeometryFractal<double>({0, 20},
                                {
                                    {{0, 0}, {0, 10}},
                                },
                                {
                                    {{0, 10}, {8, 20}},
                                    {{0, 10}, {-8, 20}},
                                }),
    // dragon
    new GeometryFractal<double>({10, 0}, {},
                                {
                                    {{5, 5}, {0, 0}},
                                    {{5, 5}, {10, 0}},
                                }),
    // mand
    new AlgebraicFractal<double>([](auto c) -> double {
      size_t iteration = 0, max_iters = 500;
      std::complex<double> z;

      double x2 = 0, y2 = 0;
      double x = z.real(), y = z.imag();
      double x0 = c.real(), y0 = c.imag();
      double xold = 0, yold = 0, period = 0;

      while (x2 + y2 <= 4 and iteration < max_iters) {
        y = (x + x) * y + y0;
        x = x2 - y2 + x0;
        x2 = x * x;
        y2 = y * y;
        iteration = iteration + 1;

        if (std::abs(x - xold) < 1e-2 and std::abs(y - yold) < 1e-2) {
          iteration = max_iters;
          break;
        }
        period = period + 1;
        if (period > 20) {
          period = 0;
          xold = x;
          yold = y;
        }
      }

      return (double)iteration / max_iters;
      return 0;
    }),
    new AlgebraicFractal<double>([](auto c) -> double {
      size_t iteration = 0, max_iters = 500;
      std::complex<double> z;

      double x2 = 0, y2 = 0;
      double x = z.real(), y = z.imag();
      double x0 = c.real(), y0 = c.imag();
      double xold = 0, yold = 0, period = 0;

      while (x2 + y2 <= 4 and iteration < max_iters) {
        x = std::abs(x);
        y = std::abs(y);
        
        y = (x + x) * y + y0;
        x = x2 - y2 + x0;
        x2 = x * x;
        y2 = y * y;
        

        
        iteration = iteration + 1;

        if (std::abs(x - xold) < 1e-2 and std::abs(y - yold) < 1e-2) {
          iteration = max_iters;
          break;
        }
        period = period + 1;
        if (period > 20) {
          period = 0;
          xold = x;
          yold = y;
        }
      }

      return (double)iteration / max_iters;
      return 0;
    }),
    new RandFractal<double>()
};

// #define FRACTAL star

int last_iters = 0;
int last_selected = 0;

extern "C" {
  EMSCRIPTEN_KEEPALIVE uint8_t* getTestData() {
    if (auto fractal = dynamic_cast<GeometryFractal<double>*>(fractals[wasm::ioGetInt(5)]))
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
