## node-hash-ring - Consistent Hashing C++ Add-on for node.js

* * *

See this [blog post](http://ngchi.wordpress.com/2010/08/23/towards-auto-sharding-in-your-node-js-app/) for more information.

### Installation

Via npm:

```bash
$ npm install hash_ring
```

Via git:

```bash
$ git clone http://github.com/bnoguchi/node-hash-ring.git
$ cd node-hash-ring
$ npm run build
```

### Example

```javascript
var HashRing = require("hash_ring");

// Create a cluster of 3 servers weighted so that 127.0.0.2:8080 stores twice as many 
// keys as 127.0.0.1:8080, and 127.0.0.3:8080 stores 4x as many keys as 127.0.0.1:8080
// and 2x as many keys as 127.0.0.2:8080

var ring = new HashRing({"127.0.0.1:8080": 1, "127.0.0.2:8080": 2, "127.0.0.3:8080":4});
console.log(ring.getNode("users:102") ); // Should be 127.0.0.3:8080
```

See [./test/test_distribution.js](./test/test_distribution.js) for another example:

```bash
$ node test/test_distribution.js
```

### Key Hashing

By default, MD5 is used to hash keys.  You can choose another hasher.

```js
var HashRing = require("hash_ring");

var ring = new HashRing({"127.0.0.1:8080": 1, "127.0.0.2:8080": 2, "127.0.0.3:8080":4}, "murmur");
```

Supported hashers are:

-   `md5` - the default
-   `murmur` - [MurmurHash3](https://sites.google.com/site/murmurhash/)

### Tests

To run the tests:

```bash
$ npm test
```

### License

MIT License

* * *

### Author

Brian Noguchi
