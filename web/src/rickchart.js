// Generated by CoffeeScript 1.7.1
var charter, grapher,
  __bind = function(fn, me){ return function(){ return fn.apply(me, arguments); }; };

this.Graph = (function() {
  function Graph(element) {
    this.render = __bind(this.render, this);
    this.addData = __bind(this.addData, this);
    this.renderAxes = __bind(this.renderAxes, this);
    this.init = __bind(this.init, this);
    this.element = element;
    this.colorPalette = new Rickshaw.Color.Palette();
  }

  Graph.prototype.init = function() {
    this.graph = new Rickshaw.Graph({
      element: document.querySelector(this.element),
      width: document.width - 30,
      height: document.height - 30,
      renderer: 'bar',
      series: new Rickshaw.Series.FixedDuration([
        {
          name: 'baseline'
        }
      ], this.colorPalette, {
        timeInterval: 100,
        maxDataPoints: 50,
        timeBase: new Date().getTime() / 1000
      })
    });
    return this.renderAxes();
  };

  Graph.prototype.renderAxes = function() {
    new Rickshaw.Graph.Axis.Time({
      graph: this.graph
    }).render();
    new Rickshaw.Graph.Axis.Y.Scaled({
      graph: this.graph,
      tickFormat: Rickshaw.Fixtures.Number.formatKMBT,
      scale: d3.scale.linear()
    }).render();
    return new Rickshaw.Graph.HoverDetail({
      graph: this.graph,
      yFormatter: function(y) {
        return "Count: " + y;
      }
    });
  };

  Graph.prototype.addData = function(item) {
    return this.graph.series.addData(item);
  };

  Graph.prototype.render = function() {
    return this.graph.render();
  };

  return Graph;

})();

this.Charter = (function() {
  function Charter(grapher) {
    this.tryQtBridge = __bind(this.tryQtBridge, this);
    this.receiveObjectData = __bind(this.receiveObjectData, this);
    this.grapher = grapher;
  }

  Charter.prototype.receiveObjectData = function(objectData) {
    console.log("receiving data now " + (new Date()));
    this.grapher.addData(objectData);
    return this.grapher.graph.render();
  };

  Charter.prototype.tryQtBridge = function() {
    if (window.rbkitClient) {
      return window.rbkitClient.sendDatatoJs.connect(this.receiveObjectData);
    }
  };

  return Charter;

})();

grapher = new Graph('#chart');

grapher.init();

charter = new Charter(grapher);

setTimeout(charter.tryQtBridge, 1000);
