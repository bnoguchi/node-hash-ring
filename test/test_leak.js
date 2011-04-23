var sys = require("sys");

var HashRing = require("../index");

var nodes = {
    "127.0.0.1:8080": 1,
    "127.0.0.2:8080": 1,
    "127.0.0.3:8080": 1
};

var time = (new Date()).getTime();
for(var i=0; i<1000000; i++){
	var ring = new HashRing(nodes);
	ring.getNode('a'+i+'b'+i);
}
console.log('Time: '+((new Date()).getTime() - time)/100+' s');

