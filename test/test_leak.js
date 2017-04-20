"use strict";

const assert = require("assert");
const HashRing = require("../index");

const nodes = {
  "127.0.0.1:8080": 1,
  "127.0.0.2:8080": 1,
  "127.0.0.3:8080": 1,
  "127.0.0.4:8080": 1,
  "127.0.0.5:8080": 1
};

console.log("Running test leak - be patient, it will take 1-2 minutes depending on your machine speed\n");

const memUseBefore = process.memoryUsage();
console.log("memory usage before", memUseBefore);

const beginTime = Date.now();

for (var i = 0; i < 500000; i++) {
  const ring = new HashRing(nodes, "murmur");
  ring.getNode('a' + i + 'b' + i);
}

const endTime = Date.now();
const elapsedTime = endTime - beginTime;

console.log("Time", (elapsedTime / 1000) + "s");

global.gc();

setTimeout(() => {
  const memUseAfter = process.memoryUsage();
  console.log("memory usage after", memUseAfter);

  const changed = memUseAfter.heapUsed - memUseBefore.heapUsed;
  console.log("heapUsed change", changed + " bytes; ", Math.ceil(changed / 1000) + "KB");
  assert(changed < 100000, "heapUsed change larger than 100K");

}, 100);
