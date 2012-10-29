#include "binary_hash.h"


binary_hash::~binary_hash()
{
   if(!root)
      return;
   delete root;
//   std::queue<hash_t*> q;
//   q.push(root);
//   for(hash_t* h = q.front(); !q.empty(); q.pop(), h = q.front())
//   {
//      if(h->zero)
//         q.push(h->zero);
//      if(h->one)
//         q.push(h->one);
//      if(!h)
//         h = h;
//      delete h;
//   }
}

binary_hash::hash_end** binary_hash::get_leaf(value_t value, bool allocate)
{
   if(!root)
   {
      if(allocate)
         root = new hash_t;
      else
         return nullptr;
   }
   hash_t* h = root;
   for(size_t i = 0; i < size; ++i)
   {
      hash_t*& next = value & 1 ? h->one : h->zero;
      if(!next)
      {
         if(allocate)
            next = new hash_t;
         else
            return nullptr;
      }
      h = next;
      value >>= 1;
   }
   hash_end*& leaf = (hash_end*&)(value & 1 ? h->one : h->zero);
   if(!leaf)
   {
      if(allocate)
         leaf = new hash_end(false);
      else
         return nullptr;
   }
   return &leaf;
}

bool binary_hash::put(value_t value)
{
//   if(!root)
//      root = new hash_t;
//   hash_t* h = root;
//   for(size_t i = 0; i < size; ++i)
//   {
//      hash_t*& next = value & 1 ? h->one : h->zero;
//      if(!next)
//         next = new hash_t;
//      h = next;
//      value >>= 1;
//   }
//   hash_end*& leaf = (hash_end*&)(value & 1 ? h->one : h->zero);
   hash_end*& leaf = *get_leaf(value, true);
   //if(leaf)
   //{
      if(leaf->exists)
         return false;
      leaf->exists = true;
      return true;
   //}
   //leaf = new hash_end(true);
   //return true;
}

bool binary_hash::pop(value_t value)
{
   hash_end** leaf = get_leaf(value, false);
   if(!leaf)
      return false;
   if((*leaf)->exists)
   {
      (*leaf)->exists = false;
      return true;
   }
   return false;
}

bool binary_hash::check(value_t value)
{
   hash_end** leaf = get_leaf(value, false);
   if(!leaf)
      return false;
   if((*leaf)->exists)
      return true;
   return false;
//   hash_t* h = root;
//   if(!h)
//      return false;
//   for(size_t i = 0; i < size; ++i)
//   {
//      if(value & 1)
//      {
//         h = h->one;
//      }
//      else
//      {
//         h = h->zero;
//      }
//      if(!h)
//         return false;
//      value >>= 1;
//   }
//   return false;
}
