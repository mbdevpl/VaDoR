/*!
simple_list: list template
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
#ifndef SIMPLE_LIST_H
#define SIMPLE_LIST_H

#ifndef SIMPLE_LIST_NOSTRINGS
#include <string>
#include <ostream>
#include <sstream>
#endif // SIMPLE_LIST_NOSTRINGS

#include <stdexcept>

#ifdef DEBUG
#include <typeinfo> // test()
#include <string>
#include <ostream>
#include <iostream>
#include <sstream>
#endif // DEBUG

#include "simple_container.h"

/*!
  \brief Simple list, that should be first of all easy to inherit from and expand.

  Features are quite standard for vectors/lists from STL:
  1) modifiable length
  2) ability to insert elements in the middle without physically moving around in memory
     any other element that is already in the list
  3) class elem (contained inside the class simple_list) allows STL iterator-like operations
  4) overloaded index operator, ++ prefix operator, etc.
  5) underlying bi-directional list built of plain, ordinary pointers: allows easy expansion
  8) modifications: adding/removing arbitrary element by index
  9) 'elem_const' iterator usable for const list references, like const_iterator

  Features outside of the scope of regular STL containers:
  1) not only data type is a part of template, but also length - i.e. maximum length of
     list can be adjusted, and if a kind of type without maximum value is used, the list can
     have unlimited length
  2) you can insert/remove elements directly from 'elem' class: it's an iterator that can not
     only iterate and change values of elements in the list, but can be used to modify
     the structure of the list in the meantime (insert & remove elements)
  3) integrity: when list length changes due to the use of 'elem' class features, the length stored
     in the class is updated, also in case of removing or adding first/last element.
  4) built-in std::ostream opearator<< and str() method that returns string. Both of these can be
     disabled via #define SIMPLE_LIST_NOSTRINGS

  Planned features:
  - sorting (insert, heap, etc.)
  - easy converting to/from STL containers
  - parallel version (for OpenCL) of the template, stored in GPU memory instead of RAM
  - moving data between lists without copying data, but just with pointers rearrangement

  Examples of 'for' loops involving simple_list:
  \code
  int arr[] = {10,20,30,40,50};
  simple_list<int,short> list(5, arr);

   for(simple_list<int,short>::elem e = list.first(); e.valid(); e.next())
      ; // just iteration here ...
   for(simple_list<int,short>::elem e = list.first(); !e.empty(); ++e)
      ; // this loop is equivalent to the previous one
   for(simple_list<int,short>::elem e = list.first(); e; ++e)
      ; // this loop is equivalent to both previous loops

  const simple_list<int,short> list_const(5, arr);

   for(simple_list<int,short>::elem_const e = list_const.first(); e; ++e)
      ; // iterating even if the list is const (and this is safe!)
  \endcode

  */
