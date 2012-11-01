#ifndef DOMINO_PROBLEM_INPUT_H
#define DOMINO_PROBLEM_INPUT_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <streambuf>

#include <rapidxml.hpp>

#include "simple_list.h"
#include "half_elem.h"
#include "domino_elem_located.h"

class domino_problem_input
{
protected:
   typedef simple_list<domino_elem_located*,size_t> elements_t;
   typedef simple_list<half_elem*,size_t> column_t;
   typedef simple_list<column_t,size_t> board_t;
protected:
   // Indication that this object allocated memory under its pointers.
   bool alloc;
   // Collection of domino_elem_located pointers.
   elements_t* elements;
   size_t width;
   size_t height;
   // Collection of half_elem* that come from 'elements' field.
   board_t board;
   // Impossible to remove due to size of the board.
   elements_t* invalid;
   // Algorithm does not know anything about these pieces.
   elements_t* unresolved;
   // Possible to remove if other pieces are placed right.
   elements_t checked;

   unsigned long long checked_key;

   bool is_compact;
protected:
   domino_problem_input();
   domino_problem_input(const domino_problem_input& input);
public:
   domino_problem_input(const std::string& path);
   ~domino_problem_input();
   void release();
private:
   void read_txt(const std::string& path);
   void read_xml(const std::string& path);
   void init_board();
   // checks all unresolved pieces to know which cannot be removed at all
   void resolve_elements();
public:
   // Packs the object, decreasing memory usage.
   void pack();
   // Unpacks the object, increasing memory usage.
   void expand();
   const elements_t::elem_const elements_first() const { return elements->first(); }
   size_t size() const { return elements->length(); }
   inline domino_elem_located* elem(size_t index) { return elements->element(index).value_ref(); }
   inline size_t w() { return width; }
   inline size_t h() { return height; }
   std::string board_str() const;
   friend std::ostream& operator<<(std::ostream& os, const domino_problem_input& input)
   {
      os << '[' << input.width << 'x' << input.height << "]: " << *(input.elements) << "\n";
      os << input.board_str();
      return os;
   }
};

#endif // DOMINO_PROBLEM_INPUT_H
