#ifndef HASH_RING_H_
#define HASH_RING_H_

#include <node.h>
#include <node_object_wrap.h>

namespace HashRing
{
typedef int (*compfn)(const void *, const void *);

typedef struct
{
  unsigned int point;
  char *id;
} Vpoint;

typedef struct
{
  int num_points;
  Vpoint *vpoints;
} Ring;

typedef struct
{
  char *id;
  int weight;
} NodeInfo;

class HashRing : public node::ObjectWrap
{

  Ring ring;

public:
  HashRing(v8::Local<v8::Object> weight_hash);
  ~HashRing();

  static void Initialize(v8::Local<v8::Object> exports);

private:
  static void New(const v8::FunctionCallbackInfo<v8::Value> &args);
  static void GetNode(const v8::FunctionCallbackInfo<v8::Value> &args);
  //
  static void hash_digest(char *in, unsigned char out[16]);
  static unsigned int hash_val(char *in);
  static int vpoint_compare(Vpoint *a, Vpoint *b);
  static v8::Persistent<v8::Function> constructor;
};
}
#endif // HASH_RING_H_
