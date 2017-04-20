#ifndef HASH_RING_H_
#define HASH_RING_H_

#include <node.h>
#include <node_object_wrap.h>
#include <string>
#include <vector>

namespace HashRing
{
typedef int (*compfn)(const void *, const void *);

typedef struct
{
  unsigned int point;
  std::string id;
} Vpoint;

class Ring
{
public:
  int num_points;
  int num_servers;
  std::vector<Vpoint> vpoints;
  Ring() : num_points(0), num_servers(0)
  {
  }

  void setNumServers(int ns)
  {
    num_servers = ns;
    vpoints.resize(ns * 160);
  }

  void setNumPoints(int np)
  {
    num_points = np;
  }

  ~Ring()
  {
  }
};

class NodeInfo
{
public:
  std::string id;
  int weight;
  NodeInfo(const char *idStr, int weight) : id(idStr), weight(weight)
  {
  }
  ~NodeInfo()
  {
  }
};

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
  static void hash_digest(char *in, int len, unsigned char out[16]);
  static unsigned int hash_val(char *in, int len);
  static int vpoint_compare(Vpoint *a, Vpoint *b);
  static v8::Persistent<v8::Function> constructor;
};
}
#endif // HASH_RING_H_