template<typename T, typename L>
class simple_list
{
private:
   typedef simple_list<T,L> C;

private:
   // Single element of the list, pointing to the next and previous elements.
   class elem_raw
   {
      ELEM_RAW_BASIC
      ;
   private:
      //T elem_value;
      // Pointer to the next element.
      elem_raw* ptr_next;
      // Pointer to the previous element.
      elem_raw* ptr_prev;
   public:
      ELEM_RAW_DEF_CONSTR, ptr_next(0), ptr_prev(0) { }
      ELEM_RAW_VAL_CONSTR, ptr_next(0), ptr_prev(0) { }
      // Constructs element with specified value and specified next & previous elements.
      inline elem_raw(const T& value, elem_raw* next, elem_raw* prev)
         : elem_value(value), ptr_next(next), ptr_prev(prev) { }
   public:
      ELEM_RAW_DESTR { ELEM_RAW_DESTR_T ptr_prev = ptr_next = 0; }
   public:
      // Gets the reference to the next element.
      inline elem_raw*& next() { return this->ptr_next; }
      // Gets the reference to previous element.
      inline elem_raw*& prev() { return this->ptr_prev; }
#ifdef DEBUG
   public:
      static std::ostream& test(std::ostream& s = std::cout);
#endif
   };

public:
   // Advanced pointer to the element of the list, const-correct.
   class elem_const
   {
      ELEM_CONST_BASIC
      ;
      ELEM_CONST_TRAVERSE(back,forward)
      ;
   public:
      // Moves back by the specified ammount.
      const elem_const& back(L count) const;
      // Goes to the previous element.
      const elem_const& back() const;
      // Goes to the next element.
      const elem_const& forward() const;
      // Moves forward by the specified ammount.
      const elem_const& forward(L count) const;
#ifdef DEBUG
   public:
      static std::ostream& test(std::ostream& s = std::cout);
#endif
   };

public:
   // Advanced pointer to the element of the list. Can be used to modify the list.
   class elem : public elem_const
   {
      ELEM_BASIC
      ;
      ELEM_TRAVERSE(back,forward)
      ;
   public:
      // Goes to the next element.
      elem& forward();
      // Moves forward by the specified ammount.
      elem& forward(L count);
      // Goes to the previous element.
      elem& back();
      // Moves back by the specified ammount.
      elem& back(L count);
      // Swaps two elements of the list by moving pointers, not data.
      elem& swapWith(elem& element);
   private:
      inline void delete_and_clear();
   public:
      /*!
        \brief Inserts new element after this element.

        If this element is empty, it does nothing unless the element
        is connected to a list which is empty.

        If this element is not empty, and is connected to any list, the
        the changes in the list are handled by this method:
         - length of list increases
         - if element is inserted after the last one, the last_elem moves

        \param value value of the inserted element
        */
      void insertNext(const T& value);
      /*!
        \brief Inserts a new element before this element.

        If this element is empty, it does nothing unless the element
        is connected to a list which is empty.

        If this element is not empty, and is connected to any list, the
        the changes in the list are handled by this method:
         - length of list increases
         - if element is inserted before the first one, the first_elem moves

        \param value value of the inserted element
        */
      void insertPrev(const T& value);
      void removeAndBack();
      void removeBefore();
      /*!
        \brief Releases the memory reserved for this element.

        Releases the memory, and if element is connected (by default it is)
        this method removes the element from the list.
        */
      void remove();
      void removeAfter();
      void removeAndForward();
   public:
      static inline void clear(elem& element) { element.clear(); }
#ifdef DEBUG
   public:
      static std::ostream& test(std::ostream& s = std::cout);
#endif
   };

private:
   // First element of the list.
   elem first_elem;
   // Last element of the list.
   elem last_elem;
   // Length of the list.
   L list_length;
public:
   // Creates an empty list.
   simple_list();
   // Creates a list from another simple list.
   simple_list(const simple_list<T,L>& list)
      : first_elem(), last_elem(), list_length(0) { initFrom(list); }
   // Creates a one element list.
   simple_list(const T& value)
      : first_elem(), last_elem(), list_length(0) { initFrom(value); }
   // Creates a list from array of elements.
   simple_list(L length, const T array[]);
   // Destructor.
   ~simple_list() { release(); }
public:
   // Returns first element of the list.
   elem first() { return first_elem; }
   const elem_const first() const { return (elem_const)first_elem; }
   // Returns arbitrary element of the list.
   elem element(L index);
   const elem_const element(L index) const;
   // Returns last element of the list.
   elem last() { return last_elem; }
   const elem_const last() const { return (elem_const)last_elem; }
   inline L length() const { return list_length; }
public:
   simple_list<T,L>& initFrom(const T& value);
   simple_list<T,L>& initFrom(const simple_list<T,L>& list);
private:
   void release();
public:
   simple_list<T,L>& append(const T& value);
   /*!
     Appends a list to the end of this list.
     \param list to be appended to the end
     */
   simple_list<T,L>& append(const simple_list<T,L>& list);
   simple_list<T,L>& insertFirst(const T& value);
   simple_list<T,L>& insertBefore(L index, const T& value);
   simple_list<T,L>& insertAfter(L index, const T& value);
   simple_list<T,L>& insertLast(const T& value);
   simple_list<T,L>& insertLast(const simple_list<T,L>& list);
   simple_list<T,L>& removeFirst();
   simple_list<T,L>& remove(L index);
   simple_list<T,L>& removeLast();
   simple_list<T,L>& clear();
   elem find(const T& value);
private:
   void smoothsort_form_heaps(size_t* heaps, L heaps_len, L curr_heap_index/*, bool ignore_left = false*/);
   void smoothsort_check_left_heap(size_t* heaps, L heaps_len, L curr_heap_index);
public:
   void sort();
   void sort_smoothsort();
   void sort_bubblesort();
public:
   const T& operator[](const L& index) const;
   T& operator[](const L& index);
   inline simple_list<T,L>& operator=(const simple_list<T,L>& list) { return initFrom(list); }
   // Concatenates two lists.
   inline simple_list<T,L>& operator+=(const simple_list<T,L>& list) { return append(list); }
   bool operator==(const simple_list<T,L>& list) const;
public:
#ifndef SIMPLE_LIST_NOSTRINGS
   std::string str() const;
   friend std::ostream& operator <<(std::ostream& os, const simple_list<T,L>& list)
   {
      os << list.str();
      return os;
   }
#endif // SIMPLE_LIST_NOSTRINGS
#ifdef DEBUG
public:
   static std::ostream& test_iteration(std::ostream& s = std::cout);
   static std::ostream& test(std::ostream& s = std::cout);
#endif // DEBUG
};

