import React, {useState} from 'react'
import ReactJson from "react-json-view";
import {
  ioGetStr,
  ioSetStr,
} from "./wwasm/wwasm.js";

// export default function Stats() {
//   let [json, setJson] = useState();
//   setInterval(() => {
//     let data = ioGetStr("warehouse_json");
//     setJson(JSON.parse(data));
//   }, 4000);

//   let t = {da: "pizda"};
//   return (
//     <div>

//     </div>
//   );
// }
