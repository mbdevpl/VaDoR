/*!
simple_container: macros for container templates
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
#ifndef SIMPLE_CONTAINER_H
#define SIMPLE_CONTAINER_H

#ifdef DEBUG
#include <ostream>
#include <iostream>
#endif

#define ELEM_RAW_BASIC \
   private: \
   /*! Value of this element of the list. */ \
   T elem_value; \
   inline elem_raw(const elem_raw& e) \
   : elem_value(e.elem_value) { } \
   inline elem_raw& operator=(const elem_raw& e) \
{ \
   elem_value = e.elem_value; \
   return *this; \
   } \
   public: \
   /*! Returns value of this element. */ \
   inline T value() const { return elem_value; } \
   /*! Returns reference to the value of this raw element. */ \
   inline T& value_ref() { return elem_value; } \
   /*! Returns const reference of the value of this raw element. */ \
   inline const T& value_ref_const() const { return elem_value; }

#define ELEM_RAW_DEF_CONSTR \
   /*! Default constructor. */ \
   inline elem_raw() \
   : elem_value()

#define ELEM_RAW_VAL_CONSTR \
   /*! Constructs element with specified value. */ \
   inline elem_raw(const T& value) \
   : elem_value(value)

#define ELEM_RAW_DESTR \
   /*! Destructor. */ \
   inline ~elem_raw()

#define ELEM_RAW_DESTR_T \
   elem_value = T();

///*!
//  \brief Advanced pointer to the element of the list, const-correct.
//  */
#define ELEM_CONST_BASIC \
   protected: \
   /*! Pointer to the container, which contains this element. */ \
   C* c; \
   /*! Raw pointer to the actual data. */ \
   elem_raw* e; \
   public: \
   /*! Default constructor. */ \
   inline elem_const() \
   : c(nullptr), e(nullptr) { } \
   /*! Copy constructor. */ \
   inline elem_const(const elem_const& element_const) \
   : c(element_const.c), e(element_const.e) { } \
   /*! Copy assignment operator. */ \
   inline const elem_const& operator=(const elem_const& element_const) const \
{ \
   *(C**)(&c) = element_const.c; \
   *(elem_raw**)(&e) = element_const.e; \
   return *this; \
   } \
   /*! Destructor. */ \
   inline virtual ~elem_const() { c = nullptr; e = nullptr; } \
   protected: \
   inline elem_const(const T& value) \
   : c(nullptr), e(new elem_raw(value)) { } \
   public: \
   /*! Returns value stored in this element. */ \
   inline T value() const { return e->value(); } \
   /*! Reference to the value stored in this element. */ \
   inline const T& value_ref_const() const { return e->value_ref_const(); } \
   /*!
   \brief Checks if there is a raw pointer assigned to this element.
   \return True if there is a raw pointer assigned to this element.
   */ \
   inline bool valid() const { return (e ? true : false); } \
   /*! Returns true if there is no raw pointer assigned to this element. */ \
   inline bool empty() const { return (e ? false : true); } \
   /*! Returns true if this element is connected to a list */ \
   inline bool connected() const { return (c ? true : false); } \
   inline bool operator==(const elem_const& element_const) const \
{ \
   if(e == element_const.e && c == element_const.c) \
   return true; \
   return false; \
   } \
   inline bool operator!=(const elem_const& element_const) const \
{ \
   if(e == element_const.e && c == element_const.c) \
   return false; \
   return true; \
   } \
   /*! Returns const reference of the value stored in this element. */ \
   inline const T& operator*() const { return value_ref_const(); } \
   inline operator bool() const { return valid(); }

#define ELEM_CONST_TRAVERSE(decrement,increment) \
   /*! Moves back by the specified ammount. */ \
   const elem_const& operator-=(L count) const { return decrement(count); } \
   /*! Goes to the previous element. */ \
   const elem_const& operator--() const { return decrement(); } \
   /*! Goes to the next element. */ \
   const elem_const& operator++() const { return increment(); } \
   /*! Moves forward by the specified ammount. */ \
   const elem_const& operator+=(L count) const { return increment(count); }