/*!
  \brief A simpler list, which assumes that the length can be as large as possible.

  It is exactly the same as: \code simple_list<T, unsigned long long> \endcode
  */
template<typename T>
class simpler_list : public simple_list<T, unsigned long long>
{
public:
   simpler_list()
      : simple_list<T, unsigned long long>() { }
   simpler_list(const simpler_list<T>& list)
      : simple_list<T, unsigned long long>(list) { }
   simpler_list(const T& value)
      : simple_list<T, unsigned long long>(value) { }
   simpler_list(unsigned long long length, const T array[])
      : simple_list<T, unsigned long long>(length, array) { }
};

/*!
  \brief The simplest list. Used to store list of pointers to any objects.

  It is exactly the same as: \code simple_list<void*, unsigned long long> \endcode
  */
class simplest_list : public simpler_list<void*>
{
public:
   simplest_list()
      : simpler_list() { }
   simplest_list(const simplest_list& list)
      : simpler_list(list) { }
   simplest_list(void* value)
      : simpler_list(value) { }
   simplest_list(unsigned long long length, void* array[])
      : simpler_list(length, array) { }
};

//template<typename T, typename L>
//typename simple_list<T,L>::elem_raw& simple_list<T,L>::elem_raw::operator=(
//      const elem_raw& e)
//{
//   elem_value = e.elem_value;
//   ptr_next = e.ptr_next;
//   ptr_prev = e.ptr_prev;
//   return *this;
//}

template<typename T, typename L>
const typename simple_list<T,L>::elem_const& simple_list<T,L>::elem_const::forward() const
{
   if(empty())
      return *this;
   *(elem_raw**)(&e) = e->next();
   return *this;
}

template<typename T, typename L>
const typename simple_list<T,L>::elem_const& simple_list<T,L>::elem_const::forward(
      L count) const
{
   if(!count || empty())
      return *this;
   for(;count > 0; --count)
      *(elem_raw**)(&e) = e->next();
   return *this;
}

template<typename T, typename L>
const typename simple_list<T,L>::elem_const& simple_list<T,L>::elem_const::back() const
{
   if(empty())
      return *this;
   *(elem_raw**)(&e) = e->prev();
   return *this;
}

template<typename T, typename L>
const typename simple_list<T,L>::elem_const& simple_list<T,L>::elem_const::back(
      L count) const
{
   if(!count || empty())
      return *this;
   for(;count > 0; --count)
      *(elem_raw**)(&e) = e->prev();
   return *this;
}

//template<typename T, typename L>
//const typename simple_list<T,L>::elem_const& simple_list<T,L>::elem_const::operator=(
//      const elem_const& element_const) const
//{
//   *(simple_list<T,L>**)(&list) = element_const.list;
//   *(elem_raw**)(&e) = element_const.e;
//   return *this;
//}

template<typename T, typename L>
typename simple_list<T,L>::elem& simple_list<T,L>::elem::forward()
{
   if(this->empty())
      return *this;
   this->e = this->e->next();
   return *this;
}

template<typename T, typename L>
typename simple_list<T,L>::elem& simple_list<T,L>::elem::forward(L count)
{
   if(!count || this->empty())
      return *this;
   for(;count > 0; --count)
      this->e = this->e->next();
   return *this;
}

template<typename T, typename L>
typename simple_list<T,L>::elem& simple_list<T,L>::elem::back()
{
   if(this->empty())
      return *this;
   this->e = this->e->prev();
   return *this;
}

template<typename T, typename L>
typename simple_list<T,L>::elem& simple_list<T,L>::elem::back(L count)
{
   if(!count || this->empty())
      return *this;
   for(;count > 0; --count)
      this->e = this->e->prev();
   return *this;
}


