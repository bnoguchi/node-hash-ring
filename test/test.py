from hash_ring import HashRing

weights = {
    '0.0.0.1': 1,
    '0.0.0.2': 2,
    '0.0.0.3': 3,
    '0.0.0.4': 4,
    '0.0.0.5': 5

}

ring = HashRing(weights.keys(), weights)
for i in xrange(0,100000):
    print i, ring.get_node(str(i))
