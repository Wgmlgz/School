import React, { Component, useState } from "react";

import frog1 from "./Frogs/frog1.jpg";
import frog2 from "./Frogs/frog2.jpg";
import frog3 from "./Frogs/frog3.jpg";
import frog4 from "./Frogs/frog4.jpg";
import frog5 from "./Frogs/frog5.jpg";
import frog6 from "./Frogs/frog6.jpg";

export default function Frog({ frog_id }) {
  let frogs = [frog1, frog2, frog3, frog4, frog5, frog6];
  return (
    <div>
      <img src={frogs[frog_id]} alt="frog" className="frog" />
    </div>
  );
}
