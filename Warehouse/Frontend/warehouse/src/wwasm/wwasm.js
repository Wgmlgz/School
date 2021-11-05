import React, { Component, useEffect, useState } from "react";
import WWasmModule from "C:/Code/School/Warehouse/Frontend/warehouse/src/build.mjs";

export let ioGetInt = () => null;
export let ioSetInt = () => null;
export let ioGetDouble = () => null;
export let ioSetDouble = () => null;
export let ioGetStr = () => null;
export let ioSetStr = () => null;


export let getCanvasData = () => null;
export let getCanvasJSON = () => null;
export let readUint8Array = () => null;
export let wwasmUpdate = () => null;
export let wwasmInvoke = () => null;

export let is_init = false;

WWasmModule().then((Module) => {
  ioSetInt = Module.cwrap("ioSetInt", "number", ["string", "number"]);
  ioGetInt = Module.cwrap("ioGetInt", "number", ["string"]);

  ioSetDouble = Module.cwrap("ioSetDouble", "number", ["string", "number"]);
  ioGetDouble = Module.cwrap("ioGetDouble", "number", ["string"]);

  ioSetStr = Module.cwrap("ioSetStr", "string", ["string", "string"]);
  ioGetStr = Module.cwrap("ioGetStr", "string", ["string"]);
  wwasmInvoke = Module.cwrap("wwasmInvoke", "number", ["string"]);

  getCanvasData = Module.cwrap("getCanvasData", "number", [
    "string",
    "number",
    "number",
  ]);
  getCanvasJSON = Module.cwrap("getCanvasJSON", "string", [
    "string",
    "number",
    "number",
  ]);
  wwasmUpdate = Module.cwrap("wwasmUpdate", "number", ["number"]);
  readUint8Array = (ptr, size) => {
    return new Uint8Array(Module.HEAPU8.buffer, ptr, size);
  };
  setInterval(() => {
    wwasmUpdate(16);
  }, 16);
  is_init = true;
});

function writeImageDataToCanvas(canvas, data, width, height) {
  canvas.width = width;
  canvas.height = height;
  var context = canvas.getContext("2d");
  var imageData = context.createImageData(width, height);
  if (imageData === undefined || imageData === null) return;
  if (data === undefined || data === null) return;
  imageData.data.set(data);
  context.putImageData(imageData, 0, 0);
}

export function drawCanvas(canvas_id, w, h) {
  let ptr = getCanvasData(canvas_id, w, h);
  if (!ptr) return;
  writeImageDataToCanvas(
    document.getElementById(canvas_id),
    readUint8Array(ptr, w * h * 4),
    w,
    h
  );
}

export function drawCanvasJSON(canvas_id, w, h) {
  let s = getCanvasJSON(canvas_id, w, h);
  let data = JSON.parse(s);
  if (data === null) return;
  let canvas = document.getElementById(canvas_id);
  const ctx = canvas.getContext("2d");

  ctx.fillStyle = "#282a3600";
  ctx.fillRect(0, 0, canvas.width, canvas.height);
  data.data.forEach(i => {
    if (i.type === "line") {
      ctx.beginPath();
      ctx.moveTo(i.ax, h - i.ay);
      ctx.lineTo(i.bx, h - i.by);
      ctx.strokeStyle = i.col;
      ctx.lineWidth = 2;
      ctx.stroke();
    } else if (i.type === "img") {
      ctx.drawImage(document.getElementById(i.img_id), i.x, h - i.y - i.h, i.w, i.h);
    }
  });
}

