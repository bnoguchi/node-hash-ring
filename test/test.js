var HashRing = require("../index");
var ring = new HashRing({
    '0.0.0.1' : 1,
    '0.0.0.2' : 2,
    '0.0.0.3' : 3,
    '0.0.0.4' : 4,
    '0.0.0.5' : 5
});


for (var i=0; i < 100000; i++){
    console.log(i + ' ' + ring.getNode(i));
}