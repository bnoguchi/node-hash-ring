var HashRing = require("../index");

var crypto = require("crypto");

var nodes = {
  "127.0.0.1:8080": 1,
  "127.0.0.2:8080": 1,
  "127.0.0.3:8080": 1
};

var ring = new HashRing(nodes);

const memUseBefore = process.memoryUsage();
console.log("memory usage before", memUseBefore);

var time = (new Date()).getTime();
for (var i = 0; i < 10000000; i++) {
  ring.getNode('a' + i + 'b' + i);
}

console.log('Time: ' + ((new Date()).getTime() - time) / 100 + ' s');

global.gc();

setTimeout(() => {
  const memUseAfter = process.memoryUsage();
  console.log("memory usage after", memUseAfter);

  console.log("heapUsed change", memUseAfter.heapUsed - memUseBefore.heapUsed);
}, 100);
