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

using std::cout;
using std::endl;

using std::ifstream;

const int MAX_CHARS_PER_LINE = 128;
const int MAX_TOKENS_PER_LINE = 5;
const char* const DELIMITER = ";";



class domino_problem_input
{
private:
    void read_xml(const std::string& path);
    void read_txt(const std::string& path);

protected:
   typedef simple_list<domino_elem_located*,size_t> elements_t;
   typedef simple_list<half_elem*,size_t> column_t;
   typedef simple_list<column_t,size_t> board_t;
protected:
   // collection of domino_elem*
   elements_t elements;
   size_t width;
   size_t height;
   // collection of half_elem* that come from 'elements' field
   board_t board;
protected:
   domino_problem_input();
public:
   domino_problem_input(const std::string& path);
protected:
   domino_problem_input(const domino_problem_input& input)
      : elements(input.elements), width(input.width), height(input.height), board(input.board) { }
public:
   ~domino_problem_input()
   {
      // delete elements!
   }
   inline domino_elem_located* elem(size_t index) { return elements[index]; }
   inline size_t w() { return width; }
   inline size_t h() { return height; }
public:
   std::string board_str() const;
   friend std::ostream& operator<<(std::ostream& os, const domino_problem_input& input)
   {
      os << '[' << input.width << 'x' << input.height << "]: " << input.elements << "\n";
      os << input.board_str();
      return os;
   }
};

#endif // DOMINO_PROBLEM_INPUT_H
