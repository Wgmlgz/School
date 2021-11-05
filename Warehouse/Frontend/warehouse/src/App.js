import "./App.scss";

import CssBaseline from "@material-ui/core/CssBaseline";

import React from "react";
import "./index.css";

import Button from "@material-ui/core/Button";
import AppBar from "@material-ui/core/AppBar";
import Toolbar from "@material-ui/core/Toolbar";
import Typography from "@material-ui/core/Typography";
import Dialog from "@material-ui/core/Dialog";
import DialogTitle from "@material-ui/core/DialogTitle";
import { Container } from "@material-ui/core";
import { ThemeProvider, createTheme } from "@material-ui/core/styles";

import ReactJson from "react-json-view";

import MainPage from "./MainPage.jsx";
import BarButton from "./BarButton.jsx";

import { ioSetStr, wwasmInvoke, ioSetInt } from "./wwasm/wwasm.js";

import FoodImages from "./Food/Food.jsx";

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
    auto_mode: true,
    is_main: false,
    src: {
      packages: {
        n: 20,
      },
      clients: {
        n: 6,
        amount: {
          mean: 8.0,
          stddev: 2.0,
        },
        type: {
          base: 10,
          diff: 10,
        },
        "request propability": {
          mean: 0.8,
          stddev: 0.2,
        },
      },
      factory: {
        "wait time": {
          mean: 3.0,
          stddev: 1.0,
        },
      },
      warehouse: {
        "max capacity": 33,
        threshold: 0.75,
      },
    },
  };

  Restart = () => {
    this.setState({ is_main: false });
  };

  OnClose = () => {
    console.log("da");

    ioSetStr("settings", JSON.stringify(this.state.src));
    console.log(this.state.src);

    this.setState({ is_main: true });
    wwasmInvoke("restart");
  };

  ToggleAutoMode = () => {
    let t = this.state.auto_mode;
    t = !t;
    this.setState({ auto_mode: t});
    ioSetInt("auto_mode", t);
  };

  NextDay = () => {
    wwasmInvoke("next_day");
  };

  render() {
    return (
      <ThemeProvider theme={theme}>
        <CssBaseline />
        <div>
          <AppBar position="static">
            <Toolbar variant="dense">
              <Typography
                variant="h5"
                component="div"
                style={{ margin: "10px" }}
              >
                Warehouse simulator by Wgmlgz
              </Typography>
              <BarButton content="Restart" event={this.Restart} />
              <BarButton content="Report" event={this.Restart} />
              <BarButton
                content="Toggle auto mode"
                event={this.ToggleAutoMode}
              />
              <BarButton content="Next day" event={this.NextDay} />
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
            <Dialog open={!this.state.is_main}>
              <Container>
                <DialogTitle>Warehouse simulation settings:</DialogTitle>
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

                <BarButton content="Apply and run" event={this.OnClose} />
              </Container>
            </Dialog>
          </div>
        </div>
        <FoodImages />
      </ThemeProvider>
    );
  }
}
