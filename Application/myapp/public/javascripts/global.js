$(document).ready(function() {

    var svg = d3.select("#svgContainer").append("svg")
        .attr("width", 550)
        .attr("height", 456)
        .style("background-color", "green");


    $("#svgContainer").click(function() {

        var newColor = get_random_color();

        svg.style("background-color", newColor);

        console.log("new color: " , newColor);
    });

});

function get_random_color() {
  function c() {
    var hex = Math.floor(Math.random()*256).toString(16);
    return ("0"+String(hex)).substr(-2); // pad with zero
  }
  return "#"+c()+c()+c();
}
