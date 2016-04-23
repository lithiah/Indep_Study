var express = require('express');
var cpphello = require('cpphello');

var router = express.Router();

router.get('/', function(req, res, next) {

    res.render('index', { title: 'Testing', clusterResults: cpphello.clustersInit("testfile.xml", 10, 2, 2, [1, 1])});

})

module.exports = router;
