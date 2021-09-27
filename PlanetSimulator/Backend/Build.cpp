#include <iostream>
#include "../../Lib/Wasm/WasmLib.hpp"
#include "engine.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE 
#endif

wasm::Canvas<double> canvas(650, 1000);

const int PLANETS = 3;
Planet *planets[PLANETS];

extern "C" {
  // render image & update input
  EMSCRIPTEN_KEEPALIVE uint8_t* getTestData() {
    static int selected = 0;
    if (paused and selected == wasm::ioGetInt(1)) {
      planets[selected]->pos.x = wasm::ioGetDouble(10);
      planets[selected]->pos.y = wasm::ioGetDouble(11);
      planets[selected]->vel.x = wasm::ioGetDouble(12);
      planets[selected]->vel.y = wasm::ioGetDouble(13);

      planets[selected]->m = wasm::ioGetDouble(15);
    }
    selected = wasm::ioGetInt(1);
    
    wasm::ioSetDouble(10, planets[selected]->pos.x);
    wasm::ioSetDouble(11, planets[selected]->pos.y);
    wasm::ioSetDouble(12, planets[selected]->vel.x);
    wasm::ioSetDouble(13, planets[selected]->vel.y);
    wasm::ioSetDouble(15, planets[selected]->m);

    canvas.setX(wasm::ioGetDouble(1));
    canvas.setY(wasm::ioGetDouble(2));
    canvas.setZoom(wasm::ioGetDouble(3));

    canvas.fill(0, 0, 0, 0);
    for (int i = 0; i < PLANETS; ++i) {
      canvas.drawNgon(planets[i]->pos.x, planets[i]->pos.y, sqrt(planets[i]->m) / 10, 8, 255, 85, 85);
      canvas.drawLine(
        {{planets[i]->pos.x, planets[i]->pos.y}, {planets[i]->pos.x + planets[i]->vel.x, planets[i]->pos.y + planets[i]->vel.y}}, 80, 250, 123);
    }
   
    return canvas.data();
  }

  // play / pause
  EMSCRIPTEN_KEEPALIVE void playPause() { paused ^= 1; }
  
  // reset to default planets
  EMSCRIPTEN_KEEPALIVE void reset() {
    // a setup
    planets[0] = newPlanet();
    planets[0]->pos.x = 0;
    planets[0]->pos.y = 0;
    planets[0]->vel.x = 0;
    planets[0]->vel.y = 0;
    planets[0]->m = 100000;

    // b setup
    planets[1] = newPlanet();
    planets[1]->pos.x = 200;
    planets[1]->pos.y = 0;
    planets[1]->vel.x = 0;
    planets[1]->vel.y = 60;
    planets[1]->m = 1000;

    // c setup
    planets[2] = newPlanet();
    planets[2]->pos.x = 220;
    planets[2]->pos.y = 0;
    planets[2]->vel.x = 0;
    planets[2]->vel.y = 70;
    planets[2]->m = 50;
  }  

  // update phisics
  EMSCRIPTEN_KEEPALIVE void update() {
    if (!paused) simulate(planets, PLANETS);
    else resetTime();
  }
}
