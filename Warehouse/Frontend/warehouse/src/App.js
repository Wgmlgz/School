import "./App.scss";
import "./Frogs/frog1.jpg";

import CssBaseline from "@material-ui/core/CssBaseline";

import { ThemeProvider, createTheme } from "@material-ui/core/styles";

import React, { useState, setEffect } from "react";
import "./index.css";

import Button from "@material-ui/core/Button";
import Box from "@material-ui/core/Box";
import AppBar from "@material-ui/core/AppBar";
import Toolbar from "@material-ui/core/Toolbar";
import IconButton from "@material-ui/core/IconButton";
import Typography from "@material-ui/core/Typography";
import Card from "@material-ui/core/Card";

import TextField from "@material-ui/core/TextField";

import Dialog from "@material-ui/core/Dialog";
import DialogActions from "@material-ui/core/DialogActions";
import DialogContent from "@material-ui/core/DialogContent";
import DialogContentText from "@material-ui/core/DialogContentText";
import DialogTitle from "@material-ui/core/DialogTitle";
import FormControl from "@material-ui/core/FormControl";
import FormControlLabel from "@material-ui/core/FormControlLabel";
import InputLabel from "@material-ui/core/InputLabel";
import MenuItem from "@material-ui/core/MenuItem";
import Select from "@material-ui/core/Select";
import Switch from "@material-ui/core/Switch";

import ReactJson from "react-json-view";

import MainPage from "./MainPage.jsx";
import { Container } from "@material-ui/core";

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
    is_main: false,
    src: { test: "da", among: "us" }
  };

  Reset = () => {
    console.log("da");
    this.setState({ is_main: false });
  };

  handleClose = () => {
    this.setState({ is_main: true });
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
                  onClick={this.Reset}
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

            <div
              style={{ visibility: this.state.is_main ? "visible" : "hidden" }}
            >
              <MainPage />
            </div>
            <div
              style={{ visibility: this.state.is_main ? "hidden" : "visible" }}
            >
              <Dialog open={!this.state.is_main} onClose={this.handleClose}>
                <Container>
                  <DialogTitle>IMPOSTOR from game anong aus!!!!</DialogTitle>
                  <Container>
                    <ReactJson
                      theme={dracula16}
                      style={style}
                      quotesOnKeys={false}
                      displayDataTypes={false}
                      displayObjectSize={false}
                      enableClipboard={false}
                      iconStyle="triangle"
                      name="data"
                      src={this.state.src}
                      onEdit={(e) => {
                        console.log(e);
                        this.setState({ src: e.updated_src });
                      }}
                    />
                  </Container>

                  <Button
                    variant="contained"
                    size="large"
                    color="secondary"
                    onClick={this.handleClose}
                    style={{ margin: "10px" }}
                  >
                    gg
                  </Button>
                </Container>
              </Dialog>
            </div>
          </div>
        </div>
      </ThemeProvider>
    );
  }
}
