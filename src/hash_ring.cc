#include <stdlib.h>
#include <stdio.h>
#include <math.h> // For floorf
#include "md5.h"
#include "hash_ring.h"
#include <iostream>
#include <vector>

namespace HashRing
{

using namespace std;
using namespace v8;
using namespace node;

void HashRing::hash_digest(char *in, int len, unsigned char out[16])
{
  md5_state_t md5_state;
  md5_init(&md5_state);
  md5_append(&md5_state, (unsigned char *)in, len);
  md5_finish(&md5_state, out);
};

unsigned int HashRing::hash_val(char *in, int len)
{
  unsigned char digest[16];
  hash_digest(in, len, digest);
  return (unsigned int)((digest[3] << 24) |
                        (digest[2] << 16) |
                        (digest[1] << 8) |
                        digest[0]);
}

int HashRing::vpoint_compare(Vpoint *a, Vpoint *b)
{
  return (a->point < b->point) ? -1 : ((a->point > b->point) ? 1 : 0);
}

Persistent<Function> HashRing::constructor;

HashRing::HashRing(Local<Object> weight_hash) : ObjectWrap()
{
  Local<Array> node_names = weight_hash->GetPropertyNames();
  Local<String> node_name;
  uint32_t weight_total = 0;
  size_t num_servers = node_names->Length();

  vector<NodeInfo> node_list;
  node_list.reserve(num_servers);
  size_t max_id_len = 0;
  // Construct the server list based on the weight hash
  for (size_t i = 0; i < num_servers; i++)
  {
    node_name = node_names->Get(i)->ToString();
    String::Utf8Value utfVal(node_name);
    size_t id_len = utfVal.length();
    if (id_len > max_id_len)
    {
      max_id_len = id_len;
    }
    NodeInfo node(*utfVal, weight_hash->Get(node_name)->Uint32Value());
    node_list.push_back(node);
    weight_total += node.weight;
  }

  ring.setNumServers(num_servers);
  vector<Vpoint> &vpoints = ring.vpoints;
  size_t j, k;
  int vpoint_idx = 0;
  max_id_len += 50;
  for (j = 0; j < num_servers; j++)
  {
    float percent = (float)node_list[j].weight / (float)weight_total;
    size_t num_replicas = floorf(percent * 40.0 * (float)num_servers);
    for (k = 0; k < num_replicas; k++)
    {
      char ss[max_id_len];
      int ss_len = sprintf(ss, "%s-%d", node_list[j].id.c_str(), (int)k);
      unsigned char digest[16];
      hash_digest(ss, ss_len, digest);
      int m;
      for (m = 0; m < 4; m++)
      {
        vpoints[vpoint_idx].point = (digest[3 + m * 4] << 24) |
                                    (digest[2 + m * 4] << 16) |
                                    (digest[1 + m * 4] << 8) |
                                    digest[m * 4];
        vpoints[vpoint_idx].id = node_list[j].id;
        vpoint_idx++;
      }
    }
  }
  qsort((void *)&vpoints[0], vpoint_idx, sizeof(Vpoint), (compfn)vpoint_compare);

  ring.setNumPoints(vpoint_idx);
}

HashRing::~HashRing()
{
}

void HashRing::Initialize(Local<Object> exports)
{
  Isolate *isolate = exports->GetIsolate();

  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "HashRing"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_PROTOTYPE_METHOD(tpl, "getNode", GetNode);

  constructor.Reset(isolate, tpl->GetFunction());

  exports->Set(String::NewFromUtf8(isolate, "HashRing"),
               tpl->GetFunction());
}

void HashRing::New(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();

  if (!args.IsConstructCall())
  {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Please call new HashRing(nodes)")));
  }
  else if (args[0]->IsObject())
  {
    Local<Object> weight_hash = args[0]->ToObject();
    HashRing *hash_ring = new HashRing(weight_hash);
    hash_ring->Wrap(args.Holder());
    args.GetReturnValue().Set(args.Holder());
  }
  else
  {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "HashRing Bad argument")));
  }
}

void HashRing::GetNode(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();

  HashRing *hash_ring = ObjectWrap::Unwrap<HashRing>(args.Holder());
  Ring &ring = hash_ring->ring;

  Local<String> str = args[0]->ToString();
  String::Utf8Value utfVal(str);
  char *key = *utfVal;
  size_t h = hash_val(key, utfVal.length());

  int high = ring.num_points;
  const vector<Vpoint> &vpoints = ring.vpoints;
  int low = 0, mid;
  size_t mid_val, mid_val_1;
  while (true)
  {
    mid = (int)((low + high) / 2);
    if (mid == ring.num_points)
    {
      args.GetReturnValue().Set(String::NewFromUtf8(isolate, vpoints[0].id.c_str())); // We're at the end. Go to 0
      return;
    }
    mid_val = vpoints[mid].point;
    mid_val_1 = mid == 0 ? 0 : vpoints[mid - 1].point;
    if (h <= mid_val && h > mid_val_1)
    {
      args.GetReturnValue().Set(String::NewFromUtf8(isolate, vpoints[mid].id.c_str()));
      return;
    }
    if (mid_val < h)
    {
      low = mid + 1;
    }
    else
    {
      high = mid - 1;
    }

    if (low > high)
    {
      args.GetReturnValue().Set(String::NewFromUtf8(isolate, vpoints[0].id.c_str()));
      return;
    }
  }
}
}
