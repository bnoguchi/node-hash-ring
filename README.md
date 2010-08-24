## node-hash-ring - Consistent Hashing C++ Add-on for node.js
---

### Installation
    git clone http://bnoguchi@github.com/bnoguchi/node-hash-ring.git
    cd node-hash-ring/src
    node-waf configure build

### Example
    var sys = require("sys"),
        HashRing = require("./lib/hash_ring");
    // Create a cluster of 3 servers weighted so that 127.0.0.2:8080 stores twice as many 
    // keys as either 127.0.0.1:8080, and 127.0.0.3:8080 stores 4x as many keys as 
    // 127.0.0.1:8080 and 2x as many keys as 127.0.0.2:8080
    var ring = new HashRing({"127.0.0.1:8080": 1, "127.0.0.2:8080": 2, "127.0.0.3:8080":4});
    sys.log(ring.getNode("users:102") ); // Should be 127.0.0.3:8080

### License
MIT License

---
### Author
Brian Noguchi
