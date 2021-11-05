import React, {useState} from 'react'
import ReactJson from "react-json-view";
import {
  ioGetStr,
  ioSetStr,
} from "./wwasm/wwasm.js";

import { 
  Container,
  Grid,
  Paper,
  Typography,
  Button,
} from "@material-ui/core";

import BarButton from "./BarButton.jsx";

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

export default class Stats extends React.Component {
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
     <Paper>
      <Container style={{ display: "flex", padding: "10px" }}>
        <Typography variant="h4" style={{ margin: "10px" }}>
          Warehouse stats
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
      <Container
        style={{ maxHeight: "calc(100vh - 190px)", overflow: "auto" }}
      >
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
  );
}

}