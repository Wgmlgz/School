import "./App.scss";

import ClientImg from "./Images/Client.png";
import WarehouseImg from "./Images/Warehouse.png";
import FactoryImg from "./Images/Factory.png";
import TrashImg from "./Images/Trash.png";
import RequestImg from "./Images/Request.png";
import PackageImg from "./Images/Package.png";
import ShelfImg from "./Images/Shelf.png";

import Button from "@material-ui/core/Button";
import Typography from "@material-ui/core/Typography";


import ReactJson from "react-json-view";

import React from "react";
import "./index.css";
import WCanvas from "./wwasm/canvas.jsx";

import { ioGetStr } from "./wwasm/wwasm.js";
import { 
  Container,
  Grid,
  Paper,
} from "@material-ui/core";

const dracula16 = {
  base00: "#44475a",
  base01: "#3a3c4e",
  base02: "#4d4f68",
  base03: "#626483",
  base04: "#62d6e8",
  base05: "#e9e9f4",
  base06: "#f1f2f8",
  base07: "#f7f7fb",
  base08: "#ea51b2",
  base09: "#b45bcf",
  base0A: "#00f769",
  base0B: "#ebff87",
  base0C: "#a1efe4",
  base0D: "#62d6e8",
  base0E: "#b45bcf",
  base0F: "#00f769",
};

const style = {
  fontFamily: "Rubik",
  fontSize: "15px",
};

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
            <Paper>
              <Container style={{ display: "flex", padding: "10px" }}>
                <Typography variant="h4" style={{ margin: "10px" }}>
                  Warehouse orders and requests
                </Typography>
                <Button
                  variant="contained"
                  size="large"
                  color="primary"
                  onClick={this.updateStats}
                  style={{ margin: "10px" }}
                >
                  Update
                </Button>
              </Container>
              <Container>
                <ReactJson
                  theme={dracula16}
                  style={style}
                  quotesOnKeys={false}
                  enableClipboard={false}
                  displayObjectSize={false}
                  displayDataTypes={false}
                  iconStyle="triangle"
                  name="data"
                  src={this.state.warehouse_json}
                />
                <br />
              </Container>
            </Paper>
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