//   public: \
//   /*! Goes to the next element. */ \
//   const elem_const& increment() const; \
//   /*! Moves forward by the specified ammount. */ \
//   const elem_const& increment(L count) const; \
//   /*! Goes to the previous element. */ \
//   const elem_const& decrement() const; \
//   /*! Moves back by the specified ammount. */ \
//   const elem_const& decrement(L count) const;

///*!
//  \brief Advanced pointer to the element of the list. Can be used to modify the list.
//  */
#define ELEM_BASIC \
   public: \
   /*! Default constructor. */ \
   inline elem() \
   : elem_const() { } \
   inline elem(const elem& element) \
   : elem_const(element) { } \
   inline virtual ~elem() { } \
   private: \
   inline elem(const T& value) \
   : elem_const(value) { } \
   inline void clear() { e = 0; } \
   public: \
   /*! Returns copy of this element. */ \
   inline elem copy() const { return elem(*this); } \
   /*! Reference to the value stored in this element. */ \
   inline T& value_ref() { return e->value_ref(); } \
   public: \
   elem& operator=(const elem& element) \
{ \
   elem_const::operator=(element); \
   return *this; \
   } \
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
   */ \
   inline void connectTo(C* the_c) \
{ \
   if(connected()) \
   return; \
   c = the_c; \
   } \
   /*! Reference to the value stored in this element. */ \
   inline T& operator*() { return value_ref(); } \
   inline operator elem_const() { return elem_const(*this); } \
   inline operator elem_raw*() { return e; }

#define ELEM_TRAVERSE(decrement,increment) \
public: \
   /*! Moves back by the specified ammount. */ \
   elem& operator-=(L count) { return decrement(count); } \
   /*! Goes to the previous element. */ \
   inline elem& operator--() { return decrement(); } \
   /*! Goes to the next element. */ \
   inline elem& operator++() { return increment(); } \
   /*! Moves forward by the specified ammount. */ \
   elem& operator+=(L count) { return increment(count); }

#ifdef DEBUG

#define ELEM_RAW_BASIC_TEST \
   int size = (sizeof(elem_raw) * 8); \
   s << " elem_raw:"; \
   s << " size = " << size; \
   s << std::endl; \
   T t; \
   elem_raw elem_raw_1; \
   elem_raw elem_raw_2(t); \
   elem_raw elem_raw_3(elem_raw_2); \
   elem_raw_1.value(); \
   elem_raw_1.value_ref(); \
   elem_raw_1.value_ref_const(); \
   elem_raw_1 = elem_raw_3;

#define ELEM_CONST_BASIC_TEST \
   int size_list = sizeof(C*) * 8; \
   int size_e = sizeof(C::elem_raw*) * 8; \
   int size = sizeof(elem_const) * 8; \
   s << " elem_const:"; \
   s << " size = " << size; \
   s << " (size_list = " << size_list \
     << " size_e = " << size_e << ")"; \
   s << std::endl; \
   T x; \
   bool y; \
   elem_const elem_const_1; \
   elem_const elem_const_2(0); /* memory alloc */ \
   elem_const elem_const_3(elem_const_2); \
   elem_raw* ptr = elem_const_1.e; \
   elem_const_1 = elem_const_3; \
   y = elem_const_1.connected(); \
   y = elem_const_1.empty(); \
   x = *elem_const_1; \
   y = elem_const_1.valid(); \
   x = elem_const_1.value(); \
   ++elem_const_1; \
   elem_const_1+=2; \
   --elem_const_1; \
   elem_const_1-=2; \
   delete ptr;

#define ELEM_BASIC_TEST \
   T x; \
   elem a; \
   elem b(0); /* memory alloc */ \
   elem c(b); \
   elem_raw* d; \
   elem_const e; \
   a = c; \
   c.copy(); \
   a.connectTo(0); \
   x = *a; \
   *a = x; \
   d = (elem_raw*)a; \
   e = (elem_const)a; \
   ++a; \
   a+=2; \
   --a; \
   a-=2; \
   a.clear(); \
   delete d;

#define CONTAINER_BASIC_TEST \
   elem_raw::test(s); \
   elem_const::test(s); \
   elem::test(s);

#endif // DEBUG

#endif // SIMPLE_CONTAINER_H