template<typename T, typename L>
typename simple_list<T,L>::elem& simple_list<T,L>::elem::swapWith(typename simple_list<T,L>::elem& element)
{
   if(this->empty() || !this->connected())
      return *this;
   if(element.empty() || !element.connected() || this->c != element.c)
   {
      clear();
      return *this;
   }
   //C::elem_raw* this_e = e;
   C::elem_raw* element_e = (C::elem_raw*)element;
   if(this->e == element_e)
      return *this;

   C::elem_raw* temp;

   temp = this->e->next();
   if(element_e->next() != this->e)
      this->e->next() = element_e->next();
   else
      this->e->next() = element_e;
   if(temp != element_e)
      element_e->next() = temp;
   else
      element_e->next() = this->e;

   temp = this->e->prev();
   if(element_e->prev() != this->e)
      this->e->prev() = element_e->prev();
   else
      this->e->prev() = element_e;
   if(temp != element_e)
      element_e->prev() = temp;
   else
      element_e->prev() = this->e;

   if(this->e->prev())
      this->e->prev()->next() = this->e;
   if(this->e->next())
      this->e->next()->prev() = this->e;

   if(element_e->prev())
      element_e->prev()->next() = element_e;
   if(element_e->next())
      element_e->next()->prev() = element_e;

   if(this->e == (C::elem_raw*)this->c->first_elem)
      this->c->first_elem = element;
   else if(element_e == (C::elem_raw*)this->c->first_elem)
      this->c->first_elem = *this;

   if(this->e == (C::elem_raw*)this->c->last_elem)
      this->c->last_elem = element;
   else if(element_e == (C::elem_raw*)this->c->last_elem)
      this->c->last_elem = *this;

   return *this;
}

//template<typename T, typename L>
//typename simple_list<T,L>::elem& simple_list<T,L>::elem::operator=(const elem& e_ptr)
//{
//   elem_const::operator =(e_ptr);
//   return *this;
//}

template<typename T, typename L>
void simple_list<T,L>::elem::delete_and_clear()
{
   if(this->empty())
      return;
   // organize connected elements
   if(this->e->prev())
      this->e->prev()->next() = this->e->next();
   if(this->e->next())
      this->e->next()->prev() = this->e->prev();
   // release memory
   delete this->e;
   clear();
}

template<typename T, typename L>
void simple_list<T,L>::elem::insertNext(const T& value)
{
   if(this->empty())
   {
      if(this->connected() && this->c->list_length == 0)
      {
         this->e = new elem_raw(value);
         this->c->last_elem = this->c->first_elem = *this;
         this->c->list_length = 1;
      }
      return;
   }
   elem_raw* new_elem = new elem_raw(value);
   new_elem->prev() = this->e;
   if(this->e->next()) {
      new_elem->next() = this->e->next();
      this->e->next()->prev() = new_elem;
   }
   this->e->next() = new_elem;
   if(this->connected())
   {
      L& len = this->c->list_length;
      ++len;
      if(len == 1)
         throw std::runtime_error("something went wrong: empty list has a non empty element");
      if(this->e == this->c->last_elem.e)
         this->c->last_elem.forward();
      return;
   }
}

template<typename T, typename L>
void simple_list<T,L>::elem::insertPrev(const T& value)
{
   if(this->empty())
   {
      if(this->connected() && this->c->list_length == 0)
      {
         this->e = new elem_raw(value);
         this->c->last_elem = this->c->first_elem = *this;
         this->c->list_length = 1;
      }
      return;
   }
   elem_raw* new_elem = new elem_raw(value);
   new_elem->next() = this->e;
   if(this->e->prev()) {
      new_elem->prev() = this->e->prev();
      this->e->prev()->next() = new_elem;
   }
   this->e->prev() = new_elem;
   if(this->connected())
   {
      L& len = this->c->list_length;
      ++len;
      if(len == 1)
         throw std::runtime_error("something went wrong: empty list has a non empty element");
      if(this->e == this->c->first_elem.e)
         this->c->first_elem.back();
      return;
   }
}

template<typename T, typename L>
void simple_list<T,L>::elem::removeAndBack()
{
   elem e(*this);
   back();
   e.remove();
}

template<typename T, typename L>
void simple_list<T,L>::elem::removeBefore()
{
   elem e(*this);
   e.back();
   e.remove();
}

template<typename T, typename L>
void simple_list<T,L>::elem::remove()
{
   if(this->empty())
      return;
   if(this->connected())
   {
      L& len = this->c->list_length;
      --len;
      if(!len)
      {
         // zero elements left => this is the only elem in the list
         this->c->first_elem.clear();
         this->c->last_elem.clear();
      }
      else if(this->e == this->c->first_elem.e)
         this->c->first_elem.forward();
      else if(this->e == this->c->last_elem.e)
         this->c->last_elem.back();
   }
   delete_and_clear();
}

