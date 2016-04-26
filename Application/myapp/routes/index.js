var express = require('express');
var cpphello = require('cpphello');

var router = express.Router();

router.get('/', function(req, res, next) {

    var data = [1, 1, 2, 1, 4, 3, 5, 4];
    var iterations = 10;
    var clusters = 2;
    var attributes = 2;
    var activeDimensions = [1,1];

    clusters = cpphello.clustersInit(data, iterations, clusters, attributes, activeDimensions);

    res.render('index', { title: 'Testing', clusterResults: clusters});

})

module.exports = router;
