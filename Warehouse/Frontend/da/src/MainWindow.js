import React, { Component, useState } from "react";

import frog1 from "./Frogs/frog1.jpg";
import frog2 from "./Frogs/frog2.jpg";
import frog3 from "./Frogs/frog3.jpg";
import frog4 from "./Frogs/frog4.jpg";
import frog5 from "./Frogs/frog5.jpg";
import frog6 from "./Frogs/frog6.jpg";

export default function MainWindow() {
  return (
    <div>
      <img src={frogs[frog_id]} alt="frog" className="frog" />
    </div>
  );
}