template<typename T, typename L>
void simple_list<T,L>::elem::removeAfter()
{
   elem e(*this);
   e.forward();
   e.remove();
}

template<typename T, typename L>
void simple_list<T,L>::elem::removeAndForward()
{
   elem e(*this);
   forward();
   e.remove();
}

template<typename T, typename L>
simple_list<T,L>::simple_list()
   : first_elem(), last_elem(), list_length(0)
{
   first_elem.connectTo(this);
   last_elem.connectTo(this);
}

template<typename T, typename L>
simple_list<T,L>::simple_list(L length, const T array[])
   : first_elem(), last_elem(), list_length(0)
{
   if(!length)
   {
      first_elem.connectTo(this);
      last_elem.connectTo(this);
      return;
   }
   for(L i = 0; i < length; ++i)
      append(array[i]);
}

template<typename T, typename L>
typename simple_list<T,L>::elem simple_list<T,L>::element(L index)
{
   if(index < 0 || index >= list_length)
      return elem();
   L diff = list_length - 1 - index;
   if(diff >= index)
      return first()+=index;
   return last()-=diff;
}

template<typename T, typename L>
const typename simple_list<T,L>::elem_const simple_list<T,L>::element(L index) const
{
   if(index < 0 || index >= list_length)
      return elem();
   L diff = list_length - 1 - index;
   if(diff >= index)
      return first()+=index;
   return last()-=diff;
}

template<typename T, typename L>
simple_list<T,L>& simple_list<T,L>::initFrom(const T& value)
{
   release();
   last_elem = first_elem = elem();
   first_elem.connectTo(this);
   last_elem.connectTo(this);
   first_elem.insertNext(value);
   return *this;
}

template<typename T, typename L>
simple_list<T,L>& simple_list<T,L>::initFrom(const simple_list<T,L>& list)
{
   release();
   return append(list);
}

template<typename T, typename L>
void simple_list<T,L>::release()
{
   if(!first_elem.empty())
   {
      elem_raw* raw = (elem_raw*)first_elem;
      elem_raw* temp = raw;
      while(temp)
      {
         raw = raw->next();
         delete temp;
         temp = raw;
      }
   }
   elem::clear(first_elem);
   elem::clear(last_elem);
   list_length = 0;
}

template<typename T, typename L>
simple_list<T,L>& simple_list<T,L>::append(const T& value)
{
   if(!list_length)
      return initFrom(value);
   last().insertNext(value);
   return *this;
}

template<typename T, typename L>
simple_list<T,L>& simple_list<T,L>::append(const simple_list<T,L>& list)
{
   L len = list.length();
   if(!len)
      return *this;
   for(L i = 0; i < len; ++i)
      append(list[i]);
   return *this;
}

template<typename T, typename L>
simple_list<T,L>& simple_list<T,L>::insertFirst(const T& value)
{
   if(!list_length)
      return initFrom(value);
   first().insertPrev(value);
   return *this;
}

template<typename T, typename L>
simple_list<T,L>& simple_list<T,L>::insertBefore(L index, const T& value)
{
   if(!list_length)
      return initFrom(value);
   element(index).insertPrev(value);
   return *this;
}

template<typename T, typename L>
simple_list<T,L>& simple_list<T,L>::insertAfter(L index, const T& value)
{
   if(!list_length)
      return initFrom(value);
   element(index).insertNext(value);
   return *this;
}

template<typename T, typename L>
simple_list<T,L>& simple_list<T,L>::insertLast(const T& value)
{
   return append(value);
}

template<typename T, typename L>
simple_list<T,L>& simple_list<T,L>::insertLast(const simple_list<T,L>& list)
{
   return append(list);
}

template<typename T, typename L>
simple_list<T,L>& simple_list<T,L>::removeFirst()
{
   if(!list_length)
      return *this;
   first().remove();
   return *this;
}

template<typename T, typename L>
simple_list<T,L>& simple_list<T,L>::remove(L index)
{
   if(!list_length)
      return *this;
   element(index).remove();
   return *this;
}

template<typename T, typename L>
simple_list<T,L>& simple_list<T,L>::removeLast()
{
   if(!list_length)
      return *this;
   last().remove();
   return *this;
}

template<typename T, typename L>
simple_list<T,L>& simple_list<T,L>::clear()
{
   release();
   return *this;
}

template<typename T, typename L>
typename simple_list<T,L>::elem simple_list<T,L>::find(const T& value)
{
   for(simple_list<T,L>::elem e = first(); e; ++e)
      if(e.value_ref_const() == value)
         return e;
   return elem();
}

