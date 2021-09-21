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
      },
    },
    elements: {
      point: {
        radius: 0,
      },
    },
    scales: {
      y: {
        min: -10,
        max: 10,
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

var last_refresh = new Date();
var last_refresh = new Date();

function executeAsync(func) {
  setTimeout(func, 0);
}

function refresh() {
  var cur = new Date();

  if (cur.getTime() - last_refresh.getTime() < 16) {
    return;
  }

  _EXPORT_setLhs(document.getElementById("lhs").value);
  _EXPORT_setRhs(document.getElementById("rhs").value);
  _EXPORT_setStep(document.getElementById("step").value);

  var str = "";
  input.forEach((element) => (str += element.value + "|"));

  var buffer = Module._malloc(str.length + 1);
  writeAsciiToMemory(str, buffer);
  _EXPORT_setGraphs(buffer);

  var p = _EXPORT_getJson();
  graph = JSON.parse(UTF8ToString(p));
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

  _EXPORT_free(p);
  chart.config.data = data;
  chart.update();

  last_refresh = new Date();
}

var target_surface = 0,
  cur_surface = 0;

function refreshSurface() {
  target_surface = _EXPORT_getSurface();
}

window.onload = function () {
  // refresh();
  function test() {
    cur_surface += (target_surface - cur_surface) * 0.25;
    document.getElementById("surface").innerHTML = cur_surface.toPrecision(6);
  }
  setInterval(test, 16);
};
