/*!
simple_tree: tree template
Copyright (C) 2012  Mateusz Bysiek, http://mbdev.pl/

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
  */
#pragma once
#ifndef SIMPLE_TREE_H
#define SIMPLE_TREE_H

#ifdef DEBUG
#include <ostream>
#include <iostream>
#endif

#include "simple_container.h"
#include "simple_list.h"

/*!
  \brief Simple tree, that should be first of all easy to inherit from and expand.
  */
template<typename T, typename S>
class simple_tree
{
private:
   typedef simple_tree<T,S> C;
   typedef S L;

private:
   // Single element of the tree, pointing to the root and sub-elements.
   class elem_raw
   {
      ELEM_RAW_BASIC;
   private:
      // Pointer to the root element.
      elem_raw* ptr_root;
      // Pointers to the sub-elements.
      simple_list<elem_raw*,S> subelems_list;
   public:
      ELEM_RAW_DEF_CONSTR, ptr_root(nullptr) { }
      ELEM_RAW_VAL_CONSTR, ptr_root(nullptr) { }
      // Constructs element with specified value and specified root.
      inline elem_raw(const T& value, elem_raw* root)
         : elem_value(value), ptr_root(root) { }
      // Destructor.
      ELEM_RAW_DESTR { ELEM_RAW_DESTR_T ptr_root = nullptr; subelems_list.clear(); }
   public:
      inline elem_raw*& root() { return ptr_root; }
      inline simple_list<elem_raw*,S>& subList() { return subelems_list; }
      inline typename simple_list<elem_raw*,S>::elem sub(S index) { return subelems_list.element(index); }
      inline typename simple_list<elem_raw*,S>::elem findSub(elem_raw*& e)
      { return subelems_list.find(e); }
      inline S subCount() const { return subelems_list.length(); }
#ifdef DEBUG
   public:
      static std::ostream& test(std::ostream& s = std::cout);
#endif // DEBUG
   };

public:
   // Advanced pointer to the element of the tree, const-correct.
   class elem_const
   {
      ELEM_CONST_BASIC;
      ELEM_CONST_TRAVERSE(left,right);
   public:
      inline bool hasRoot() const;
      S subCount() const { return e->subCount(); }
   public:
      // Goes to the root element.
      const elem_const& root() const;
      // Goes to the left.
      const elem_const& left(S count) const;
      // Goes to the left element (previous sibling).
      const elem_const& left() const;
      // Goes to the right element (next sibling).
      const elem_const& right() const;
      // Goes to the right.
      const elem_const& right(S count) const;
      // Goes to the first sub-element.
      const elem_const& sub() const;
      // Goes to the sub-element with given index (n-th child).
      const elem_const& sub(S index) const;
#ifdef DEBUG
   public:
      static std::ostream& test(std::ostream& s = std::cout);
#endif // DEBUG
   };

public:
   // Advanced pointer to the element of the tree. Can be used to modify the tree.
   class elem : public elem_const
   {
      ELEM_BASIC;
      ELEM_TRAVERSE(left,right);
   public:
      // Goes to the root element.
      elem& root();
      // Goes to the left.
      elem& left(S count);
      // Goes to the left element (previous sibling).
      elem& left();
      // Goes to the right element (next sibling).
      elem& right();
      // Goes to the right.
      elem& right(S count);
      // Goes to the first sub-element.
      elem& sub();
      // Goes to the sub-element with given index (n-th child).
      elem& sub(S index);
   public:
      void insertAbove(const T& value);
      void insertLeftmost(const T& value);
      void insertLeft(S count, const T& value);
      void insertLeft(const T& value);
      void insertRight(const T& value);
      void insertRight(S count, const T& value);
      void insertRightmost(const T& value);
      void insertBelow(S index, const T& value);
      void insertFirstSub(const T& value);
      void insertBeforeSub(S index, const T& value);
      void insertAfterSub(S index, const T& value);
      void insertLastSub(const T& value);
      void appendSub(const T& value);
#ifdef DEBUG
   public:
      static std::ostream& test(std::ostream& s = std::cout);
#endif // DEBUG
   };

private:
   // root element of the tree.
   elem root_elem;
   // Number of elements in the tree.
   S elem_count;
public:
   // Creates an empty tree.
   simple_tree();
   // Creates a one element tree, i.e. a tree with root only.
   simple_tree(const T& value);
public:
   // Returns first element of the list.
   elem root() { return root_elem; }
   // Returns first element of the list.
   S count() const { return elem_count; }
protected:
   simple_tree<T,S>& initFrom(const T& value);
public:
   const simple_list<const T*,S> getHorizontalList(S level) const;
   const simple_list<const T*,S> getVerticalList(S subelemIndex) const;
#ifdef DEBUG
public:
   static std::ostream& test(std::ostream& s = std::cout);
#endif // DEBUG
};

