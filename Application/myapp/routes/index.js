var express = require('express');
var cpphello = require('cpphello');
var request = require('request');
var xml2js = require('xml2js');

var router = express.Router();

var iterations = 10;
var clustersCount = 20; //cannot exceed number of points in data

router.get('/', function(req, res, next) {
    res.render('index', { title: 'Testing', clusterResults: []});

});

router.post('/', function (req, res) {
    request("http://localhost:3000/" + req.body.xmlName, function (error, response, body) {
        var parseString = xml2js.parseString;
        var xml = body;

        parseString(xml, function (err, result) {
            var Clustersdata = [];
            var Vizdata = [];

            for (actor in result["actorData"]["actor"]) {
                for (attribute in result["actorData"]["actor"][actor]) {
                    if (attribute != "Name") {
                        Clustersdata.push(parseFloat(result["actorData"]["actor"][actor][attribute][0]));
                    }
                }
                var nextActor = {};
                nextActor["name"] = result["actorData"]["actor"][actor]["Name"][0];
                nextActor["x"] = parseFloat(result["actorData"]["actor"][actor]["MedianYear"][0]);
                nextActor["y"] = parseFloat(result["actorData"]["actor"][actor]["MedianRank"][0]);
                Vizdata.push(nextActor);
            }

            var numOfAttributes = Object.keys(result["actorData"]["actor"][0]).length - 1;
            var activeDimArray = Array(numOfAttributes + 1).join('1').split('').map(parseFloat);
            
            newClusters = cpphello.clustersInit(Clustersdata, iterations, clustersCount, numOfAttributes, activeDimArray);

            for (var i=0; i<newClusters.length; i++) {
                if (newClusters[i]) {
                    Vizdata[i]["cluster"] = newClusters[i];
                }
            }

            console.log(Vizdata);

            res.render('index', {title: 'Testing', clusterResults: newClusters, vizResults: JSON.stringify(Vizdata)});
        });
    });
});

module.exports = router;
