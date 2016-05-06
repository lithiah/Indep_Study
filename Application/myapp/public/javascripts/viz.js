$(document).ready(function() {
    //on page load, generate the chart with data passed from index.jade
    generateChart(data);
});

function generateChart(data) {
    //margins & svg canvas size
    var margin = {top: 40, right: 120, bottom: 50, left: 50},
    width = 5000 - margin.left - margin.right,
    height = 500 - margin.top - margin.bottom;

    //use a linear scale for the x-axis with a range equal to width of chart, and domain equal to min and max dates
    var x = d3.scale.linear()
        .range([0, width])
        .domain(d3.extent(data, function(d) { return d["x"]; }));

    //create a d3 axis object for the x-axis using the x scale
    var xAxis = d3.svg.axis()
        .scale(x)
        .orient("bottom")
        .ticks(data.length)
        .tickSize(-height, 0, 0)
        .tickFormat(d3.format( "d" ));

    //use a linear scale for y-axis with a range equal to height of chart and domain of 1-10
    var y = d3.scale.linear()
        .range([height, 0])
        .domain([10, 1]);

    //create a d3 axis object for y-axis using the y scale
    var yAxis = d3.svg.axis()
        .scale(y)
        .orient("left")
        .ticks(10)
        .tickSize(-width, 0, 0);

    //use this d3-provided color scale for coloring the clusters
    var color = d3.scale.category20();

    //create svg body with and attach to #viz div in index.jade
    var svg = d3.select("#viz").append("svg")
        .attr("width", width + margin.left + margin.right)
        .attr("height", height + margin.top + margin.bottom)
    .append("g")
        .attr("transform", "translate(" + margin.left + "," + margin.top + ")");
    
    //append x and y axes with labels to the svg
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

    //create cluster overlay (the slightly transparent overlay)
    svg.selectAll(".cluster")
        .data(data)
    .enter().append("circle")
        .attr("class", "cluster")
        .attr("r", 50)
        .attr("cx", function(d) { return x(d["x"]); })
        .attr("cy", function(d) { return y(d["y"]); })
        .style("fill", function(d) { return color(d["cluster"]); })
        .style("opacity", .05)
        .style("stroke-width", 0);

    //create a group "g" for each data point and append to svg
    var dots = svg.selectAll(".dot")
        .data(data)
    .enter().append("g")
        .attr("class", "dot");

    //for each group "g" with its associated data, make a circle 
    dots.append("circle")
        .attr("class", "circle")
        .attr("r", 5)
        .attr("cx", function(d) { return x(d["x"]); }) //interpolate x coordinate based on d3 x linear scale
        .attr("cy", function(d) { return y(d["y"]); }) //interpolate y coordinate based on d3 y linear scale
        .style("fill", function(d) { return color(d["cluster"]); }) //assign color based on d3 color scale 
        .style("stroke", "#008000")
        //when you hover over a circle, show the name and cluster 
        .on("mouseover", function(d) {
            svg.append("text")
                .attr("class", "dottext")
                .attr("x", d.cx)
                .attr("y", d.cy)
                .text( function () { return d.name + ": " + d.cluster; })
                .attr("font-family", "sans-serif")
                .attr("font-size", "1em")
                .attr("fill", "black")
                .attr("transform", function() {
                    var ycor = parseFloat(y(d["y"])) - 20;
                    return "translate(" + x(d["x"]) + "," + ycor.toString() + ")";
                });
        })
        //when you stop hovering over circle, remove the text 
        .on("mouseout", function() {
            d3.selectAll(".dottext").remove();
        });    
}
