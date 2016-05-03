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
        .domain([10, 1]);

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

    //create cluster overlay
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

    //create circles
    var dots = svg.selectAll(".dot")
        .data(data)
    .enter().append("g")
        .attr("class", "dot");

    dots.append("circle")
        .attr("class", "circle")
        .attr("r", 5)
        .attr("cx", function(d) { return x(d["x"]); })
        .attr("cy", function(d) { return y(d["y"]); })
        .style("fill", function(d) { return color(d["cluster"]); })
        .style("stroke", "#008000")
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
        .on("mouseout", function() {
            d3.selectAll(".dottext").remove();
        });

    // var text = svg.selectAll("text")
    //     .data(data)
    // .enter().append("text")
    //     .attr("class", "dottext")
    //     .attr("x", function(d) { return x(d["x"]); })
    //     .attr("y", function(d) { return y(d["y"])-20; })
    //     .text( function (d) { console.log(d["name"], d["cluster"]); return d["name"] + ": " + d["cluster"]; })
    //     .attr("font-family", "sans-serif")
    //     .attr("font-size", "1em")
    //     .attr("fill", "black");
        
}
