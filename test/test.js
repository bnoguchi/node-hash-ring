var vows = require("vows"),
    assert = require("assert"),
    HashRing = require("../lib/hash_ring").HashRing;

var unweightedRuns = 1000,
    unweightedErrorBound = 0.05,
    weightedRuns = 1000,
    weightedErrorBound = 0.05;

// Suites are top level
// Batches are executed sequentially
// Batches contain contexts, which are executed in parallel
// Contexts contain topics and vows (in combination define your tests)
vows.describe("A HashRing").addBatch({
    'locating a node': {
        topic: new HashRing(),
        'should return 0 if less than the first element': function (topic) {
            assert.equal(topic.binarySearch([10, 20, 30], 5), 0);
        },
        'should return 1 if less than the second element': function (topic) {
            assert.equal(topic.binarySearch([10, 20, 30], 15), 1);
        },
        'should return 0 if greater than the last element': function (topic) {
            assert.equal(topic.binarySearch([10, 20, 30], 40), 0);
        }
    },

//    'distribution of nodes': {
//        topic: new HashRing({nodes: [{id: 1}, {id: 2}, {id: 3}], numReplicas: 100}),
//        'should distribute evenly': function (topic) {
//
//        }
//        
//    }


}).export(module);
