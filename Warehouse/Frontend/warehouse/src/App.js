import "./App.scss";
import "./Frogs/frog1.jpg";

import ClientImg from "./Frogs/Client.png";
import WarehouseImg from "./Frogs/Warehouse.png";
import FactoryImg from "./Frogs/Factory.png";
import TrashImg from "./Frogs/Trash.png";
import RequestImg from "./Frogs/Request.png";
import PackageImg from "./Frogs/Package.png";
import ShelfImg from "./Frogs/Shelf.png";

import Button from "@material-ui/core/Button";
import Box from "@material-ui/core/Box";
import AppBar from "@material-ui/core/AppBar";
import Toolbar from "@material-ui/core/Toolbar";
import IconButton from "@material-ui/core/IconButton";
import Typography from "@material-ui/core/Typography";
import Card from "@material-ui/core/Card";

import TextField from "@material-ui/core/TextField";
import CssBaseline from "@material-ui/core/CssBaseline";

import ReactJson from "react-json-view";
import { ThemeProvider, createTheme } from "@material-ui/core/styles";

import React, { useState, setEffect } from "react";
import "./index.css";
// import Stats from './Stats.jsx'
import Frog from "./Frog.js";
import WCanvas from "./wwasm/canvas.jsx";

import { ioGetStr, ioSetStr } from "./wwasm/wwasm.js";
import {
  CardContent,
  CardHeader,
  Container,
  Grid,
  Paper,
} from "@material-ui/core";

const theme = createTheme({
  palette: {
    type: "dark",
    primary: {
      main: "#bd93f9",
    },
    secondary: {
      main: "#50fa7b",
    },
    background: {
      default: "#181a1f",
      paper: "#44475a",
    },
    error: {
      main: "#ffb86c",
    },
    info: {
      main: "#8be9fd",
    },
    success: {
      main: "#50fa7b",
    },
  },
  typography: {
    fontFamily: "Rubik",
  },
  shape: {
    borderRadius: 20,
  },
});

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

export default class App extends React.Component {
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
      <ThemeProvider theme={theme}>
        <CssBaseline />
        <div>
          <div>
            <AppBar position="static">
              <Toolbar variant="dense">
                <Typography
                  variant="h5"
                  color="#fff"
                  component="div"
                  style={{ margin: "10px" }}
                >
                  Warehouse simulator by Wgmlgz
                </Typography>
                <Button
                  variant="contained"
                  size="large"
                  color="secondary"
                  onClick={this.updateStats}
                  style={{ margin: "10px" }}
                >
                  Restart
                </Button>
                <Button
                  variant="contained"
                  size="large"
                  color="secondary"
                  onClick={this.updateStats}
                  style={{ margin: "10px" }}
                >
                  Report
                </Button>
              </Toolbar>
            </AppBar>

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
            </div>
            <div className="hidden">
              <img src={ClientImg} id="client_img" />
              <img src={WarehouseImg} id="warehouse_img" />
              <img src={FactoryImg} id="factory_img" />
              <img src={TrashImg} id="trash_img" />
              <img src={RequestImg} id="request_img" />
              <img src={ShelfImg} id="shelf_img" />
              <img src={PackageImg} id="package_img" />
            </div>
          </div>
        </div>
      </ThemeProvider>
    );
  }
}
