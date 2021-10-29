#include "wwasm/wwasm.hpp"
#include "UiEngine.hpp"


    /* Keyframes for animation */
    // auto a = wwasm::frm{"pos_x", 0_cnt} | wwasm::frm{"pos_y", 0_cnt};
    // auto b = wwasm::frm{"pos_x", 500_cnt} | wwasm::frm{"pos_y", 500_cnt};
    // auto c = wwasm::frm{"pos_x", -500_cnt} | wwasm::frm{"pos_y", 500_cnt};

    // /* Animation build from keyframes */
    // auto an = wwasm::anim({{0, a}, {2, b}, {6, c}, {7, a}});

    /* Just fill canvas with some frogs */
    // void makeDemoCanvas(wwasm::Canvas& canvas) {
    //   canvas.pushEntity("frog",
    //                     new wwasm::Img({0, 0}, "frog", {1080, 1038}));
    //   canvas.pushEntity(
    //       "smol frog", new wwasm::Img({0, 0}, "smol frog", {1080, 1061}));

    //   canvas.playAnim("frog", an);

    //   std::cout << "Frogger gang" << std::endl;
    // }

auto _init = wwasm::init([]() {
  
});

auto _update = wwasm::update([]() {
  ui_engine.update();
});

// int main() {
//   while (true) {
//     // wwasm::wwasmUpdate(16);
//     // std::cout << "da" << std::endl;
//   }
// }