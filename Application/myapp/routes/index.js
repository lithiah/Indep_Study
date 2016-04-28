var express = require('express');
var cpphello = require('cpphello');
var request = require('request');
var xml2js = require('xml2js');

var router = express.Router();

var data = [1, 1, 2, 1, 4, 3, 5, 4];
var iterations = 10;
var clustersCount = 2;
var attributes = 2;
var activeDimensions = [1,1];

initialClusters = cpphello.clustersInit(data, iterations, clustersCount, attributes, activeDimensions);

function vectorize_XML(xmlName) {

}

router.get('/', function(req, res, next) {
    res.render('index', { title: 'Testing', clusterResults: initialClusters});

});

router.post('/', function (req, res) {
    request("http://localhost:3000/" + req.body.xmlName, function (error, response, body) {
        var parseString = xml2js.parseString;
        var xml = body;

        parseString(xml, function (err, result) {
            var Clustersdata = [];
            for (actor in result["actorData"]["actor"]) {
                for (attribute in result["actorData"]["actor"][actor]) {
                    if (attribute != "Name") {
                        Clustersdata.push(parseFloat(result["actorData"]["actor"][actor][attribute][0]));
                    }
                }
            }

            var numOfAttributes = Object.keys(result["actorData"]["actor"][0]).length - 1;
            var activeDimArray = Array(attributes).join('1').split('').map(parseFloat);

            console.log("iterations: ", iterations);
            console.log("clusters: ", clustersCount);
            console.log("numOfAttributes: " , numOfAttributes);
            console.log("activeDimArray: ", activeDimArray);

            newClusters = cpphello.clustersInit(Clustersdata, iterations, clustersCount, numOfAttributes, activeDimArray);

            res.render('index', {title: 'Testing', clusterResults: newClusters});
        });
    });
});

module.exports = router;








            //result["actorData"]["actor"]
            // { Name: [ 'Williams, E.' ],
            // Count: [ '2' ],
            // Degree: [ '13' ],
            // MedianYear: [ '1949.5' ],
            // MedianRank: [ '8' ],
            // MeanYear: [ '1949.5' ],
            // MeanRank: [ '8' ],
            // DegreeYear: [ '6.5' ] },
