var func = function() {
    var value = 0;

    return {
        increment: function(inc) {
            value += typeof inc === 'number' ? inc : 1;
        },

        getValue: function() {
            return value;
        }
    };
};

y = func();
y.increment();
y.increment();
y.increment();
y.increment();
y.increment();
y.increment();

console.log(y.getValue());
