#include <iostream>
#include "../../Lib/Wasm/WasmLib.hpp"
#include "engine.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE 
#endif

wasm::Canvas<double> canvas(600, 1800);
Planet *a, *b;

extern "C" {
  EMSCRIPTEN_KEEPALIVE uint8_t* getTestData() {
    if (paused) {
      a->pos.x = wasm::ioGetDouble(10);
      a->pos.y = wasm::ioGetDouble(11);
      b->pos.x = wasm::ioGetDouble(20);
      b->pos.y = wasm::ioGetDouble(21);

      a->vel.x = wasm::ioGetDouble(12);
      a->vel.y = wasm::ioGetDouble(13);
      b->vel.x = wasm::ioGetDouble(22);
      b->vel.y = wasm::ioGetDouble(23);

      a->m = wasm::ioGetDouble(15);
      b->m = wasm::ioGetDouble(25);
    }

    wasm::ioSetDouble(10, a->pos.x);
    wasm::ioSetDouble(11, a->pos.y);
    wasm::ioSetDouble(20, b->pos.x);
    wasm::ioSetDouble(21, b->pos.y);

    wasm::ioSetDouble(12, a->vel.x);
    wasm::ioSetDouble(13, a->vel.y);
    wasm::ioSetDouble(22, b->vel.x);
    wasm::ioSetDouble(23, b->vel.y);

    wasm::ioSetDouble(15, a->m);
    wasm::ioSetDouble(25, b->m);

    canvas.setX(wasm::ioGetDouble(1));
    canvas.setY(wasm::ioGetDouble(2));
    canvas.setZoom(wasm::ioGetDouble(3));

    canvas.fill(0, 0, 0, 0);
    canvas.drawRect(a->pos.x, a->pos.y, 1);
    canvas.drawRect(b->pos.x, b->pos.y, 1);

    canvas.drawLine(
        {{a->pos.x, a->pos.y}, {a->pos.x + a->vel.x, a->pos.y + a->vel.y}});
    canvas.drawLine(
        {{b->pos.x, b->pos.y}, {b->pos.x + b->vel.x, b->pos.y + b->vel.y}});

    // if (auto fractal = dynamic_cast<GeometryFractal<double>*>(fractals[wasm::ioGetInt(5)]))
    //   canvas.fill(0, 0, 0, 255);



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
  EMSCRIPTEN_KEEPALIVE void playPause() { paused ^= 1; }
  EMSCRIPTEN_KEEPALIVE void reset() {
    // a setup
    a = newPlanet();
    a->pos.x = 0;
    a->pos.y = 0;

    a->vel.x = 0;
    a->vel.y = 0.2;

    a->m = 10000;

    // b setup
    b = newPlanet();
    b->pos.x = 20;
    b->pos.y = 0;

    b->vel.x = 0;
    b->vel.y = -40;

    b->m = 100;
  }  
  EMSCRIPTEN_KEEPALIVE void update() {
    if (!paused) simulate(a, b);
    else resetTime();
  }
}
