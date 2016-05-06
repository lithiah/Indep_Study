//modules I need for the this page 
var express = require('express');
var cpphello = require('cpphello'); //c++ addon
var request = require('request'); //allows you to make POST requests - not necessary for local files but good for future letting people link to their own xml files
var xml2js = require('xml2js'); //parser for the xml file

var router = express.Router();

//constants for calculating clusters
var iterations = 10;
var clustersCount = 20; //cannot exceed number of points in data, max for color mapping to chart is 20

//when the URL is "/", execute this function
router.get('/', function(req, res, next) {
    //make a request to the actorsData in public
    request("http://localhost:3000/actorsData.xml", function (error, response, body) {
        var parseString = xml2js.parseString;
        var xml = body;

        //parse the xml file
        parseString(xml, function (err, result) {
            var Clustersdata = []; //create one array of numbers for c++ addon
            var Vizdata = []; //create objects of data for visualization

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
            var activeDimArray = Array(numOfAttributes + 1).join('1').split('').map(parseFloat); //which dimensions to use in calculations, default here is all of them

            //get cluster assignments from cpphello addon
            var newClusters = cpphello.clustersInitPar4(Clustersdata, iterations, clustersCount, numOfAttributes, activeDimArray);

            //add those cluster assignments to Viz points objects
            for (var i=0; i<newClusters.length; i++) {
                if (newClusters[i]) {
                    Vizdata[i]["cluster"] = newClusters[i];
                }
                else {
                    Vizdata[i]["cluster"] = 0;
                }
            }

            //render the index.jade file with the following passed through variables
            res.render('index', {title: 'Testing', clusterResults: newClusters, vizResults: JSON.stringify(Vizdata)});
        });
    });
});

//when the url is '/timing' execute this function
router.get('/timing', function(req, res, next) {
    //request the long actors data xml file
    request("http://localhost:3000/longActorsData.xml", function (error, response, body) {
        var parseString = xml2js.parseString;
        var xml = body;

        //parse the xml
        parseString(xml, function (err, result) {
            var Clustersdata = []; //we're only timing how long the c++ addon takes to generate clusters

            for (actor in result["actorData"]["actor"]) {
                for (attribute in result["actorData"]["actor"][actor]) {
                    if (attribute != "Name") {
                        Clustersdata.push(parseFloat(result["actorData"]["actor"][actor][attribute][0]));
                    }
                }
            }

            var numOfAttributes = Object.keys(result["actorData"]["actor"][0]).length - 1;
            var activeDimArray = Array(numOfAttributes + 1).join('1').split('').map(parseFloat);

            //first round of 1 iteration of timing
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

            //loop 10 times for each method and calculate average time
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

            //load the timing.jade file with the following passed through values
            res.render('timing', {title: 'Testing', par1Time: time1, par2Time: time3, par4Time: time4, par8Time: time5, seqTime: time2, SeqTimeArray: seqTimeArray, SeqTimeAvg: seqTimeAvg, Par1TimeArray: par1TimeArray, Par1TimeAvg: par1TimeAvg, Par2TimeArray: par2TimeArray, Par2TimeAvg: par2TimeAvg, Par4TimeArray: par4TimeArray, Par4TimeAvg: par4TimeAvg, Par8TimeArray: par8TimeArray, Par8TimeAvg: par8TimeAvg});
        });
    });
})

module.exports = router;
