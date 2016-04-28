$(document).ready(function() {
    var data = [
        {
            "name": "Zoey",
            "cluster": 0,
            "vector": [1936, 1] //x is year, y is rank
        },
        {
            "name": "Poe",
            "cluster": 0,
            "vector": [1923, 3]
        },
        {
            "name": "Dipper",
            "cluster": 1,
            "vector": [1954, 9]
        },
        {
            "name": "Quinton",
            "cluster": 1,
            "vector": [1962, 7]
        }
    ]

    generateChart(data);
});

function generateChart(data) {
    //margins & svg canvas size
    var margin = {top: 40, right: 120, bottom: 50, left: 50},
    width = 860 - margin.left - margin.right,
    height = 500 - margin.top - margin.bottom;

    var x = d3.scale.linear()
        .range([0, width])
        .domain(d3.extent(data, function(d) { return d["vector"][0]; }));

    var y = d3.scale.linear()
        .range([height, 0])
        .domain([1, 10]);

    var color = d3.scale.category10();

    var xAxis = d3.svg.axis()
        .scale(x)
        .orient("bottom")
        .ticks(data.length)
        .tickSize(-height, 0, 0);

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
        .attr("class", "x axis")
        .attr("transform", "translate(0," + height + ")")
        .call(xAxis)
    .append("text")
        .attr("class", "label")
        .attr("x", width)
        .attr("y", 50)
        .style("text-anchor", "end")
        .text("Years");

    svg.append("g")
        .attr("class", "y axis")
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
        .attr("cx", function(d) { return x(d["vector"][0]); })
        .attr("cy", function(d) { return y(d["vector"][1]); })
        .style("fill", "green")
        .style("stroke", "#008000");
}