//// simple_tree::elem_const

template<typename T, typename S>
bool simple_tree<T,S>::elem_const::hasRoot() const
{
   if(empty())
      return false;
   return (e->root() ? true : false);
}

template<typename T, typename S>
const typename simple_tree<T,S>::elem_const& simple_tree<T,S>::elem_const::left(
      S count) const
{
   if(!count || empty())
      return *this;
   if(!hasRoot())
      return elem_const();
   elem_raw* r = e->root();
   elem_raw*& x = *(elem_raw**)(&e);
   simple_list<C::elem_raw*,S>::elem el = r->findSub(x);
   el.back(count);
   *(C::elem_raw**)(&e) = el.value_ref();
   return *this;
}

template<typename T, typename S>
const typename simple_tree<T,S>::elem_const& simple_tree<T,S>::elem_const::left() const
{
   if(empty())
      return *this;
   if(!hasRoot())
      return elem_const();
   elem_raw* r = e->root();
   elem_raw*& x = *(elem_raw**)(&e);
   simple_list<C::elem_raw*,S>::elem el = r->findSub(x);
   el.back();
   *(C::elem_raw**)(&e) = el.value_ref();
   return *this;
}

template<typename T, typename S>
const typename simple_tree<T,S>::elem_const& simple_tree<T,S>::elem_const::right() const
{
   if(empty())
      return *this;
   if(!hasRoot())
      return elem_const();
   elem_raw* r = e->root();
   elem_raw*& x = *(elem_raw**)(&e);
   simple_list<C::elem_raw*,S>::elem el = r->findSub(x);
   el.forward();
   *(C::elem_raw**)(&e) = el.value_ref();
   // as one operation:
   //*(elem_raw**)(&e) = e->root()->findSub(*(elem_raw**)(&e)).forward().value_ref();
   return *this;
}

template<typename T, typename S>
const typename simple_tree<T,S>::elem_const& simple_tree<T,S>::elem_const::right(
      S count) const
{
   if(!count || empty())
      return *this;
   if(!hasRoot())
   {
      clear();
      return *this;
   }
   elem_raw* r = e->root();
   elem_raw*& x = *(elem_raw**)(&e);
   simple_list<C::elem_raw*,S>::elem el = r->findSub(x);
   el.forward(count);
   *(elem_raw**)(&e) = el.value_ref();
   return *this;
}

//// simple_tree::elem

template<typename T, typename S>
typename simple_tree<T,S>::elem& simple_tree<T,S>::elem::left()
{
   if(empty())
      return *this;
   if(!hasRoot())
   {
      clear();
      return *this;
   }
   elem_raw* r = e->root();
   simple_list<C::elem_raw*,S>::elem this_as_sub = r->findSub(e);
   this_as_sub.back();
   e = this_as_sub.value_ref();
   // as one line command:
   //e = r->subList().find(e).back().value_ref();
   return *this;
}

template<typename T, typename S>
typename simple_tree<T,S>::elem& simple_tree<T,S>::elem::right()
{
   if(empty())
      return *this;
   if(!hasRoot())
   {
      clear();
      return *this;
   }
   elem_raw* r = e->root();
   simple_list<C::elem_raw*,S>::elem this_as_sub = r->findSub(e);
   this_as_sub.forward();
   if(this_as_sub.empty())
   {
      clear();
      return *this;
   }
   e = this_as_sub.value_ref();
   // as one line command:
   //e = r->subList().find(e).back().value_ref();
   return *this;
}

template<typename T, typename S>
typename simple_tree<T,S>::elem& simple_tree<T,S>::elem::sub()
{
   if(empty())
      return *this;
   if(subCount() == 0)
   {
      clear();
      return *this;
   }
   e = e->subList().first().value_ref();
   return *this;
}


//template<typename T, typename S>
//simple_list<simple_tree<T,S>::elem,S> simple_tree<T,S>::elem::subList()
//{
//}

//void insertAbove(const T& value);

template<typename T, typename S>
void simple_tree<T,S>::elem::insertLeftmost(const T& value)
{

}

