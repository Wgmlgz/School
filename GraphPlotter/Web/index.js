const datapoints = graph.graphs.graphs[0].x;
const data = {
  labels: graph.graphs.graphs[0].x,
  datasets: [
    {
      label: "hull top",
      data: graph.hull.graphs[0].y,
      borderColor: "rgb(255, 102, 153)",
      tension: 0.4,
      fill: "+1",
      backgroundColor: "rgba(255, 102, 153, 0.2)",
    },
    {
      label: "hull bottom",
      data: graph.hull.graphs[1].y,
      borderColor: "rgb(255, 102, 153)",
      tension: 0.4,
    },
    {
      label: "a",
      data: graph.graphs.graphs[0].y,
      borderColor: "rgb(153, 102, 255)",
      tension: 0.4,
    },
    {
      label: "b",
      data: graph.graphs.graphs[1].y,
      borderColor: "rgb(153, 102, 255)",
      tension: 0.4,
    },
    {
      label: "c",
      data: graph.graphs.graphs[2].y,
      borderColor: "rgb(153, 102, 255)",
      tension: 0.4,
    },
  ],
};
var ctx = document.getElementById("myChart").getContext("2d");
var chart = new Chart(ctx, {
  type: "line",
  data: data,
  options: {
    responsive: true,
    maintainAspectRatio: false,
    plugins: {
      legend: {
        position: "top",
      },
      title: {
        display: true,
        text: "kill me pls",
      },
      // zoom: {
      //   zoom: {
      //     wheel: {
      //       enabled: true,
      //     },
      //     pinch: {
      //       enabled: true
      //     },

      //     mode: 'xy',
      //   }
      // },
    },
    elements: {
      point: {
        radius: 0,
      },
    },
    scales: {
      x: {
        suggestedMin: -10,
        suggestedMax: 10,
      },
      y: {
        suggestedMin: 0,
        suggestedMax: 5,
      },
    },
  },
});
chart.canvas.parentNode.style.height = "700px";
chart.canvas.parentNode.style.width = "700px";
