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

#ifdef DEBUG
#include <typeinfo> // test()
#include <string>
#include <ostream>
#include <iostream>
#include <sstream>
#endif // DEBUG

//#include <initializer_list>

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
  */
template<typename T, typename L>
class simple_list
{
private:
   // Single element of the list, pointing to the next and previous elements.
   class elem_raw
   {
   private:
      // Value of this element of the list.
      T elem_value;
      // Pointer to the next element.
      elem_raw* ptr_next;
      // Pointer to the previous element.
      elem_raw* ptr_prev;
   public:
      // Default constructor.
      inline elem_raw()
         : elem_value(), ptr_next(0), ptr_prev(0) { }
      // Constructs element with specified value.
      inline elem_raw(const T& value)
         : elem_value(value), ptr_next(0), ptr_prev(0) { }
      // Constructs element with specified value and specified next & previous elements.
      inline elem_raw(const T& value, elem_raw* next, elem_raw* prev)
         : elem_value(value), ptr_next(next), ptr_prev(prev) { }
      // Copy constructor.
      inline elem_raw(const elem_raw& e)
         : elem_value(e.elem_value), ptr_next(e.ptr_next), ptr_prev(e.ptr_prev) { }
      // Destructor.
      inline ~elem_raw() { elem_value = T(); ptr_prev = ptr_next = 0; }
   public:
      // Copy assignment operator.
      inline elem_raw& operator=(const elem_raw& e);
      // Returns value of this element.
      inline T value() const { return elem_value; }
      // Returns reference to the value of this element.
      inline T& value_ref() { return elem_value; }
      // Returns reference to the value of this element.
      inline const T& value_ref_const() const { return elem_value; }
      // Gets the reference to the next element.
      inline elem_raw*& next() { return this->ptr_next; }
      // Gets the reference to previous element.
      inline elem_raw*& prev() { return this->ptr_prev; }
#ifdef DEBUG
   public:
      static std::string test();
#endif
   };

public:
   // Advanced pointer to the element of the list, const-correct.
   class elem_const
   {
   protected:
      // pointer to the list which is
      simple_list<T,L>* list;
      // raw pointer to the actual data
      elem_raw* e;
   public:
      // Default constructor.
      inline elem_const()
         : list(nullptr), e(nullptr) { }
   protected:
      inline elem_const(const T& value)
         : list(nullptr), e(new elem_raw(value)) { }
      //      inline elem_const(elem_raw* e_ptr, L /*index*/)
      //         : list(nullptr), e(e_ptr) { }
   public:
      inline elem_const(const elem_const& element_const)
         : list(element_const.list), e(element_const.e) { }
      inline virtual ~elem_const() { list = 0; e = 0; }
   public:
      // Returns value stored in this element.
      inline T value() const { return e->value(); }
      // Reference to the value stored in this element.
      inline const T& value_ref_const() const { return e->value_ref_const(); }
   public:
      /*!
        \brief Checks if there is a raw pointer assigned to this element.

        This method is useful in for loops involving simple_list:
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

        \return True if there is a raw pointer assigned to this element.
        */
      inline bool valid() const { return (e ? true : false); }
      // Returns true if there is no raw pointer assigned to this element.
      inline bool empty() const { return (e ? false : true); }
      // Returns true if this element is connected to a list
      inline bool connected() const { return (list ? true : false); }
   public:
      // Goes to the next element.
      const elem_const& forward() const;
      // Moves forward by the specified ammount.
      const elem_const& forward(L count) const;
      // Goes to the previous element.
      const elem_const& back() const;
      // Moves back by the specified ammount.
      const elem_const& back(L count) const;
   public:
      const elem_const& operator=(const elem_const& element_const) const;
      // Moves forward by the specified ammount.
      const elem_const& operator+=(L count) const { return forward(count); }
      // Moves back by the specified ammount.
      const elem_const& operator-=(L count) const { return back(count); }
      // Goes to the next element.
      const elem_const& operator++() const { return forward(); }
      // Goes to the previous element.
      const elem_const& operator--() const { return back(); }
      bool operator==(const elem_const& element_const) const
      {
         if(e == element_const.e && list == element_const.list)
            return true;
         return false;
      }
      bool operator!=(const elem_const& element_const) const
      {
         if(e == element_const.e && list == element_const.list)
            return false;
         return true;
      }
      // Returns copy of the value stored in this element.
      inline const T& operator*() const { return value_ref_const(); }
      inline operator bool() const { return valid(); }
#ifdef DEBUG
   public:
      static std::string test();
#endif
   };

public:
   // Advanced pointer to the element of the list. Can be used to modify the list.
   class elem : public elem_const
   {
   public:
      // Default constructor.
      inline elem()
         : elem_const() { }
   private:
      inline elem(const T& value)
         : elem_const(value) { }
   public:
      //      inline elem(elem_raw* e_ptr, L index)
      //         : elem_const(e_ptr, index) { }
      inline elem(const elem& element)
         : elem_const(element) { }
      inline virtual ~elem() { }
   public:
      // Goes to the next element.
      elem& forward();
      // Moves forward by the specified ammount.
      elem& forward(L count);
      // Goes to the previous element.
      elem& back();
      // Moves back by the specified ammount.
      elem& back(L count);
      /*!
        \brief Connects this element to the given list, unless it is already connected.

        Connection indicates that the element is contained in the connected list.
        This has a lot of consequences for the behaviour of the list, because a connected
        element can directy change values of fields of the list.

        Elements returned via first() element() and last() methods of simple_list are already
        connected to the list, on which the methods are executed.

        For example, if a connected element is removed, the list length decreses.
        If it was also a last element of that list, the last_elem field value changes so that
        it points to the real last element.
        */
      inline void connectTo(simple_list<T,L>* the_list)
      {
         if(connected())
            return;
         list = the_list;
      }
   public:
      // Returns copy of this element.
      inline elem copy() const { return elem(*this); }
      // Reference to the value stored in this element.
      inline T& value_ref() { return e->value_ref(); }
   private:
      inline void clear() { e = 0; }
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
      /*!
        \brief Releases the memory reserved for this element.

        Releases the memory, and if element is connected (by default it is)
        this method removes the element from the list.
        */
      void remove();
   public:
      elem& operator=(const elem& e_ptr);
      // Moves forward by the specified ammount.
      elem& operator+=(L count) { return forward(count); }
      // Moves back by the specified ammount.
      elem& operator-=(L count) { return back(count); }
      // Goes to the next element.
      inline elem& operator++() { return forward(); }
      // Goes to the previous element.
      inline elem& operator--() { return back(); }
      // Reference to the value stored in this element.
      inline T& operator*() { return value_ref(); }
      inline operator elem_const() { return elem_const(*this); }
      inline operator elem_raw*() { return e; }
   public:
      static inline void clear(elem& element) { element.clear(); }
#ifdef DEBUG
   public:
      static std::string test();
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
public:
   const T& operator[](const L& index) const;
   T& operator[](const L& index);
   inline simple_list<T,L>& operator=(const simple_list<T,L>& list) { return initFrom(list); }
   // Concatenates two lists.
   inline simple_list<T,L>& operator+=(const simple_list<T,L>& list) { return append(list); }
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
      : simple_list() { }
   simpler_list(const simpler_list<T>& list)
      : simple_list(list) { }
   simpler_list(const T& value)
      : simple_list(value) { }
   simpler_list(unsigned long long length, const T array[])
      : simple_list(length, array) { }
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

template<typename T, typename L>
typename simple_list<T,L>::elem_raw& simple_list<T,L>::elem_raw::operator=(
      const elem_raw& e)
{
   elem_value = e.elem_value;
   ptr_next = e.ptr_next;
   ptr_prev = e.ptr_prev;
   return *this;
}

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

template<typename T, typename L>
const typename simple_list<T,L>::elem_const& simple_list<T,L>::elem_const::operator=(
      const elem_const& element_const) const
{
   *(simple_list<T,L>**)(&list) = element_const.list;
   *(elem_raw**)(&e) = element_const.e;
   return *this;
}

template<typename T, typename L>
typename simple_list<T,L>::elem& simple_list<T,L>::elem::forward()
{
   if(empty())
      return *this;
   e = e->next();
   return *this;
}

template<typename T, typename L>
typename simple_list<T,L>::elem& simple_list<T,L>::elem::forward(L count)
{
   if(!count || empty())
      return *this;
   for(;count > 0; --count)
      e = e->next();
   return *this;
}

template<typename T, typename L>
typename simple_list<T,L>::elem& simple_list<T,L>::elem::back()
{
   if(empty())
      return *this;
   e = e->prev();
   return *this;
}

template<typename T, typename L>
typename simple_list<T,L>::elem& simple_list<T,L>::elem::back(L count)
{
   if(!count || empty())
      return *this;
   for(;count > 0; --count)
      e = e->prev();
   return *this;
}

template<typename T, typename L>
typename simple_list<T,L>::elem& simple_list<T,L>::elem::operator=(const elem& e_ptr)
{
   elem_const::operator =(e_ptr);
   return *this;
}

template<typename T, typename L>
void simple_list<T,L>::elem::delete_and_clear()
{
   if(empty())
      return;
   // organize connected elements
   if(e->prev())
      e->prev()->next() = e->next();
   if(e->next())
      e->next()->prev() = e->prev();
   // release memory
   delete e;
   clear();
}

template<typename T, typename L>
void simple_list<T,L>::elem::insertNext(const T& value)
{
   if(empty())
   {
      if(connected() && list->list_length == 0)
      {
         e = new elem_raw(value);
         list->last_elem = list->first_elem = *this;
         list->list_length = 1;
      }
      return;
   }
   elem_raw* new_elem = new elem_raw(value);
   new_elem->prev() = e;
   if(e->next()) {
      new_elem->next() = e->next();
      e->next()->prev() = new_elem;
   }
   e->next() = new_elem;
   if(connected())
   {
      L& len = list->list_length;
      ++len;
      if(len == 1)
         throw std::runtime_error("something went wrong: empty list has a non empty element");
      if(e == list->last_elem.e)
         list->last_elem.forward();
      return;
   }
}

template<typename T, typename L>
void simple_list<T,L>::elem::insertPrev(const T& value)
{
   if(empty())
   {
      if(connected() && list->list_length == 0)
      {
         e = new elem_raw(value);
         list->last_elem = list->first_elem = *this;
         list->list_length = 1;
      }
      return;
   }
   elem_raw* new_elem = new elem_raw(value);
   new_elem->next() = e;
   if(e->prev()) {
      new_elem->prev() = e->prev();
      e->prev()->next() = new_elem;
   }
   e->prev() = new_elem;
   if(connected())
   {
      L& len = list->list_length;
      ++len;
      if(len == 1)
         throw std::runtime_error("something went wrong: empty list has a non empty element");
      if(e == list->first_elem.e)
         list->first_elem.back();
      return;
   }
}

template<typename T, typename L>
void simple_list<T,L>::elem::remove()
{
   if(empty())
      return;
   if(connected())
   {
      L& len = list->list_length;
      --len;
      if(!len)
      {
         // zero elements left => this is the only elem in the list
         list->first_elem.clear();
         list->last_elem.clear();
      }
      else if(e == list->first_elem.e)
         list->first_elem.forward();
      else if(e == list->last_elem.e)
         list->last_elem.back();
   }
   delete_and_clear();
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
      if(e.value_ref() == value)
         return e;
   return elem();
}

template<typename T, typename L>
const typename T& simple_list<T,L>::operator[](const L& index) const
{
   return element(index).value_ref_const();
}

template<typename T, typename L>
typename T& simple_list<T,L>::operator[](const L& index)
{
   return element(index).value_ref();
}

#ifndef SIMPLE_LIST_NOSTRINGS

template<typename T, typename L>
std::string simple_list<T,L>::str() const
{
   std::stringstream s;
   s << '{';
   for(simple_list<T,L>::elem_const e = first(); e; ++e)
   {
      s << ' ' << *e;
      if(e != last())
         s << ',';
   }
   s << " }";
   return s.str();
}

#endif // SIMPLE_LIST_NOSTRINGS

#ifdef DEBUG

template<typename T, typename L>
std::string simple_list<T,L>::elem_raw::test()
{
   int size_val = sizeof(T) * 8;
   int size_next = sizeof(elem_raw*) * 8;
   int size_prev = sizeof(elem_raw*) * 8;
   int size = size_val + size_next + size_prev;
   std::stringstream s;
   s << " elem_raw:";
   s << " size = " << size;
   s << " (size_val = " << size_val
     << " size_next = " << size_next
     << " size_prev = " << size_prev << ")";
   s << "\n";

   //typedef simple_list<T,L>::elem_raw el_r;
   elem_raw e1;
   elem_raw e2(0);
   elem_raw e3(T(), &e1, &e2);
   elem_raw e4(e3);

   e1.next() = &e4;
   e1.prev() = &e4;
   e1.value();
   e1.value_ref();
   e1 = e2;

   return s.str();
}

template<typename T, typename L>
std::string simple_list<T,L>::elem_const::test()
{
   int size_list = sizeof(simple_list<T,L>*) * 8;
   int size_e = sizeof(simple_list<T,L>::elem_raw*) * 8;
   int size = size_list + size_e;
   std::stringstream s;
   s << " elem_const:";
   s << " size = " << size;
   s << " (size_list = " << size_list
     << " size_e = " << size_e << ")";
   s << "\n";

   T x;
   bool y;
   elem_const a;
   elem_const b(0); // memory alloc
   elem_const c(b);
   elem_raw* ptr = a.e;
   a = c;
   y = a.connected();
   y = a.empty();
   x = *a;
   y = a.valid();
   x = a.value();
   a.forward();
   a.forward(2);
   a.back();
   a.back(2);
   ++a;
   a+=2;
   --a;
   a-=2;

   delete ptr;

   return s.str();
}

template<typename T, typename L>
std::string simple_list<T,L>::elem::test()
{
   T x;
   std::stringstream s;
   elem a;
   elem b(0); // memory alloc
   elem c(b);
   elem_raw* d;
   elem_const e;

   a = c;
   c.copy();
   a.connectTo(0);
   x = *a;
   *a = x;
   d = (elem_raw*)a;
   e = (elem_const)a;
   a.forward();
   a.forward(2);
   a.back();
   a.back(2);
   ++a;
   a+=2;
   --a;
   a-=2;

   a.clear();
   elem::clear(a);
   a.delete_and_clear();

   a.remove();
   a.insertNext(0);
   a.insertPrev(0);

   delete d;

   return s.str();
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
   s << elem_raw::test();
   s << elem_const::test();
   s << elem::test();
   s << " list:";
   s << " size = " << size;
   s << " (size_first = " << size_first
     << " size_last = " << size_last
     << " size_len = " << size_len << ")";
   s << "\n";

   test_iteration(s);

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
