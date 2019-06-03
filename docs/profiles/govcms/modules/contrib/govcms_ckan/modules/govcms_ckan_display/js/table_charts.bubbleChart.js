(function ($) {
  /*
   * This is the D3 bubble chart implementation class for tableCharts.
   *
   * It is called by the table_charts.bubbleChart.js and assumes data has been parsed from
   * the table and all applicable settings and data are present.
   *
   */

  window.tableChartsChart = window.tableChartsChart || {};

  tableChartsChart.bubble = function (chartData) {

    // Self instance.
    var self = this.bubble;
    var data;
    var $chart;

    // Bubble options
    self.options = {
      label: null,
      width: null,
      height: null,
      multiTableMode: false
    };

    if ($.isArray(chartData) === true) {
      // Multiple tables mode should pass multiple chartData as a Array of charData.
      // Then we build only one chart for them.
      // Otherwise we will create one chart for one chardData.
      self.options.multiTableMode = true;

      // @todo make it configurable?
      self.options.width = 700;
      self.options.height = 700;
    }
    else if (chartData.data.type !== 'bubble') {
      // Check to see if chart type is correct.
      return;
    }

    /*
     * Initialize the bubble Chart.
     */
    self.init = function () {

      if (self.options.multiTableMode) {
        // In multiple table mode, we pick the first one for getting some options.
        var sampleChartData = chartData[0];
        $chart = $(sampleChartData.bindto);

        self.addNotes(chartData);

        // Convert the data into a bubble friendly format.
        data = parseMultiTables(chartData);

      }
      else {
        $chart = $(chartData.bindto);

        // Convert the data into a bubble friendly format.
        data = parseColumns(chartData);
      }

      /*
       * Disable tooltips & related vertical grid hover lines.
       * @TODO Find better way of doing this.
       *
       * Note: setting -> "tooltip.show: false" doesn't seem to work.
       */
      $('.c3-event-rects').css({'display': 'none'});

      $chart.once('bubble-chart', function () {

        if (!self.options.multiTableMode) {
          self.options.width = $chart.find('svg')[0].clientWidth;
          self.options.height = $chart.find('svg')[0].clientHeight;
        }

        // If the data is a Array, that means there are more than one root bubbles, we will then build one bubble chart
        // for each root.
        if ($.isArray(data)) {
          $chart.parents('.ckan-display-table-wrapper').before('<div class="bubble-chart-container c3" style="overflow: hidden"></div>');

          var dataIndex;
          for(dataIndex = 0; dataIndex < data.length; dataIndex++) {
            // Adding containers for each bubble chart here.
            $('.bubble-chart-container').append('<div class="bubble-chart bubble-chart-' + dataIndex + '" style="width:50%; float:left"></div>');
            self.buildChart(data[dataIndex], dataIndex);
          }
        }
        else {
          self.buildChart(data);
        }

      });
    };

    /*
     * Build bubble chart for each root bubble.
     */
    self.buildChart = function (data, index) {
      var bubble = {
        force: null,
        svg: null,
        link: null,
        node: null,
        root: null
      };

      /*
       * Init the bubble.
       */
      bubble.init = function () {

        // Create a div for the tooltips.
        bubble.div = d3.select("body").append("div")
          .attr("class", "tooltip__container__bubbles")
          .style("opacity", 0);

        // @TODO: Tweak these settings for gentler animations when children from expanded from parent node(s).
        bubble.force = d3.layout.force()
          .linkDistance(100)
          .gravity(-0.001)
          .friction(0.2)
          .size([self.options.width, self.options.height])
          .on("tick", bubble.tick);

        if (typeof index === 'undefined') {
          // This is the case one bubble for one data.
          bubble.svg = d3.select($($chart).find('.c3-chart')[0]).append("svg")
            .attr("width", self.options.width)
            .attr("height", self.options.height);
        }
        else {
          // This is the case more than one root bubble for one data.
          bubble.svg = d3.select('.bubble-chart-' + index).append("svg")
            .attr("width", self.options.width)
            .attr("height", self.options.height);
        }

        bubble.link = bubble.svg.selectAll(".link");
        bubble.node = bubble.svg.selectAll(".node");

        // Convert the data to JSON & parse.
        bubble.root = JSON.parse(JSON.stringify(data));

        // Start with nodes collapsed.
        bubble.root.fixed = true;
        bubble.root.x = self.options.width / 2;
        bubble.root.y = self.options.height / 2 - 80;

        // Collapse all children of root bubble.
        bubble.collapseChildren(bubble.root);
      };

      /*
       * Update the bubble.
       */
      bubble.update = function () {
        var nodes = bubble.flatten(bubble.root),
          links = d3.layout.tree().links(nodes);

        // Restart the force layout.
        bubble.force
          .nodes(nodes)
          .links(links)
          .start();

        // Update links.
        bubble.link = bubble.link.data(links, function (d) {
          return d.target.id;
        });

        bubble.link.exit().remove();

        bubble.link.enter().insert("line", ".node")
          .attr("class", "link");

        // Update nodes.
        bubble.node = bubble.node.data(nodes, function (d) {
          return d.id;
        });

        bubble.node.exit().remove();

        var nodeEnter = bubble.node.enter().append("g")
          .attr("class", "node")
          .on("click", bubble.click)
          .call(bubble.force.drag);

        nodeEnter.append("circle")
          .attr("r", function (d) {
            var r = Math.sqrt(d.size) / 5;
            return r > 25 ? r : 25;
          })
          .attr('title', function(d) {
            return d.name;
          })
          .on('mouseover', function(d) {
            bubble.div.transition()
              .duration(200)
              .style("opacity", .9);
            bubble.div.html('<div class="tooltip__bubbles__content"><div class="tooltip__bubbles__content__inner">' + d.name + '</div></div>')
              .style("left", (d3.event.pageX) + "px")
              .style("top", (d3.event.pageY - 28) + "px");
          })
          .on('mouseleave', function(d) {
            bubble.div.transition()
              .duration(500)
              .style("opacity", 0);
          })
          .on('mousemove', function(d) {
            bubble.div.style("left", (d3.event.pageX) + "px")
              .style("top", (d3.event.pageY - 28) + "px");
          })

        /*
         * Bubble value.
         */
        nodeEnter.append("text")
          .attr({
            'class': 'amount',
            'y': '.4em',
            'text-anchor': 'middle'
          })
          .style({
            "font-size": "14px",
            "fill": "#555"
          })
          .text(bubble.formatNumber);

        // Bubble note.
        nodeEnter.append("text")
          .attr({
            'class': 'note',
            'y': '3em'
          })
          .style({
            "font-size": "12px",
            "fill": "#025dab"
          })
          .text(function (d) {
            if (typeof(d.note) != 'undefined' && d.note !== null) {
              return 'Note:' + d.table + '.' + d.note;
            }
          });

        // Set the colour of the circle.
        bubble.node.select("circle")
          .style("fill", bubble.color);
      };

      bubble.tick = function () {
        bubble.link.attr("x1", function (d) {
          return d.source.x;
        })
          .attr("y1", function (d) {
            return d.source.y;
          })
          .attr("x2", function (d) {
            return d.target.x;
          })
          .attr("y2", function (d) {
            return d.target.y;
          });

        bubble.node.attr("transform", function (d) {
          return "translate(" + d.x + "," + d.y + ")";
        });
      };

      bubble.formatNumber = function(d) {
        if (!d.value) {
          return 'undefined';
        }
        var value = d.value;
        var newValue = parseInt(value);

        if (value >= 1000) {
          var suffixes = ["", "k", "m", "b","t"];
          var suffixNum = Math.floor( (""+value).length/3 );
          var shortValue = '';
          for (var precision = 2; precision >= 1; precision--) {
            shortValue = parseFloat( (suffixNum != 0 ? (value / Math.pow(1000,suffixNum) ) : value).toPrecision(precision));
            var dotLessShortValue = (shortValue + '').replace(/[^a-zA-Z 0-9]+/g,'');
            if (dotLessShortValue.length <= 2) { break; }
          }
          if (shortValue % 1 != 0)  shortNum = shortValue.toFixed(1);
          newValue = shortValue+suffixes[suffixNum];
        }

        return newValue;
      }

      /*
       * Sets the colour of the node.
       */
      bubble.color = function (d) {
        var defaultColor = "#b3d4fc";
        return d.color ? d.color : defaultColor;
      };

      /*
       * Toggle children on click.
       */
      bubble.click = function (d) {
        if (d3.event.defaultPrevented) return; // ignore drag
        if (d.children) {
          d._children = d.children;
          bubble.collapseChildren(d);
          d.children = null;
        } else {
          d.children = d._children;
          d._children = null;
        }

        if (d.children || d._children) {
          // Toggle expanded class if this bubble has children.
          var thisBubble = d3.select(this);
          thisBubble.classed("expanded", !thisBubble.classed("expanded"));
        }

        bubble.update();
      };

      /*
       * Returns a list of all nodes under the root.
       */
      bubble.flatten = function (root) {
        var nodes = [], i = 0;

        function recurse(node) {
          if (node.children) node.children.forEach(recurse);
          if (!node.id) node.id = ++i;
          nodes.push(node);
        }

        recurse(root);
        return nodes;
      };

      /*
       * Collapse all children bubble.
       */
      bubble.collapseChildren = function (node) {
        var flatNodes = bubble.flatten(node);
        flatNodes.forEach(function (d) {
          if (d.children) {
            d._children = d.children;
            d.children = null;
          }
        });
      };

      // Init bubble.
      bubble.init();

      // Now that nodes are set up, run update().
      bubble.update();
    };

    self.addNotes = function(chartData) {
      // Get the array of table selectors. If not available, bail.
      if (!Drupal.settings.govcmsCkanDisplay.tableChartSelectors) {
        return;
      }

      var settings = Drupal.settings.govcmsCkanDisplay.rowIds;

      // Get the footnote key-value data, that will be used to find the right table cell.
      if (!settings.allRowIds) {
        return;
      }

      // Get the footnote rowIds so we know where to append the footnote to.
      if (!settings.datasetNoteRowIds) {
        return;
      }

      if (self.options.multiTableMode) {
        // Add notes to chartData for multiple tables mode.
        $.each(chartData, function (k, elem) {
          elem.data.notes = {
            'allRowIds': settings.allRowIds[k],
            'datasetRowIds': settings.datasetNoteRowIds[k]
          }
        });
      }
      else {
        // @todo add notes to normal mode.
      }
    };

    // Create the bubble.
    self.init();

  };

  /*
   * Function to convert data in columns to bubble friendly nested data.
   * Not used for multi table mode.
   *
   * @todo this may need to be refactored to use new buildLeaf() function.
   */
  function parseColumns(chartData) {
    // Set up initial object.
    var bubbleData = {
      name: "Results",
      children: []
    };

    var cols = chartData.data.columns;
    var colors = chartData.data.colors;
    var currentColor = "#000000";

    // Work out how much to multiply the value by to set the size of the bubble.
    var bubbleMultiplier = calcBubbleMultiplier(cols);
    var i = cols.length;
    while (i--) {
      cols[i].forEach(function (item, j) {
        // Find the correct bubble color.
        if (colors[item]) {
          currentColor = colors[item];
        }
        // Set the data.
        if (j == 0 && i != 0) {
          bubbleData.children.push({
            name: item,
            children: [],
            color: currentColor
          });
        } else if (i != 0) {
          var k = bubbleData.children.length - 1;
          bubbleData.children[k].children.push({
            name: cols[0][j],
            value: item,
            size: item * bubbleMultiplier,
            color: currentColor
          });
        }
      });
    }

    return bubbleData;
  }

  /*
   * Function to convert multiple data to bubble friendly nested data.
   * Used for multi table mode only.
   */
  function parseMultiTables(tables) {
    // Set up initial object.
    var bubbleData = [];
    var currentColor = "#000000";
    var colors = ['#bcbd22', '#ffbb78', '#ff9896', '#f7b6d2', '#c7c7c7', '#8c564b'];

    // Loop all the table data to build the new data structure required by D3.
    tables.forEach(function (table, index) {
      var label = table.bubbleLabel;
      var leafAttached = false;
      var notes = table.data.notes;
      var columns = table.data.columns;
      // @todo this need to be rewrite to get the largest number for calculate.
      var bubbleMultiplier = calcBubbleMultiplier(columns);

      if (typeof colors[index] !== 'undefined') {
        currentColor = colors[index];
      }

      // Build the data as children nodes which only have leaves.
      var leafData = buildLeaf(columns, currentColor, bubbleMultiplier, index, notes, label);

      // Add above data to existing bubbles, based on parents.
      for (var i = 0; i < table.parents.length; i++) {
        // Find the parent bubble in existing bubbles and get the path.
        path = findDataParent(bubbleData, table.parents[i], [], -1);

        if (path !== false) {
          // Attach to the existing bubble by the path.
          leafAttached = true;
          addBubbleChild(bubbleData, leafData, path);
          break;
        }
        else {
          // Parent bubble is not found. Create a bubble for parent and attach to it.
          var totalAdd = sumChildren(leafData);

          leafData = [{
            name: table.parents[i],
            children: leafData,
            value: totalAdd.value,
            size: totalAdd.size
          }];
        }
      }

      if (leafAttached === false) {
        // None of parents has been found in existing bubbles, then add a root node.
        bubbleData.push.apply(bubbleData, leafData);
      }
    });

    return bubbleData;
  }

  // Search bubble parent name in existing bubble data tree.
  function findDataParent(bubbleData, name, path)  {
    var pathResult;
    var thisPath;

    if ($.isArray(bubbleData)) {
      // bubbleData may contains more than one root bubble. In this case, it is a Array instead of Object.
      for (var i = 0; i < bubbleData.length; i++) {
        thisPath = path;
        thisPath.push(i);
        pathResult = findDataParent(bubbleData[i], name, thisPath);
        if (pathResult) {
          return pathResult;
        }
      }
    }

    if (bubbleData.name === name) {
      // Find it, return path.
      return path;
    }
    else if (bubbleData.hasOwnProperty('children') && bubbleData.children.length) {
      for (var i = 0; i < bubbleData.children.length; i++) {
        thisPath = path;
        thisPath.push('children');
        thisPath.push(i);
        pathResult = findDataParent(bubbleData.children[i], name, thisPath);
        if (pathResult) {
          return pathResult;
        }
      }
    }

    return false;
  }

  // Add children to parent bubble based on a path.
  // Path is a array like [0, 'children', 0] for bubbleData[0].children[0].
  function addBubbleChild(bubbleData, children, path) {
    var i;
    for (i = 0; i < path.length - 1; i++) {
      bubbleData = bubbleData[path[i]];
    }

    // Add children to parent node.
    if ($.isArray(children)) {
      var totalOrigin = sumChildren(children);
      var totalAdd = sumChildren(bubbleData[path[i]].children);

      bubbleData[path[i]].value = totalOrigin.value + totalAdd.value;
      bubbleData[path[i]].size = totalOrigin.size + totalAdd.size;

      bubbleData[path[i]].children.push.apply(bubbleData[path[i]].children, children);
    }
    else {
      bubbleData[path[i]].children.push(children);
    }
  }

  // Get the total from children.
  function sumChildren(children) {
    var totalValue = 0;
    var totalSize = 0;

    for (var i = 0; i < children.length; i++) {
      totalValue += children[i].value;
      totalSize += children[i].size;
    }

    return { 'value': totalValue, 'size': totalSize };
  }

  // Build bubble nodes with leaves.
  function buildLeaf(columns, currentColor, bubbleMultiplier, tableIndex, notes, label) {
    var leafData = [];

    var i = columns.length;
    while (i--) {
      // Each column will be a node with several leaves.
      columns[i].forEach(function (item, j) {
        var itemName = columns[0][j];
        var columnsLabel = columns[i][0];

        // Set the data.
        if (j == 0 && i != 0) {
          leafData.push({
            name: label ? label : item,
            children: [],
            color: currentColor,
            value: 0,
            size: 0
          });
        } else if (i != 0) {
          var rowId = notes.allRowIds[itemName][columnsLabel];
          var note = notes.datasetRowIds.indexOf(rowId.toString());
          var k = leafData.length - 1;
          leafData[k].children.push({
            name: itemName,
            value: item,
            size: item * bubbleMultiplier,
            color: currentColor,
            _id: rowId,
            note: (note !== -1) ? note : null,
            table: tableIndex
          });

          // Update parent total.
          leafData[k].value += item;
          leafData[k].size = leafData[k].value * bubbleMultiplier;
        }
      });
    }

    return leafData;
  }

  /*
   * Function to calculate how much each value should be multiplied by,
   * to form the size of the bubble.
   */
  function calcBubbleMultiplier(cols) {
    var i = cols.length;
    var colMax = 0;
    var max = 0;
    var maxBubbleSize = 30000;
    // Get the largest value in the dataset.
    while (i--) {
      colMax = Object.keys(cols[i]).reduce(function (m, k) {
        return cols[i][k] > m ? cols[i][k] : m
      }, -Infinity);
      if (colMax != '-Infinity') {
        max = colMax > max ? colMax : max;
      }
    }

    return maxBubbleSize/max;
  }

})(jQuery);
