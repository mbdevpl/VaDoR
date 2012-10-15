#ifndef DOMINO_PROBLEM_INPUT_H
#define DOMINO_PROBLEM_INPUT_H

#include <fstream>
#include <string>

#include <rapidxml.hpp>

#include "simple_list.h"
#include "half_elem.h"
#include "domino_elem_located.h"

class domino_problem_input
{
public:
   typedef simple_list<domino_elem_located*,size_t> elements_t;
   typedef simple_list<half_elem*,size_t> column_t;
   typedef simple_list<column_t,size_t> board_t;
public:
   elements_t elements;
   size_t width;
   size_t height;
   board_t board;
public:
   domino_problem_input(const std::string& path);
   ~domino_problem_input()
   {
      // delete elements!
   }
   inline domino_elem_located* elem(size_t index) { return elements[index]; }
   inline size_t w() { return width; }
   inline size_t h() { return height; }
public:
   std::string board_str();
   friend std::ostream& operator<<(std::ostream& os, const domino_problem_input& pr)
   {
      os << '[' << pr.width << 'x' << pr.height << "]: " << pr.elements;
      return os;
   }
};

#endif // DOMINO_PROBLEM_INPUT_H
