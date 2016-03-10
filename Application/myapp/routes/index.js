var express = require('express');
var cpphello = require('cpphello');

var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'Testing'});
});

router.post('/', function(req, res, next) {
    var number = parseInt(req.body.number);
    console.log("input number: ", number);

    res.render('index', { title: 'Testing', sentValue: number, cppOutput: cpphello.timesTwo(number), threadTest: cpphello.threadTest(), threadSum: cpphello.threadSum(number) });
})

module.exports = router;