template<typename T, typename S>
void simple_tree<T,S>::elem::insertLeft(const T& value)
{
   if(!connected())
      return;
   if(empty())
   {
      if(c->elem_count > 0)
         return; // something's wrong, element is empty but the tree is not
   }
   if(!empty() && !hasRoot())
      return; // cannot add siblings to the main root of the tree
   typedef simple_tree<T,S>::elem_raw elem_raw_t;
   elem_raw_t* new_elem = new elem_raw_t(value);
   if(empty())
   {
      e = new_elem;
      c->root_elem = *this;
      c->elem_count = 1;
      return;
   }
   new_elem->root() = e->root();
   simple_list<C::elem_raw*,S>::elem this_as_sub = e->root()->findSub(e);
   this_as_sub.insertPrev(new_elem);
   c->elem_count += 1;
}

template<typename T, typename S>
void simple_tree<T,S>::elem::insertRight(const T& value)
{
   if(!connected())
      return;
   if(empty())
   {
      if(c->elem_count > 0)
         return; // something's wrong, element is empty but the tree is not
   }
   if(!empty() && !hasRoot())
      return; // cannot add siblings to the main root of the tree
   typedef simple_tree<T,S>::elem_raw elem_raw_t;
   elem_raw_t* new_elem = new elem_raw_t(value);
   if(empty())
   {
      e = new_elem;
      c->root_elem = *this;
      c->elem_count = 1;
      return;
   }
   new_elem->root() = e->root();
   simple_list<C::elem_raw*,S>::elem this_as_sub = e->root()->findSub(e);
   this_as_sub.insertNext(new_elem);
   c->elem_count += 1;
}

template<typename T, typename S>
void simple_tree<T,S>::elem::insertRightmost(const T& value)
{

}

template<typename T, typename S>
void simple_tree<T,S>::elem::insertFirstSub(const T& value)
{

}

template<typename T, typename S>
void simple_tree<T,S>::elem::insertBeforeSub(S index, const T& value)
{

}

template<typename T, typename S>
void simple_tree<T,S>::elem::insertAfterSub(S index, const T& value)
{

}

template<typename T, typename S>
void simple_tree<T,S>::elem::insertLastSub(const T& value)
{
   if(!connected() || empty())
      return;
   typedef simple_tree<T,S>::elem_raw elem_raw_t;
   elem_raw_t* new_elem = new elem_raw_t(value, e);
   e->subList().insertLast(new_elem);
   c->elem_count += 1;
}

template<typename T, typename S>
void simple_tree<T,S>::elem::appendSub(const T& value) { insertLastSub(value); }

//// simple_tree

template<typename T, typename S>
simple_tree<T,S>::simple_tree()
   : root_elem(), elem_count(0)
{
   root_elem.connectTo(this);
}

template<typename T, typename S>
simple_tree<T,S>::simple_tree(const T& value)
   : root_elem(), elem_count(0)
{
   initFrom(value);
}

template<typename T, typename S>
simple_tree<T,S>& simple_tree<T,S>::initFrom(const T& value)
{
   //release();
   root_elem = elem();
   root_elem.connectTo(this);
   root_elem.insertRight(value);
   return *this;
}

template<typename T, typename S>
const simple_list<const T*,S> simple_tree<T,S>::getHorizontalList(S level) const
{
   simple_list<const T*,S> elem_ptrs;
   C::elem r(root());

   if(level == 0)
   {
      elem_ptrs.append(&(r.value_ref()));
      return elem_ptrs;
   }

   simple_list<C::elem,S> elems;
   elem s = r.sub();
   S l = 0;
   while(l < level)
   {
      if(elems.length() > 0)
      {

      }
      else
         s = r.sub();
      for(; s.valid(); s.right())
      {
         elems.append(s);
      }
      ++l;
   }
   return elem_ptrs;
}

template<typename T, typename S>
const simple_list<const T*,S> simple_tree<T,S>::getVerticalList(S subelemIndex) const
{
   simple_list<const T*,S> elem_ptrs;
   return elem_ptrs;
}

#ifdef DEBUG

template<typename T, typename S>
std::ostream& simple_tree<T,S>::elem_raw::test(std::ostream& s)
{
   ELEM_RAW_BASIC_TEST;
   return s;
}

template<typename T, typename S>
std::ostream& simple_tree<T,S>::elem_const::test(std::ostream& s)
{
   ELEM_CONST_BASIC_TEST;
   return s;
}

template<typename T, typename S>
std::ostream& simple_tree<T,S>::elem::test(std::ostream& s)
{
   ELEM_BASIC_TEST;
   return s;
}

template<typename T, typename S>
std::ostream& simple_tree<T,S>::test(std::ostream& s)
{
   CONTAINER_BASIC_TEST;
   C tree;
   tree.getHorizontalList(0);
   tree.getVerticalList(0);
   return s;
}

#endif // DEBUG

#endif // SIMPLE_TREE_H
