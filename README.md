## node-hash-ring - Consistent Hashing C++ Add-on for node.js
---

See this [blog post](http://ngchi.wordpress.com/2010/08/23/towards-auto-sharding-in-your-node-js-app/) for more information.

### Installation
Via npm:

    npm install hash_ring

Via git:

    git clone http://github.com/bnoguchi/node-hash-ring.git
    cd node-hash-ring/src
    node-waf configure build

### Example

```javascript
var HashRing = require("./lib/hash_ring");

// Create a cluster of 3 servers weighted so that 127.0.0.2:8080 stores twice as many 
// keys as 127.0.0.1:8080, and 127.0.0.3:8080 stores 4x as many keys as 127.0.0.1:8080
// and 2x as many keys as 127.0.0.2:8080
var ring = new HashRing({"127.0.0.1:8080": 1, "127.0.0.2:8080": 2, "127.0.0.3:8080":4});
console.log(ring.getNode("users:102") ); // Should be 127.0.0.3:8080
```

See [./test/test_distribution.js](https://github.com/bnoguchi/node-hash-ring/test/test_distribution.js) for another example:
    node test/test_distribution.js

### Tests

To run the tests:

    $ make test

### License
MIT License

---
### Author
Brian Noguchi