template<typename T, typename L>
void simple_list<T,L>::smoothsort_check_left_heap(size_t* heaps, L heaps_len, L curr_heap_index)
{
   if(heaps_len == 0 || curr_heap_index == 0)
      return;

   // Leonardo numbers
   static const size_t LP[]
         = { 1, 1, 3, 5, 9, 15, 25, 41, 67, 109,
             177, 287, 465, 753, 1219, 1973, 3193, 5167, 8361, 13529, 21891,
             35421, 57313, 92735, 150049, 242785, 392835, 635621, 1028457,
             1664079, 2692537, 4356617, 7049155, 11405773, 18454929, 29860703,
             48315633, 78176337, 126491971, 204668309, 331160281, 535828591,
             866988873 // the next number is > 31 bits.
           };
   L left_heap_index = curr_heap_index + 1 - LP[heaps[curr_heap_index]];

   if(left_heap_index == 0)
      return;

   --left_heap_index;

   elem curr = element(curr_heap_index);
   elem left = element(left_heap_index);

   bool l_c = *left > *curr;
   if(!l_c)
      return;
   bool do_swap = false;

   if(heaps[curr_heap_index] < 2)
      do_swap = true;
   else
   {
      //here, there are subnodes to check!!!
      L prev_heap_index = curr_heap_index - 1;
      elem prev = element(prev_heap_index);
      bool l_p = *left > *prev;
      if(!l_p)
         return;

      L prevprev_heap_index = prev_heap_index - LP[heaps[prev_heap_index]];
      elem prevprev = element(prevprev_heap_index);
      bool l_pp = *left > *prevprev;
      if(!l_pp)
         return;
      do_swap = true;
   }

   if(!do_swap)
      return;

   T temp = *curr;
   *curr = *left;
   *left = temp;
#ifdef DEBUG
   std::cout << "swap left<->curr!" << std::endl;
#endif // DEBUG
   smoothsort_check_left_heap(heaps, heaps_len, left_heap_index);
   //smoothsort_form_heaps(heaps, heaps_len, left_heap_index);
}

template<typename T, typename L>
void simple_list<T,L>::smoothsort_form_heaps(size_t* heaps, L heaps_len, L curr_heap_index)
{
   if(heaps_len == 0 || curr_heap_index == 0)
      return;

   // Leonardo numbers
   static const size_t LP[]
         = { 1, 1, 3, 5, 9, 15, 25, 41, 67, 109,
             177, 287, 465, 753, 1219, 1973, 3193, 5167, 8361, 13529, 21891,
             35421, 57313, 92735, 150049, 242785, 392835, 635621, 1028457,
             1664079, 2692537, 4356617, 7049155, 11405773, 18454929, 29860703,
             48315633, 78176337, 126491971, 204668309, 331160281, 535828591,
             866988873 // the next number is > 31 bits.
           };

   if(heaps[curr_heap_index] < 2)
      return;

   L prev_heap_index = curr_heap_index - 1;
   L prevprev_heap_index = prev_heap_index - LP[heaps[prev_heap_index]];

   elem curr = element(curr_heap_index);
   elem prev = element(prev_heap_index);
   elem prevprev = element(prevprev_heap_index);

   bool p_c = *prev > *curr;
   bool pp_p = *prevprev > *prev;
   bool pp_c = pp_p && p_c ? true : *prevprev > *curr;

   if(p_c && !pp_p)
   {
      T temp = *curr;
      *curr = *prev;
      *prev = temp;
#ifdef DEBUG
      std::cout << "swap prev<->curr!" << std::endl;
#endif // DEBUG
      smoothsort_form_heaps(heaps, heaps_len, prev_heap_index);
   }
   else if(pp_c && pp_p)
   {
      T temp = *curr;
      *curr = *prevprev;
      *prevprev = temp;
#ifdef DEBUG
      std::cout << "swap prevprev<->curr!" << std::endl;
#endif // DEBUG
      smoothsort_form_heaps(heaps, heaps_len, prevprev_heap_index);
   }
}

template<typename T, typename L>
void simple_list<T,L>::sort()
{
   sort_smoothsort();
}

