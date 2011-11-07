try {
  module.exports = require("./build/default/hash_ring").HashRing;
}
catch(e) {
  // update for v0.5.5+
  module.exports = require("./build/Release/hash_ring").HashRing;
}
