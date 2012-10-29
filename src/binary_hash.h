#pragma once
#ifndef BINARY_HASH_H
#define BINARY_HASH_H

#include <queue>

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
      ~hash_t()
      {
         if(zero)
            delete zero;
         if(one)
            delete one;
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
   hash_t* root;
public:
   binary_hash(size_t size)
      : size(size-1), root(0)
   {
      //
   }
   ~binary_hash();
private:
   hash_end** get_leaf(value_t value, bool allocate);
public:
   bool put(value_t value);
   bool pop(value_t value);
   bool check(value_t value);
};

#endif // BINARY_HASH_H
