import React from "react";
import "./canvas.css";
import { wwasmUpdate, is_init, ioGetDouble, ioSetDouble, drawCanvas, drawCanvasJSON } from "./wwasm.js";

export default function WCanvas(props) {
  let canvas_id = props.canvas_id;

  let zoom = 1;

  let offset_x = 0;
  let offset_y = 0;

  let down_x = 0;
  let down_y = 0;

  let is_down = false;

  function wheel(event) {
    zoom *= 1 + event.deltaY * -0.001 * 1.5;
    zoom = Math.min(Math.max(0.1, zoom), 100);
    ioSetDouble(canvas_id + "_zoom", zoom);
  }

  function btnDown(e) {
    is_down = true;
    down_x = -e.screenX;
    down_y = e.screenY;
  }

  function btnUp(e) {
    is_down = false;
    offset_x = ioGetDouble(canvas_id + "_x");
    offset_y = ioGetDouble(canvas_id + "_y");
  }

  function dragCanvas(e) {
    let x = -e.screenX;
    let y = e.screenY;

    if (is_down) {
      x = (x - down_x) * (1 / zoom) + offset_x;
      y = (y - down_y) * (1 / zoom) + offset_y;
      ioSetDouble(canvas_id + "_x", x);
      ioSetDouble(canvas_id + "_y", y);
    }
  }

  function canvasUpdate() {
    if (!is_init) return;

    let canvas_div = document.getElementById(canvas_id + "_div");
    let canvas = document.getElementById(canvas_id);
    let w = canvas_div.parentElement.clientWidth;
    let h = canvas_div.parentElement.clientHeight;
    canvas.width = w;
    canvas.height = h;

    wwasmUpdate(16);
    if (props.render && props.render == "cpp") {
      drawCanvas(canvas_id, w, h);
    } else {
      drawCanvasJSON(canvas_id, w, h);
    }
  }

  setInterval (canvasUpdate, 16);

  requestAnimationFrame(() => {
    canvasUpdate();
    requestAnimationFrame(canvasUpdate)
  });


  return (<div id={canvas_id + "_div"}>
      <canvas
        id={canvas_id}
        onMouseDown={btnDown}
        onMouseUp={btnUp}
        onMouseMove={dragCanvas}
        onWheel={wheel}
      ></canvas>
    </div>);
}
