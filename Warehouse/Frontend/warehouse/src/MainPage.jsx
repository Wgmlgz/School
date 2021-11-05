import "./App.scss";

import ClientImg from "./Images/Client.png";
import WarehouseImg from "./Images/Warehouse.png";
import FactoryImg from "./Images/Factory.png";
import TrashImg from "./Images/Trash.png";
import RequestImg from "./Images/Request.png";
import PackageImg from "./Images/Package.png";
import ShelfImg from "./Images/Shelf.png";

import Stats from "./Stats.jsx"

import React from "react";
import WCanvas from "./wwasm/canvas.jsx";

import { ioGetStr } from "./wwasm/wwasm.js";
import { 
  Grid,
  Paper,
} from "@material-ui/core";

export default class MainPage extends React.Component {
  state = {
    warehouse_json: {},
  };
  
  updateStats = () => {
    let data = ioGetStr("warehouse_json");
    let json = JSON.parse(data);
    this.setState({ warehouse_json: json });
  };

  render() {
    return (
      <div className="page">
        <Grid container spacing={2}>
          <Grid item xs={3}>
          <Stats />
          </Grid>
          <Grid item xs={9}>
            <Paper>
              <div className="canvas">
                <WCanvas canvas_id="main_canvas" render="js" />
              </div>
            </Paper>
          </Grid>
        </Grid>
        <div className="hidden">
          <img src={ClientImg} id="client_img" alt="" />
          <img src={WarehouseImg} id="warehouse_img" alt="" />
          <img src={FactoryImg} id="factory_img" alt="" />
          <img src={TrashImg} id="trash_img" alt="" />
          <img src={RequestImg} id="request_img" alt="" />
          <img src={ShelfImg} id="shelf_img" alt="" />
          <img src={PackageImg} id="package_img" alt="" />
        </div>
      </div>
    );
  }
}
