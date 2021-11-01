import React from 'react'
import ReactJson from "react-json-view";
import {
  ioGetStr,
  ioSetStr,
} from "./wwasm/wwasm.js";

export default function Stats() {
  setInterval(() => {
    let json = ioGetStr("warehouse_json");
    console.log(json);
  }, 100);
  let t = {da: "pizda"};
  return (
    <div>
      <ReactJson src={t} />
    </div>
  );
}
