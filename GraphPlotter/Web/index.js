const datapoints = graph.graphs.graphs[0].x;
var data = {
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
    animation: {
      duration: 0,
    },
    responsive: true,
    maintainAspectRatio: false,
    plugins: {
      legend: {
        display: false,
        position: "top",
      },
      // title: {
      //   display: true,
      // },
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
chart.canvas.parentNode.style.height = "800px";
chart.canvas.parentNode.style.width = "800px";

var input = [
  document.getElementById("fun0"),
  document.getElementById("fun1"),
  document.getElementById("fun2"),
];

input.forEach((element) => (element.oninput = refresh));

function addData(chart, label, data) {
  chart.data.labels.push(label);
  chart.data.datasets.forEach((dataset) => {
    dataset.data.push(data);
  });
  chart.update();
}

function removeData(chart) {
  chart.data.labels.pop();
  chart.data.datasets.forEach((dataset) => {
    dataset.data.pop();
  });
  chart.update();
}

var da = 0;
function refresh() {
  var str = "";
  input.forEach((element) => (str += element.value + "|"));
  // document.getElementById("deb").innerHTML = str;

  var buffer = Module._malloc(str.length + 1);
  writeAsciiToMemory(str, buffer);
  _EXPORT_setGraphs(buffer);

  // _EXPORT_setStr(buffer);

  // document.getElementById("deb2").innerHTML = UTF8ToString(_EXPORT_getStr());
  // UTF8ToString(_EXPORT_getJson());
  graph = JSON.parse(UTF8ToString(_EXPORT_getJson()));
  data = {
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
  chart.config.data = data;
  chart.update();
}
refresh();
