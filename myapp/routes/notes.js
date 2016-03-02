var express = require('express');
var router = express.Router();


/* GET notes page. */
router.get('/notes', function(req, res, next) {
  res.render('notes', { title: 'Notes' });
});


module.exports = router;
