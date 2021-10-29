import "./App.scss";
import "./Frogs/frog1.jpg";
import FrogImg from "./Frogs/frog1.jpg";
import SmolFrogImg from "./Frogs/frog2.jpg";


import ClientImg from "./Frogs/Client.png";
import WarehouseImg from "./Frogs/Warehouse.png";
import FactoryImg from "./Frogs/Factory.png";
import TrashImg from "./Frogs/Trash.png";
import RequestImg from "./Frogs/Request.png";

import Button from "@material-ui/core/Button";
import AppBar from "@material-ui/core/AppBar";
import Toolbar from "@material-ui/core/Toolbar";
import IconButton from "@material-ui/core/IconButton";
import Typography from "@material-ui/core/Typography";
import TextField from "@material-ui/core/TextField";
import CssBaseline from "@material-ui/core/CssBaseline";

import Background from "./Background.png";
import { ThemeProvider, createTheme } from "@material-ui/core/styles";

import { TransformWrapper, TransformComponent } from "react-zoom-pan-pinch";


import React, { useState, setEffect } from "react";
import ReactDOM from "react-dom";
import "./index.css";

import Frog from "./Frog.js"
import WCanvas from "./wwasm/canvas.jsx";

const theme = createTheme({
  palette: {
    primary: {
      main: "#e06c75",
    },
    secondary: {
      main: "#c678dd",
    },
    background: {
      default: "#181a1f",
      paper: "#282c34",
    },
  },
  typography: {
    fontFamily: "Rubik",
  },
  shape: {
    borderRadius: 20,
  },
});

export default class App extends React.Component {
  state = {
    frogid: 0,
  };
  changeFrog = () => {
    let id = Math.floor(Math.random() * 6);
    this.setState({ frogid: id });
  };

  render() {
    return (
      <ThemeProvider theme={theme}>
        <CssBaseline />
        <div>
          <div>
            <AppBar position="static">
              <Toolbar variant="dense">
                <IconButton
                  edge="start"
                  color="primary"
                  aria-label="menu"
                  sx={{ mr: 2 }}
                ></IconButton>
                <Typography variant="h6" color="inherit" component="div">
                  Frog website
                </Typography>
              </Toolbar>
            </AppBar>
            <main className="mdc-top-app-bar--fixed-adjust">
              {/* <Frog frog_id={this.state.frogid} />
              <Button
                variant="contained"
                size="large"
                color="primary"
                onClick={this.changeFrog}
              >
                Change FROG!!!!!
              </Button> */}
              <div className="hidden">
                <img src={ClientImg} id="client_img" />
                <img src={WarehouseImg} id="warehouse_img" />
                <img src={FactoryImg} id="factory_img" />
                <img src={TrashImg} id="trash_img" />
                <img src={RequestImg} id="request_img" />
              </div>
              <div className="canvas">
                <WCanvas canvas_id="main_canvas" render="js" />
              </div>
            </main>
          </div>
        </div>
      </ThemeProvider>
    );
  }
}
