var express = require('express');
var cpphello = require('cpphello');
var request = require('request');
var xml2js = require('xml2js');

var router = express.Router();

var iterations = 10;
var clustersCount = 20; //cannot exceed number of points in data, max for color mapping to chart is 20

router.get('/', function(req, res, next) {
    request("http://localhost:3000/actorsData.xml", function (error, response, body) {
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

            var newClusters = cpphello.clustersInitPar4(Clustersdata, iterations, clustersCount, numOfAttributes, activeDimArray);

            for (var i=0; i<newClusters.length; i++) {
                if (newClusters[i]) {
                    Vizdata[i]["cluster"] = newClusters[i];
                }
                else {
                    Vizdata[i]["cluster"] = 0;
                }
            }

            res.render('index', {title: 'Testing', clusterResults: newClusters, vizResults: JSON.stringify(Vizdata)});
        });
    });
});

router.get('/timing', function(req, res, next) {
    request("http://localhost:3000/longActorsData.xml", function (error, response, body) {
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
            var activeDimArray = Array(numOfAttributes + 1).join('1').split('').map(parseFloat);

            var start1 = new Date().getTime();
            var newClusters = cpphello.clustersInitPar1(Clustersdata, iterations, clustersCount, numOfAttributes, activeDimArray);
            var time1 = new Date().getTime() - start1;

            var start2 = new Date().getTime();
            var newClusters2 = cpphello.clustersInitSeq(Clustersdata, iterations, clustersCount, numOfAttributes, activeDimArray);
            var time2 = new Date().getTime() - start2;

            var start3 = new Date().getTime();
            var newClusters3 = cpphello.clustersInitPar2(Clustersdata, iterations, clustersCount, numOfAttributes, activeDimArray);
            var time3 = new Date().getTime() - start3;

            var start4 = new Date().getTime();
            var newClusters4 = cpphello.clustersInitPar4(Clustersdata, iterations, clustersCount, numOfAttributes, activeDimArray);
            var time4 = new Date().getTime() - start4;

            var start5 = new Date().getTime();
            var newClusters5 = cpphello.clustersInitPar8(Clustersdata, iterations, clustersCount, numOfAttributes, activeDimArray);
            var time5 = new Date().getTime() - start5;

            var seqTimeArray = [];
            var seqTimeAvg = 0;
            var par1TimeArray = [];
            var par1TimeAvg = 0;
            var par2TimeArray = [];
            var par2TimeAvg = 0;
            var par4TimeArray = [];
            var par4TimeAvg = 0;
            var par8TimeArray = [];
            var par8TimeAvg = 0;

            for (var i = 0; i < 10; i++) {
                var start = new Date().getTime();
                var newClusters = cpphello.clustersInitSeq(Clustersdata, iterations, clustersCount, numOfAttributes, activeDimArray);
                var time = new Date().getTime() - start;
                seqTimeArray.push(time);
                seqTimeAvg += time;
            }
            seqTimeAvg = seqTimeAvg/10;

            for (var i = 0; i < 10; i++) {
                var start = new Date().getTime();
                var newClusters = cpphello.clustersInitPar1(Clustersdata, iterations, clustersCount, numOfAttributes, activeDimArray);
                var time = new Date().getTime() - start;
                par1TimeArray.push(time);
                par1TimeAvg += time;
            }
            par1TimeAvg = par1TimeAvg/10;

            for (var i = 0; i < 10; i++) {
                var start = new Date().getTime();
                var newClusters = cpphello.clustersInitPar2(Clustersdata, iterations, clustersCount, numOfAttributes, activeDimArray);
                var time = new Date().getTime() - start;
                par2TimeArray.push(time);
                par2TimeAvg += time;
            }
            par2TimeAvg = par2TimeAvg/10;

            for (var i = 0; i < 10; i++) {
                var start = new Date().getTime();
                var newClusters = cpphello.clustersInitPar4(Clustersdata, iterations, clustersCount, numOfAttributes, activeDimArray);
                var time = new Date().getTime() - start;
                par4TimeArray.push(time);
                par4TimeAvg += time;
            }
            par4TimeAvg = par4TimeAvg/10;

            for (var i = 0; i < 10; i++) {
                var start = new Date().getTime();
                var newClusters = cpphello.clustersInitPar8(Clustersdata, iterations, clustersCount, numOfAttributes, activeDimArray);
                var time = new Date().getTime() - start;
                par8TimeArray.push(time);
                par8TimeAvg += time;
            }
            par8TimeAvg = par8TimeAvg/10;

            res.render('timing', {title: 'Testing', par1Time: time1, par2Time: time3, par4Time: time4, par8Time: time5, seqTime: time2, SeqTimeArray: seqTimeArray, SeqTimeAvg: seqTimeAvg, Par1TimeArray: par1TimeArray, Par1TimeAvg: par1TimeAvg, Par2TimeArray: par2TimeArray, Par2TimeAvg: par2TimeAvg, Par4TimeArray: par4TimeArray, Par4TimeAvg: par4TimeAvg, Par8TimeArray: par8TimeArray, Par8TimeAvg: par8TimeAvg});
        });
    });
})

module.exports = router;
