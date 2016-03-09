var express = require('express');
var cpphello = require('cpphello');

var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'Testing'});
});

router.post('/', function(req, res, next) {
    var number = req.body.number;
    console.log("input number: ", number);

    res.render('index', { title: 'Testing', sentValue: number, cppOutput: cpphello.foo(number), threadTest: cpphello.threadTest() });
})

module.exports = router;
