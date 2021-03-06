$( document ).ready(function() {

    function fib(n) {
      return n < 2 ? 1 : fib(n - 1) + fib(n - 2);
    };

    ////////

    var start = new Date().getTime();

    var p = new Parallel([40, 41, 42, 43, 44, 45, 46]),

    log = function () {
        console.log(arguments);
        var end = new Date().getTime();
        console.log(end-start);
    };

    p.map(fib).then(log);

    ///////

    var start2 = new Date().getTime();

    var noP = [40, 41, 42, 43, 44, 45, 46];

    for (var i=0; i<7; i++) {
        console.log(fib(noP[i]));
    }

    var end2 = new Date().getTime();

    console.log(end2-start2);
});
