#pragma once
#ifndef BINARY_HASH_H
#define BINARY_HASH_H

#include <typeinfo>
#include <cstdlib>

class binary_hash
{
public:
   typedef unsigned long long value_t;
   struct hash_t
   {
      hash_t* zero;
      hash_t* one;
      hash_t()
         : zero(nullptr), one(nullptr) { }
      virtual ~hash_t()
      {
         if(zero)
         {
            delete zero;
            zero = nullptr;
         }
         if(one) {
            delete one;
            one = nullptr;
         }
      }
      virtual bool both_set()
      {
         return zero && one;
      }
   };
   struct hash_end : public hash_t
   {
      bool exists;
      hash_end()
         : hash_t(), exists(false) { }
      hash_end(bool exists)
         : hash_t(), exists(exists) { }
   };
private:
   size_t size;
   value_t leaf_count;
   value_t node_count;
   hash_t* root;
public:
   binary_hash()
      : size(1), leaf_count(0), node_count(0), root(0) { }
   binary_hash(size_t size)
      : size(size-1), leaf_count(0), node_count(0), root(0) { }
   ~binary_hash();
private:
   hash_end** get_leaf(value_t value, bool allocate);
   bool optimize_node(hash_t* h);
public:
   value_t nodes() { return node_count; }
   value_t leaves() { return leaf_count; }
   bool put(value_t value);
   //bool pop(value_t value);
   //bool check(value_t value);
   void optimize();
   bool clear();
};

#endif // BINARY_HASH_H