template<typename T, typename L>
void simple_list<T,L>::sort_smoothsort()
{
#ifdef DEBUG
   std::cout << "list to be smooth-sorted: ";
   std::cout << str() << std::endl;
#endif // DEBUG
   if(length() < 2)
      return;

   // Leonardo numbers
   static const size_t LP[]
         = { 1, 1, 3, 5, 9, 15, 25, 41, 67, 109,
             177, 287, 465, 753, 1219, 1973, 3193, 5167, 8361, 13529, 21891,
             35421, 57313, 92735, 150049, 242785, 392835, 635621, 1028457,
             1664079, 2692537, 4356617, 7049155, 11405773, 18454929, 29860703,
             48315633, 78176337, 126491971, 204668309, 331160281, 535828591,
             866988873 // the next number is > 31 bits.
           };

   // heaps : for example { 1, 0, 2, 1, 3 }
   //  indices denote number of the element of the sorted simple_list.
   //  value n means that that element is a root of a tree of a size LP[n]
   //  in the example, 3rd element of the list is a root of heap of size LP[2] = 3,
   //   therefore has two precedeeing elements as its sub-heaps.
   //  the last (5th) element is a root of heap of size LP[3] = 5, therefore
   //   it is the main root of all heaps
   size_t* heaps = new size_t[length()];
   L heaps_len = 0;

   elem prev = first();
   elem prevprev = first();

   //this loop creates heaps for the whole list
   for(elem curr = first(); curr; ++curr)
   {
      // place the current element in the tree of heaps:

      // where is the most recent heap
      L prev_heap_index = heaps_len > 0 ? heaps_len-1 : 0;
      size_t prev_heap = heaps_len > 0 ? heaps[prev_heap_index] : 0;

      // where is the previous heap
      L prevprev_heap_index = 0;
      if(heaps_len > LP[prev_heap])
         prevprev_heap_index = heaps_len-1-LP[prev_heap];
      else if(heaps_len > 1)
         prevprev_heap_index = heaps_len-2;
      size_t prevprev_heap = heaps_len > 1 ? heaps[prevprev_heap_index] : 0;

      // create a heap depending on sizes of the last two heaps
      if(heaps_len >= 2 && prev_heap + 1 == prevprev_heap)
         heaps[heaps_len] = prev_heap + 2;
      else if(heaps_len >= 1 && prev_heap == 1)
         heaps[heaps_len] = 0;
      else if(heaps_len >= 0)
         heaps[heaps_len] = 1;

      // set current heap variable
      L curr_heap_index = heaps_len;
      size_t curr_heap = heaps[curr_heap_index];

      // increase length of list of heaps
      ++heaps_len;

#ifdef DEBUG
      std::cout << "heaps: { ";
      for(size_t i = 0; i < heaps_len; ++i)
         std::cout << heaps[i] << ' ';
      std::cout << '}' << std::endl;
#endif // DEBUG

      //maintain heap property across all heaps affected by
      // recent operation
      if(heaps_len > 2)
      {
         smoothsort_check_left_heap(heaps, heaps_len, curr_heap_index);
         if(curr_heap > 1)
            smoothsort_form_heaps(heaps, heaps_len, curr_heap_index);
      }
#ifdef DEBUG
      std::cout << "current: " << str() << std::endl;
      std::cout /*<< "next elem...\n"*/ << std::endl;
#endif // DEBUG
   }
#ifdef DEBUG
   std::cout << "finished building heaps" << std::endl;
   std::cout << "correct: "
             << "{ 1, 2, 9, 4, 11, 6, 3, 8, 15, 10, "
             << "5, 12, 13, 14, 28, 16, 17, 18, 19, 20, "
             << "21, 22, 23, 24, 32, 26, 7, 27, 29, 30, "
             << "31, 25, 33, 35, 34 }" << std::endl;
#endif // DEBUG

#ifdef DEBUG
#endif // DEBUG
}

template<typename T, typename L>
void simple_list<T,L>::sort_bubblesort()
{
   if(list_length < 2)
      return;
   bool swapped;
   L i_max, j_max = list_length - 1;
   for(L j = 0; j < j_max; ++j)
   {
      swapped = false;
      i_max = list_length - j;
      for(L i = 1; i < i_max; ++i)
      {
         elem e1 = element(i-1);
         elem e2 = element(i);
         if(*e1 > *e2)
         {
            e1.swapWith(e2);
            swapped = true;
         }
      }
      if(!swapped)
         break;
   }
}

template<typename T, typename L>
const T& simple_list<T,L>::operator[](const L& index) const
{
   return element(index).value_ref_const();
}

template<typename T, typename L>
T& simple_list<T,L>::operator[](const L& index)
{
   return element(index).value_ref();
}

template<typename T, typename L>
bool simple_list<T,L>::operator==(const simple_list<T,L>& list) const
{
   if(list.length() != length())
      return false;
   for(C::elem_const e1 = first(), e2 = list.first(); e1; ++e1, ++e2)
   {
      if(*e1 == *e2)
         continue;
      return false;
   }
   return true;
}

