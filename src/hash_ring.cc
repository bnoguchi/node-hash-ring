#include <stdlib.h>
#include <stdio.h>
#include <math.h>   // For floorf
#include <string.h> // For strlen
#include "md5.h"
#include "hash_ring.h"
#include <iostream>
using namespace std;

using namespace v8;
using namespace node;

void HashRing::hash_digest(char *in, unsigned char out[16]) {
    md5_state_t md5_state;
    md5_init(&md5_state);
    md5_append(&md5_state, (unsigned char*) in, strlen(in));
    md5_finish(&md5_state, out);
};

unsigned int HashRing::hash_val(char *in) {
    unsigned char digest[16];
    hash_digest(in, digest);
    return (unsigned int) (
        (digest[3] << 24) |
        (digest[2] << 16) |
        (digest[1] << 8) |
         digest[0]
    );
}

int HashRing::vpoint_compare(Vpoint *a, Vpoint *b) {
    return (a->point < b->point) ? -1 : ((a->point > b->point) ? 1 : 0);
}

void HashRing::Initialize(Handle<Object> target) {
    HandleScope scope;

    Local<FunctionTemplate> t = FunctionTemplate::New(New);
    t->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(t, "getNode", GetNode);

    target->Set(String::NewSymbol("HashRing"), t->GetFunction());
}

HashRing::HashRing(Local<Object> weight_hash) : ObjectWrap() {
    Local<Array> node_names = weight_hash->GetPropertyNames();
    Local<String> node_name;
    uint32_t weight, weight_total = 0;
    unsigned int num_servers = node_names->Length();
    
    NodeInfo *node_list = new NodeInfo[num_servers];
    // Construct the server list based on the weight hash
    for (unsigned int i = 0; i < num_servers; i++) {
        NodeInfo *node = &(node_list[i]);
        node_name = node_names->Get(i)->ToString();
        String::Utf8Value utfVal(node_name);
        node->id = new char[utfVal.length()];
        strcpy(node->id, *utfVal);
        node->weight = weight_hash->Get(node_name)->Uint32Value();
        node_list[i] = *node;
        weight_total += node->weight;
    }

    Vpoint *vpoint_list = new Vpoint[num_servers * 160];
    unsigned int j, k;
    int vpoint_idx = 0;
    for (j = 0; j < num_servers; j++) {
        float percent = (float) node_list[j].weight / (float) weight_total;
        unsigned int num_replicas = floorf(percent * 40.0 * (float) num_servers);
        for (k = 0; k < num_replicas; k++) {
            char ss[30];
            sprintf(ss, "%s-%d", node_list[j].id, k);
            unsigned char digest[16];
            hash_digest(ss, digest);
            int m;
            for (m = 0; m < 4; m++) {
                vpoint_list[vpoint_idx].point = (digest[3 + m*4] << 24) |
                                                (digest[2 + m*4] << 16) |
                                                (digest[1 + m*4] << 8) |
                                                 digest[m*4];
                vpoint_list[vpoint_idx].id = node_list[j].id;
                vpoint_idx++;
            }
        }
    }
    delete [] node_list;
    qsort((void*) vpoint_list, vpoint_idx, sizeof(Vpoint), (compfn) vpoint_compare);

    ring.vpoints = vpoint_list;
    ring.num_points = vpoint_idx;
}

HashRing::~HashRing(){
	delete [] ring.vpoints;
}

Handle<Value> HashRing::New(const Arguments &args) {
    HandleScope scope;

    if (args[0]->IsObject()) {
        Local<Object> weight_hash = args[0]->ToObject();
        HashRing *hash_ring = new HashRing(weight_hash);
        hash_ring->Wrap(args.This());
        return args.This();
    } else {
        return ThrowException(Exception::TypeError(String::New("Bad argument")));
    }
}

Handle<Value> HashRing::GetNode(const Arguments &args) {
    HandleScope scope;

    HashRing *hash_ring = ObjectWrap::Unwrap<HashRing>(args.This());
    Ring* ring = &(hash_ring->ring);

    Local<String> str = args[0]->ToString();
    String::Utf8Value utfVal(str);
    char* key = *utfVal;
    unsigned int h = hash_val(key);

    int high = ring->num_points;
    Vpoint *vpoint_arr = ring->vpoints;
    int low = 0, mid;
    unsigned int mid_val, mid_val_1;
    while (true)
    {
        mid = (int) ( (low + high) / 2);
        if (mid == ring->num_points)
            return String::New(vpoint_arr[0].id); // We're at the end. Go to 0
        mid_val = vpoint_arr[mid].point;
        mid_val_1 = mid == 0 ? 0 : vpoint_arr[mid-1].point;
        if (h <= mid_val && h > mid_val_1)
            return String::New(vpoint_arr[mid].id);
        if (mid_val < h)
            low = mid + 1;
        else
            high = mid - 1;
        if (low > high)
            return String::New(vpoint_arr[0].id);
    }
}
