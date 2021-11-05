import React from "react";
import Button from "@material-ui/core/Button";


export default function BarButton(props) {
  return (
    <Button
      variant="contained"
      size="large"
      color="secondary"
      onClick={props.event}
      style={{ margin: "10px" }}
    >
        {props.content}
    </Button>
  );
}
