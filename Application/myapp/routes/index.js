var express = require('express');
var cpphello = require('cpphello');

var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'Colors', cppOutput: cpphello.hello() });
});

router.get('/results', function(req, res, next) {
  res.render('results', { title: 'Results', sentValue: 4 });
});

router.post('/addnumber', function(req, res) {

    var number = req.body.number;
    console.log(number);

    res.render('results', { title: "Results", sentValue: number });
});

module.exports = router;
