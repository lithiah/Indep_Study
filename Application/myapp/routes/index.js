var express = require('express');
var cpphello = require('cpphello');

var router = express.Router();

router.get('/', function(req, res, next) {

    clusters = cpphello.clustersInit(/*all data points data as 1 vector*/[1, 1, 2, 1, 4, 3, 5, 4], /*number of iterations*/10, /*number of clusters*/2, /*number of attributes*/2, /*active dimensions*/[1, 1]);

    console.log("Cluster Assignment Results: ", clusters);
    console.log(typeof(clusters));
    var i = 0;
    for (thing in clusters) {
        console.log(i, ": ", clusters[i]);
        i++;
    }

    res.render('index', { title: 'Testing', clusterResults: clusters});

})

module.exports = router;
