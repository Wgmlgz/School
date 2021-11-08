import "./App.scss";

import CssBaseline from "@material-ui/core/CssBaseline";

import React from "react";

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

import {
  ioSetStr,
  wwasmInvoke,
  ioGetInt,
  ioSetInt,
  ioGetStr,
} from "./wwasm/wwasm.js";

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
    show_fin: false,
    src: {
      "display stats day": -1,
      packages: {
        n: 20,
      },
      clients: {
        n: 10,
        amount: {
          mean: 8.0,
          stddev: 2.0,
        },
        type: {
          base: 10,
          diff: 10,
        },
        "request propability": {
          mean: 0.6,
          stddev: 0.2,
        },
        "outdated request probability": {
          mean: 0.8,
          stddev: 0.1,
          days: 4,
        },
      },
      factory: {
        "wait time": {
          mean: 3.0,
          stddev: 1.0,
        },
      },
      warehouse: {
        "extra charge": 1.5,
        "max capacity": 20,
        threshold: 0.75,
      },
    },
    results: {
      among: "us",
      aboba: "da",
    },
  };

  componentDidMount() {
    this.interval = setInterval(() => {
      let cur_day = ioGetInt("cur_day");
      if (this.state.src["display stats day"] == cur_day) {
        if (this.state.show_fin) return;
        this.DisplayResults();
      }
    }, 100);
  }

  componentWillUnmount() {
    clearInterval(this.interval);
  }

  Restart = () => {
    this.setState({ is_main: false });
  };

  Continue = () => {
    this.setState({ show_fin: false });
  };

  OnClose = () => {
    console.log("da");

    ioSetStr("settings", JSON.stringify(this.state.src));

    this.setState({ is_main: true });
    wwasmInvoke("restart");
  };

  ToggleAutoMode = () => {
    let t = this.state.auto_mode;
    t = !t;
    this.setState({ auto_mode: t });
    ioSetInt("auto_mode", t);
  };

  NextDay = () => {
    wwasmInvoke("next_day");
  };

  DisplayResults = () => {
    let s = ioGetStr("results_json");
    this.setState({ results: JSON.parse(s) });
    this.setState({ show_fin: true });
  };

  render() {
    return (
      <ThemeProvider theme={theme}>
        <CssBaseline />
        {/* Bar */}
        <AppBar position="static">
          <Toolbar variant="dense">
            <Typography variant="h5" component="div" style={{ margin: "10px" }}>
              Warehouse simulator by Wgmlgz
            </Typography>
            <BarButton content="Restart" event={this.Restart} />
            <BarButton
              content="Report"
              event={() =>
                window.open(
                  "https://wgmlgz.github.io/Warehouse/Docs/Report",
                  "_blank"
                )
              }
            />
            <BarButton content="Toggle auto mode" event={this.ToggleAutoMode} />
            <BarButton content="Next day" event={this.NextDay} />
            <BarButton content="Display results" event={this.DisplayResults} />
          </Toolbar>
        </AppBar>

        {/* Main page */}
        <div style={{ visibility: this.state.is_main ? "visible" : "hidden" }}>
          <MainPage />
          <FoodImages />
        </div>

        {/* Input settings */}
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

        {/* Stats screen */}
        <Dialog open={this.state.show_fin}>
          <Container>
            <DialogTitle>Simulation results:</DialogTitle>
            <Container>
              <ReactJson
                theme={dracula16}
                style={style}
                quotesOnKeys={false}
                displayDataTypes={false}
                displayObjectSize={false}
                enableClipboard={false}
                iconStyle="triangle"
                name="results"
                src={this.state.results}
              />
            </Container>
            <BarButton content="Restart" event={this.Restart} />
            <BarButton content="Update" event={this.DisplayResults} />
            <BarButton content="Continue" event={this.Continue} />
          </Container>
        </Dialog>
      </ThemeProvider>
    );
  }
}
