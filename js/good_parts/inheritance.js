var HTMLGen = function(spec, my) {
    var that = {};

    var name = spec.name || 'HTMLGen test';
    var tag  = spec.tag  || 'h1';

    my.name = name;
    my.tag  = tag;

    var set_name = function(n) {
        name = n;
    }

    var get_name = function() {
        return name;
    }

    var print = function() {
        return '<' + tag + '>' + name + '<' + tag + '/>';
    }

    that.set_name = set_name;
    that.get_name = get_name;
    that.print    = print;

    return that;
}

var FancyHTML = function(spec, my) {

    var fancy = spec.fancy || 'Hoya!';

    my.fancy = fancy;

    var that    = HTMLGen(spec, my);

    var print = function() { 
        return 'fancy!';
    }
    
    that.print = print;

    return that;
}

module.exports.HTMLGen   = HTMLGen;
module.exports.FancyHTML = FancyHTML;
