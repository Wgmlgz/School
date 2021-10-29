#include "wwasm.hpp"

using namespace wwasm::literals;

/* Keyframes for animation */
auto a = wwasm::frm{"pos_x", 0_cnt} | wwasm::frm{"pos_y", 0_cnt};
auto b = wwasm::frm{"pos_x", 500_cnt} | wwasm::frm{"pos_y", 500_cnt};
auto c = wwasm::frm{"pos_x", -500_cnt} | wwasm::frm{"pos_y", 500_cnt};

/* Animation build from keyframes */
auto an = wwasm::anim({{0, a}, {2, b}, {6, c}, {7, a}});

/* Just fill canvas with some frogs */
void makeDemoCanvas(wwasm::Canvas& canvas) {
  canvas.pushEntity("pentagon", new wwasm::Ngon(
    {-100, 0}, 50, 5, wwasm::Dracula::green
  ));
  canvas.pushEntity("triangle", new wwasm::Ngon(
    {600, 0}, 100, 3, wwasm::Dracula::pink
  ));
  canvas.pushEntity("line", new wwasm::Line(
    {-200, -200}, {700, -200}, wwasm::Dracula::purple
  ));
  canvas.pushEntity("frog", new wwasm::Img(
    {0, 0}, "frog", frog_bmp, frog_bmp_len
  ));
  canvas.pushEntity( "smol frog", new wwasm::Img(
    {0, 0}, "smol frog", frog_bmp, frog_bmp_len
  ));

  canvas.playAnim("frog", an);

  std::cout << "Frogger gang" << std::endl;
}

/**
 * This code runs when wwams is constructed
 * auto _init = wwasm::init([]() { <code> });
 */
auto _init = wwasm::init([]() {
  auto& canvas_cpp = wwasm::Canvas::regiesterCanvas("cpp");
  makeDemoCanvas(canvas_cpp);
  auto& canvas_js = wwasm::Canvas::regiesterCanvas("js");
  makeDemoCanvas(canvas_js);
});

/**
 * This code runs every frame
 * auto _update = wwasm::update([]() { <code> });
 */
auto _update = wwasm::update([]() {
  std::cout << "I love Froggs!!!" << std::endl;
});
