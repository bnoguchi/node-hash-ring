#include <node.h>
#include "hash_ring.h"

namespace HashRing
{

using v8::Local;
using v8::Object;

void InitAll(Local<Object> exports)
{
  HashRing::Initialize(exports);
}

NODE_MODULE(hash_ring, InitAll)
}