#ifndef SIMPLE_LIST_NOSTRINGS

template<typename T, typename L>
std::string simple_list<T,L>::str() const
{
   std::stringstream s;
   s << '{';
   for(simple_list<T,L>::elem_const e = first(); e; ++e)
   {
      s << ' ';
      s << e.value_ref_const();
      if(e != last())
         s << ',';
   }
   s << " }";
   return s.str();
}

#endif // SIMPLE_LIST_NOSTRINGS

#ifdef DEBUG

template<typename T, typename L>
std::ostream& simple_list<T,L>::elem_raw::test(std::ostream& s)
{
   ELEM_RAW_BASIC_TEST;
   elem_raw e1;
   elem_raw e2(0);
   elem_raw e3(T(), &e1, &e2);
   elem_raw e4(e3);

   e1.next() = &e4;
   e1.prev() = &e4;

   return s;
}

template<typename T, typename L>
std::ostream& simple_list<T,L>::elem_const::test(std::ostream& s)
{
   ELEM_CONST_BASIC_TEST;
   elem_const a;
   a.forward();
   a.forward(2);
   a.back();
   a.back(2);

   return s;
}

template<typename T, typename L>
std::ostream& simple_list<T,L>::elem::test(std::ostream& s)
{
   ELEM_BASIC_TEST;
   elem f;
   f.forward();
   f.forward(2);
   f.back();
   f.back(2);
   f.insertNext(0);
   f.insertPrev(0);

   f.remove();
   f.removeBefore();
   f.removeAfter();
   elem::clear(f);
   f.delete_and_clear();

   return s;
}

template<typename T, typename L>
std::ostream& simple_list<T,L>::test_iteration(std::ostream& s)
{
   T a = T();
   T arr[] = {a, ++a, ++a, ++a, ++a};
   simple_list<T,L> list(5, arr);

   for(simple_list<T,L>::elem e = list.first(); e.valid(); e.forward())
      s << " " << e.value();
   s << "\n";
   for(simple_list<T,L>::elem e = list.last(); !e.empty(); --e)
      s << " " << e.value();
   s << "\n";
   for(simple_list<T,L>::elem e = list.first(); e; ++e)
      s << " " << e.value();
   s << "\n";
   for(simple_list<T,L>::elem_const e = list.first(); e; ++e)
      s << " " << *e;
   s << "\n";
   for(simple_list<T,L>::elem_const e = list.last(); e; --e)
      s << " " << *e;
   s << "\n";

   return s;
}

template<typename T, typename L>
std::ostream& simple_list<T,L>::test(std::ostream& s)
{
   std::string type_name(typeid(T).name());
   std::string length_name(typeid(T).name());
   int size_first = sizeof(elem) * 8;
   int size_last = sizeof(elem) * 8;
   int size_len = sizeof(L) * 8;
   int size = size_first + size_last + size_len;
   s << "type info for simple_list<" << type_name << "," << length_name << ">\n";
   CONTAINER_BASIC_TEST;
   s << " list:";
   s << " size = " << size;
   s << " (size_first = " << size_first
     << " size_last = " << size_last
     << " size_len = " << size_len << ")";
   s << "\n";

   test_iteration(s);

   T test_arr[] = {+1,  2, 32, 35, 15,  6, 28,  3,  9, 10,
                   +5, 12, 13, 14, 11, 16, 17, 18, 19, 20,
                   21, 22, 23, 24,  8, 26, 27,  7, 29, 30,
                   31, 25, 33,  4, 34};
   simple_list<T,L> test_lst(35, test_arr);
   test_lst.sort();

   T a = T();
   T arr[] = {a, ++a, ++a, ++a, ++a};
   simple_list<T,L> list(5, arr);
   const simple_list<T,L> list_c(5, arr);
   L len = list.length();
   list.first();
   list.element(L());
   list.element(--len);
   list.last();

   list.append(list_c);
   list.append(T());
   list.clear();
   list.initFrom(list_c);
   list.initFrom(T());
   list.insertAfter(L(), T());
   list.insertBefore(L(), T());
   list.insertFirst(T());
   list.insertLast(T());
   list[T()] = T();
   list.removeFirst();
   list.removeLast();
   list.remove(L());

   L len_c = list_c.length();
   list_c.first();
   list_c.element(L());
   list_c.element(--len_c);
   list_c.last();
   list_c[T()];

   return s;
}

#endif // DEBUG

#endif // SIMPLE_LIST_H
