#include <node.h>
#include "hash_ring.h"

extern "C" void
init (v8::Handle<v8::Object> target) {
    v8::HandleScope scope;
    HashRing::Initialize(target);
}
