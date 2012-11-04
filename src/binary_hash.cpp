#include "binary_hash.h"


binary_hash::~binary_hash()
{
   if(!root)
      return;
   delete root;
}

binary_hash::hash_end** binary_hash::get_leaf(value_t value, bool allocate)
{
   //static type_info hash_t_id = typeid(hash_t);
   //static type_info hash_end_id = typeid(hash_end);

   if(!root)
   {
      if(allocate)
      {
         root = new hash_t;
         ++node_count;
      }
      else
         return nullptr;
   }
   hash_t* h = root;
   for(size_t i = 0; i < size; ++i)
   {
      if(typeid(*h) == typeid(hash_end))
      {
         hash_end*& early_leaf = (hash_end*&)h;
         return &early_leaf;
      }
      hash_t*& next = value & 1 ? h->one : h->zero;
      if(!next)
      {
         if(allocate)
         {
            next = new hash_t;
            ++node_count;
         }
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
      {
         leaf = new hash_end(false);
         ++node_count;
      }
      else
         return nullptr;
   }
   return &leaf;
}

bool binary_hash::optimize_node(hash_t* h/*, hash_t* levels*//*, size_t level*/)
{
   if(!h)
      return false;

   bool zero_opt = false;
   if(h->zero)
   {
      //const char* id = typeid(hash_end).name();
      //const char* id_zero = typeid(*(h->zero)).name();

      if(typeid(*(h->zero)) == typeid(hash_end))
      {
         hash_end*& leaf_zero = (hash_end*&)h->zero;
         if(leaf_zero->exists)
            zero_opt = true;
      }
      else
      {
         if(optimize_node(h->zero/*, levels*//*, level+1*/))
         {
            delete h->zero;
            hash_end*& new_leaf_zero = (hash_end*&)h->zero;
            new_leaf_zero = new hash_end(true);
            zero_opt = true;
         }
      }
   }

   bool one_opt = false;
   if(h->one)
   {
      //const char* id = typeid(hash_end).name();
      //const char* id_one = typeid(*(h->one)).name();

      if(typeid(*(h->one)) == typeid(hash_end))
      {
         hash_end*& leaf_one = (hash_end*&)h->one;
         if(leaf_one->exists)
            one_opt = true;
      }
      else
      {
         if(optimize_node(h->one/*, levels*//*, level+1*/))
         {
            delete h->one;
            hash_end*& new_leaf_one = (hash_end*&)h->one;
            new_leaf_one = new hash_end(true);
            one_opt = true;
         }
      }
   }

   if(zero_opt && one_opt)
   {
      delete h->zero;
      h->zero = nullptr;
      delete h->one;
      h->one = nullptr;
      node_count -= 2;

      return true;
   }
   return false;
}

bool binary_hash::put(value_t value)
{
   hash_end*& leaf = *get_leaf(value, true);
   if(leaf->exists)
      return false;
   leaf->exists = true;
   ++leaf_count;
   return true;
}

//bool binary_hash::pop(value_t value)
//{
//   hash_end** leaf = get_leaf(value, false);
//   if(!leaf)
//      return false;
//   if((*leaf)->exists)
//   {
//      (*leaf)->exists = false;
//      return true;
//   }
//   return false;
//}

//bool binary_hash::check(value_t value)
//{
//   hash_end** leaf = get_leaf(value, false);
//   if(!leaf)
//      return false;
//   if((*leaf)->exists)
//      return true;
//   return false;
//}

void binary_hash::optimize()
{
   //hash_t* levels = new hash_t[size+1];
   optimize_node(root/*, levels*//*, 0*/);
}
