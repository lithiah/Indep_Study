$(document).ready(function() {
    generateChart(data);
});

function generateChart(data) {
    //margins & svg canvas size
    var margin = {top: 40, right: 120, bottom: 50, left: 50},
    width = 5000 - margin.left - margin.right,
    height = 500 - margin.top - margin.bottom;

    var x = d3.scale.linear()
        .range([0, width])
        .domain(d3.extent(data, function(d) { return d["x"]; }));

    var y = d3.scale.linear()
        .range([height, 0])
        .domain([1, 10]);

    var color = d3.scale.category20();

    var xAxis = d3.svg.axis()
        .scale(x)
        .orient("bottom")
        .ticks(data.length)
        .tickSize(-height, 0, 0)
        .tickFormat(d3.format( "d" ));

    var yAxis = d3.svg.axis()
        .scale(y)
        .orient("left")
        .ticks(10)
        .tickSize(-width, 0, 0);

    //create svg body
    var svg = d3.select("#viz").append("svg")
        .attr("width", width + margin.left + margin.right)
        .attr("height", height + margin.top + margin.bottom)
    .append("g")
        .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

    //create axes
    svg.append("g")
        .attr("class", "xaxis")
        .attr("transform", "translate(0," + height + ")")
        .call(xAxis)
    .append("text")
        .attr("class", "label")
        .attr("x", width)
        .attr("y", 50)
        .style("text-anchor", "end")
        .text("Years");

    svg.append("g")
        .attr("class", "yaxis")
        .call(yAxis)
    .append("text")
        .attr("class", "label")
        .attr("transform", "rotate(-90)")
        .attr("y", -50)
        .attr("dy", ".71em")
        .style("text-anchor", "end")
        .text("Rank");

    //create circles
    svg.selectAll(".dot")
        .data(data)
    .enter().append("circle")
        .attr("class", "dot")
        .attr("r", 5)
        .attr("cx", function(d) { return x(d["x"]); })
        .attr("cy", function(d) { return y(d["y"]); })
        .style("fill", function(d) { return color(d["cluster"]); })
        .style("stroke", "#008000");
}
